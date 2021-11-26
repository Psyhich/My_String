#include <cstddef>
#include <new>
#include <iostream>

#include "my_string.h"
/*
*	Be advised, TryToAllocate doesn't take care of releasing resources
* */
void MyStructs::CMyString::TryToAllocate(const size_t nLength) noexcept {
	if(nLength == 0)
	{
		m_szData = nullptr;
	}else 
	{
		m_szData = new(std::nothrow) char[nLength];
	}
	if(m_szData != nullptr)
	{
		m_nSize = nLength;
	} else {
		m_nSize = 0;
	}
}

std::size_t MyStructs::CMyString::GetStringLength(const char *cszStringToCount) noexcept {
	if(cszStringToCount == nullptr)
	{
		return 0;
	}

	std::size_t nStringLength = 0;
	char chCurrentChar = cszStringToCount[0];
	while(chCurrentChar != '\0')
	{
		++nStringLength;
		chCurrentChar = cszStringToCount[nStringLength];
	}
	// Adding 1 because string is zero terminated
	return nStringLength + 1;
}

void MyStructs::CMyString::CopyString(const char *cszStringToCopy)
{
	if(cszStringToCopy == nullptr)
	{
		return;
	}

	std::size_t nIndex = 0;
	while(cszStringToCopy[nIndex] != '\0')
	{
		m_szData[nIndex] = cszStringToCopy[nIndex];
		++nIndex;
	}
	m_szData[nIndex] = '\0';
}

void MyStructs::CMyString::AppendToString(const char *cszStringToAppend)
{
	if(cszStringToAppend == nullptr)
	{
		return;
	}
	
	const size_t cnTerminantIndex = m_nSize - 1;
	
	ReinitializeAndCopy(m_szData, size() + GetStringLength(cszStringToAppend) - 1);

	for(size_t nIndex = cnTerminantIndex; true; nIndex++)
	{
		const char cchCharToAppend = cszStringToAppend[nIndex - cnTerminantIndex];
		m_szData[nIndex] = cchCharToAppend;
		if(cchCharToAppend == '\0')
		{
			break;
		}
	}
}

void MyStructs::CMyString::Insert(const char* cszStringToInsert, const size_t nStartPosition, const size_t nCountOfCharsToInsert)
{
	const size_t nLengthOfInserted = GetStringLength(cszStringToInsert);
	if(nStartPosition > size() || nLengthOfInserted == 0)
	{
		printf("Tried to reach wrong position!\n");
		return;
	}
	if(nLengthOfInserted < nCountOfCharsToInsert)
	{
		printf("Tried to copy more characters than present\n");
		return;
	}

	const size_t cnNewSize = size() + nCountOfCharsToInsert;
	char *szNewString = new char[cnNewSize]; 

	size_t nCurrentPosition = 0;
	size_t nEndPosition = nStartPosition + nCountOfCharsToInsert;
	// Copying first part
	for(; nCurrentPosition < cnNewSize; nCurrentPosition++)
	{
		if(nCurrentPosition < nStartPosition) // Copying first part
		{
			szNewString[nCurrentPosition] = m_szData[nCurrentPosition];
		} else if(nCurrentPosition < nStartPosition + nCountOfCharsToInsert) // Inserting string to insert
		{
			szNewString[nCurrentPosition] = cszStringToInsert[nCurrentPosition - nStartPosition];
		} else // Copying back first string
		{
			szNewString[nCurrentPosition] = m_szData[nStartPosition + nCurrentPosition - nEndPosition];
		}
	}

	delete[] m_szData;
	m_szData = szNewString;
	m_nSize = cnNewSize;
}

void MyStructs::CMyString::ReinitializeAndCopy(const char* cszStringToCopy, const std::size_t& nStringLength)
{
	if(cszStringToCopy == nullptr)
	{
		m_nSize = 0;
		if(m_szData != nullptr){
			delete[] m_szData;
			m_szData = nullptr;
		}
		return;
	}
	
	bool bShouldCleanup = m_szData != nullptr && m_nSize != nStringLength;
	// Doing check if we need to reinitialize the char array
	char *szOldString = m_szData;
	m_szData = nullptr;
	if(m_szData == nullptr || bShouldCleanup)
	{
		TryToAllocate(nStringLength);
	} else 
	{
		m_szData = szOldString;
	}

	CopyString(cszStringToCopy);
	//
	// Cleaning data we used before
	if(bShouldCleanup){
		delete[] szOldString;
	}
	
}

MyStructs::CMyString::CMyString(const char* cszCharsSequence) 
{
	TryToAllocate(GetStringLength(cszCharsSequence));

	CopyString(cszCharsSequence);
}

MyStructs::CMyString::CMyString(const MyStructs::CMyString& cStringToCopy)
{
	ReinitializeAndCopy(cStringToCopy.data(), cStringToCopy.size());
}

MyStructs::CMyString& MyStructs::CMyString::operator=(const MyStructs::CMyString& cStringToCopy)
{
	if(&cStringToCopy == this) 
	{
		return *this;
	}
	ReinitializeAndCopy(cStringToCopy.data(), cStringToCopy.size());

	return *this;
}

// Destructor
MyStructs::CMyString::~CMyString()
{
	if(m_szData != nullptr)
	{
		delete[] m_szData;
	}
}

void MyStructs::CMyString::Delete(size_t nPosition, size_t nCharactersCount) noexcept
{
	if(IsOutOfBounds(nPosition, nCharactersCount))
	{
		return;
	}
	if(nCharactersCount == size() - 1) 
	{
		delete[] m_szData;
		m_szData = nullptr;
		m_nSize = 0;
		return;
	}

	const size_t cnOldSize = m_nSize;
	// Calculating new size with getting in mind that user can delete zero terminant
	m_nSize -= nCharactersCount - IsReachedTerminator(nPosition, nCharactersCount);


	char *szOldString = m_szData; 
	TryToAllocate(m_nSize);
	if(m_szData == nullptr)
	{
		return;
	}
	
	size_t nOldStringPosition = 0;
	size_t nNewStringPosition = 0;

	// Moving to cnOldSize - 2, to manualy add '\0' in the end in case we deleted it
	while(nOldStringPosition < cnOldSize - 1) 
	{
		if(nOldStringPosition == nPosition)
		{
			nOldStringPosition += nCharactersCount;
			continue;
		}
		
		m_szData[nNewStringPosition] = szOldString[nOldStringPosition];
		++nOldStringPosition;
		++nNewStringPosition;
	}

	m_szData[m_nSize - 1] = '\0';

	delete[] szOldString;
}

MyStructs::CMyString MyStructs::CMyString::Substring(size_t nPosition, size_t nCharactersCount) const noexcept
{
	CMyString substring{nullptr}; 
	if(nPosition >= m_nSize || nCharactersCount == 0 || 
		m_nSize - nPosition < nCharactersCount)
	{
		return substring;
	}
	substring.TryToAllocate(nCharactersCount);
	if(substring.m_szData == nullptr)
	{
		return substring;
	}

	const size_t cnEndPosition = nPosition + nCharactersCount;
	for(size_t nCurrentIndex = nPosition; nCurrentIndex < cnEndPosition; nCurrentIndex++)
	{
		substring.m_szData[nCurrentIndex - nPosition] = m_szData[nCurrentIndex];
	}

	return substring;
}
std::optional<size_t> MyStructs::CMyString::Find(const char *cszStringToFind) const noexcept
{
	if(cszStringToFind == nullptr)
	{
		return std::nullopt;
	}
	
	size_t nStringLength = GetStringLength(cszStringToFind);
	if(nStringLength <= 1)
	{
		return std::nullopt;
	}
	nStringLength -= 1; // Excluding terminator

	size_t nIndex = 0; // Index for iterating through this.size()
	size_t nCurrentStringIndex = 0; // Index for iterating through cszStringToFind
	bool bIsLookingFor = false;

	for(; nIndex < m_nSize; nIndex++)
	{
		if(m_szData[nIndex] != cszStringToFind[nCurrentStringIndex])
		{
			nCurrentStringIndex = 0;
			bIsLookingFor = false;
		} else if(m_szData[nIndex] == cszStringToFind[nCurrentStringIndex]){
			++nCurrentStringIndex;
			bIsLookingFor = true;
			// Breaking if we reached end of this string
			if(nCurrentStringIndex == nStringLength)
			{
				break;
			}

		}
	}

	return bIsLookingFor ? std::optional<size_t>(nIndex - nStringLength + 1) : std::nullopt;
}


MyStructs::CMyString MyStructs::CMyString::Trim(size_t nPosition, size_t nCharactersCount) noexcept
{
	CMyString newString{nullptr};
	if(IsOutOfBounds(nPosition, nCharactersCount))
	{
		return newString;
	}
	
	// Checking if we should just swap values
	if(nCharactersCount == size() - 1) 
	{
		std::swap(m_nSize, newString.m_nSize);
		std::swap(m_szData, newString.m_szData);
		return newString;
	}
	
	// Checking if we are triming terminator
	const size_t cnOldSize = size();
	char *cszOldString = m_szData;

	const bool cbIsReachedTerminator = IsReachedTerminator(nPosition, nCharactersCount);
	m_nSize = cnOldSize - nCharactersCount;
	newString.m_nSize = nCharactersCount + 1;

	if(cbIsReachedTerminator)
	{
		++m_nSize;
		--newString.m_nSize; // If we already will copy terminator reducing size by 1
	}

	newString.TryToAllocate(newString.m_nSize);
	TryToAllocate(m_nSize);
	// If we cannot allocate new data, backing up to previous and exiting
	if(!newString.data() || !data())
	{
		m_szData = cszOldString;
		m_nSize = cnOldSize;
		return newString;
	}

	const size_t endPosition = nPosition + nCharactersCount;
	for(size_t nIndex = 0; nIndex < cnOldSize - 1; nIndex++){
		if(nIndex < nPosition)
		{
			m_szData[nIndex] = cszOldString[nIndex];
		} else if(nIndex >= endPosition)
		{
			m_szData[nIndex - newString.size() + 1] = cszOldString[nIndex];
		} else 
		{
			newString.m_szData[nIndex - nPosition] = cszOldString[nIndex];
		}
		
	}

	// Manually setting terminators in case we have deleted them
	m_szData[size() - 1] = '\0';
	newString[newString.size() - 1] = '\0';

	delete[] cszOldString;
	return newString;
}

/*
void ToUpperCase(size_t nStartPos, size_t nLasPos) noexcept;
void ToUpperCase() noexcept;
void ToLowerCase(size_t nStartPos, size_t nLasPos) noexcept;
void ToLowerCase() noexcept;
*/

MyStructs::CMyString& MyStructs::CMyString::operator=(const char* cpszCharsSequence)
{
	size_t nNewSize = GetStringLength(cpszCharsSequence);
	ReinitializeAndCopy(cpszCharsSequence, nNewSize);
	return *this;
}
MyStructs::CMyString MyStructs::CMyString::operator+(const CMyString& cStringToAdd) const
{
	CMyString newString = this->data();
	newString.AppendToString(cStringToAdd.data());

	return newString;
}
// Checks if each character in strings are equal, also empty strings are equal
bool MyStructs::CMyString::operator==(const CMyString& cStringToCompare) const
{
	if(size() != cStringToCompare.size())
	{
		return false;
	}
	// If both strings have nullptrs they are equal
	if(data() == nullptr && cStringToCompare.data() == nullptr)
	{
		return true;
	}
	
	for(std::size_t nIndex = 0; nIndex < size(); nIndex++)
	{
		if(m_szData[nIndex] != cStringToCompare[nIndex])
		{
			return false;
		}
	}

	return true;
}
