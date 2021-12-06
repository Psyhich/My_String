#include <cstddef>
#include <new>
#include <iostream>
#include <cmath>

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
	if(IsOutOfBounds(nPosition, nCharactersCount))
	{
		printf("Given range is out of bounds\n");
		return substring;
	}
	substring.TryToAllocate(nCharactersCount);
	if(substring.m_szData == nullptr)
	{
		printf("Failed to allocate substring\n");
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
		printf("Given string is nullptr\n");
		return std::nullopt;
	}
	
	size_t nStringLength = GetStringLength(cszStringToFind);
	if(nStringLength <= 1)
	{
		printf("Given string is to small tp match anything\n");
		return std::nullopt;
	}
	nStringLength -= 1; // Excluding terminator

	size_t nIndex = 0; // Index for iterating through this.size()
	size_t nCurrentStringIndex = 0; // Index for iterating through cszStringToFind
	bool bIsFoundString = false;

	for(; nIndex < m_nSize; nIndex++)
	{
		if(m_szData[nIndex] != cszStringToFind[nCurrentStringIndex])
		{
			nCurrentStringIndex = 0;
		} else {
			++nCurrentStringIndex;
			// Breaking if we reached end of this string
			if(nCurrentStringIndex == nStringLength)
			{
				bIsFoundString = true;
				break;
			}

		}
	}

	return bIsFoundString ? std::optional<size_t>(nIndex - nStringLength + 1) : std::nullopt;
}


MyStructs::CMyString MyStructs::CMyString::Trim(size_t nPosition, size_t nCharactersCount, char cFilter) const noexcept
{
	CMyString newString{nullptr};
	if(IsOutOfBounds(nPosition, nCharactersCount))
	{
		printf("Given range is out of bounds\n");
		return newString;
	}
	const size_t cnEndPosition  = nPosition + nCharactersCount;
	size_t nNewSize{0};
	// Calculating new size of string
	for(size_t nCurrentIndex = nPosition; nCurrentIndex < cnEndPosition; nCurrentIndex++)
	{
		if(m_szData[nCurrentIndex] != cFilter)
		{
			nNewSize += 1;
		}
	}

	if(cnEndPosition != size())
	{
		++nNewSize;
	}

	if(nNewSize == 1) // 1 because we already included '\0' in any variant
	{
		return newString;
	}

	newString.TryToAllocate(nNewSize);
	if(newString.data() == nullptr)
	{
		printf("Failed to allocate new string\n");
		return newString;
	}

	// Assigning values from current string to new one while excluding filter
	size_t nRealIndex = 0;
	for(size_t nCurrentIndex = nPosition; nCurrentIndex < cnEndPosition; nCurrentIndex++)
	{
		if(m_szData[nCurrentIndex] != cFilter)
		{
			newString.m_szData[nRealIndex] = m_szData[nCurrentIndex];
			++nRealIndex;
		}
	}
	// Assigning \0 in case range doesn't cover it
	newString.m_szData[nNewSize - 1] = '\0';
	return newString;
}

void MyStructs::CMyString::ToUpperCase(size_t nStartPos, size_t nLasPos) noexcept
{
	if(IsOutOfBounds(nStartPos, nLasPos - nStartPos))
	{
		return;
	}

	for(size_t nIndex = nStartPos; nIndex < nLasPos; nIndex++)
	{
		if(m_szData[nIndex] >= 'a' && m_szData[nIndex] <= 'z')
		{
			m_szData[nIndex] = m_szData[nIndex] - 32;
		}
	}
}

void MyStructs::CMyString::ToLowerCase(size_t nStartPos, size_t nLasPos) noexcept
{
	if(IsOutOfBounds(nStartPos, nLasPos - nStartPos))
	{
		return;
	}

	for(size_t nIndex = nStartPos; nIndex < nLasPos; nIndex++)
	{
		if(m_szData[nIndex] >= 'A' && m_szData[nIndex] < 'Z')
		{
			m_szData[nIndex] = m_szData[nIndex] + 32;
		}
	}

}

std::optional<int> MyStructs::CMyString::ToInt() const noexcept 
{
	if(data() == nullptr || size() <= 1)
	{
		return std::nullopt;
	}
	int iConstructedNumber = 0;
	int iCurrentPower = 1;
	for(size_t nIndex = 0; nIndex < size() - 1; nIndex++)
	{
		const int ciCurrentChar = (int)m_szData[size() - 2 - nIndex];
		if(ciCurrentChar >= 48 && ciCurrentChar <= 57)
		{
			iConstructedNumber += iCurrentPower  * (ciCurrentChar - 48);
		} else if(ciCurrentChar == '-') 
		{
			iConstructedNumber *= -1;
		} else 
		{
			return std::nullopt;
		}
		iCurrentPower *= 10;
	}

	return iConstructedNumber;
}

MyStructs::CMyString MyStructs::CMyString::FromInt(int iToConvert) noexcept
{
	// Counting number of radixes
	size_t nRadixCount = 0;
	int iNumber = iToConvert;
	while(iNumber)
	{
		iNumber /= 10;
		++nRadixCount;
	}

	CMyString newString;
	// Taking in mind '\0' and '-' sign if number is negative
	newString.TryToAllocate(nRadixCount + 1 + (iToConvert < 0));

	iNumber = iToConvert;
	for(size_t nCurrentRadix = 0; nCurrentRadix < nRadixCount; nCurrentRadix++)
	{
		newString[nCurrentRadix] = iNumber % 10 + 48;
		iNumber /= 10;
	}

	if(iToConvert < 0)
	{
		newString[newString.size() - 2] = '-';
	}
	newString[newString.size() - 1] = '\n';

	return newString;
}

std::optional<double> MyStructs::CMyString::ToDouble() const noexcept
{
	if(m_szData == nullptr || size() <= 1)
	{
		return std::nullopt;
	}

	size_t nDotPos = size() - 1;
	bool bIsFoundDot = false;
	// Looking for dot to know how much float radixes we have
	const bool bIsNegative = m_szData[0] == '-';
	for(size_t nIndex = bIsNegative; nIndex < size() - 1; nIndex++)
	{
		const char ciCurrentChar = m_szData[nIndex];
		if (ciCurrentChar == '.' && !bIsFoundDot) 
		{
			nDotPos = nIndex; // I won't break, because I still need to check for errors
			bIsFoundDot = true;
		} else if(ciCurrentChar < '0' || ciCurrentChar > '9')
		{
			return std::nullopt;
		}
	}

	double dNewDoubleVal = 0;
	double lRadixModifier = 1;
	for(size_t nIndex = 1 + bIsNegative; nIndex < nDotPos; nIndex++)
	{
		lRadixModifier *= 10.0;
	}

	if(nDotPos == 0) // Check for situations when we have ".<digits>"
	{
		lRadixModifier = 0.1;
	}

	for(size_t nIndex = bIsNegative; nIndex < size() - 1; nIndex++)
	{
		if(m_szData[nIndex] != '.')
		{
			// Jumping to next value because we don't need to divide by 0
			if(m_szData[nIndex] != '0')
			{
				int iParsedNumber = m_szData[nIndex] - (int)'0';
				dNewDoubleVal += iParsedNumber * lRadixModifier;
			}
			lRadixModifier /= 10.0;
		}
	}

	if(bIsNegative)
	{
		dNewDoubleVal *= -1;
	}


	return dNewDoubleVal;
}


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
