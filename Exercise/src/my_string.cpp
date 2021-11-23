#include <cstddef>
#include <new>
#include <iostream>

#include "my_string.h"

char* Exercise_1::CMyString::TryToAllocate(const size_t nLength) noexcept {
	char *pcAllocated{nullptr};
	if(nLength > 0)
	{
		pcAllocated = new(std::nothrow) char[nLength];
	}
	return pcAllocated;
}

std::size_t Exercise_1::CMyString::GetStringLength(const char *cszStringToCount) noexcept {
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

void Exercise_1::CMyString::CopyString(const char *cszStringToCopy)
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

void Exercise_1::CMyString::AppendToString(const char *cszStringToAppend)
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

void Exercise_1::CMyString::Insert(const char* cszStringToInsert, const size_t nStartPosition, const size_t nCountOfCharsToInsert)
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

void Exercise_1::CMyString::ReinitializeAndCopy(const char* cszStringToCopy, const std::size_t& nStringLength)
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
		m_nSize = nStringLength;
		m_szData = TryToAllocate(m_nSize);
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

Exercise_1::CMyString::CMyString(const char* cszCharsSequence) 
{
	m_nSize = GetStringLength(cszCharsSequence);
	m_szData = TryToAllocate(m_nSize);

	CopyString(cszCharsSequence);
}

Exercise_1::CMyString::CMyString(const Exercise_1::CMyString& cStringToCopy)
{
	ReinitializeAndCopy(cStringToCopy.data(), cStringToCopy.size());
}

Exercise_1::CMyString& Exercise_1::CMyString::operator=(const Exercise_1::CMyString& cStringToCopy)
{
	if(&cStringToCopy == this) 
	{
		return *this;
	}
	ReinitializeAndCopy(cStringToCopy.data(), cStringToCopy.size());

	return *this;
}

// Destructor
Exercise_1::CMyString::~CMyString()
{
	if(m_szData != nullptr)
	{
		delete[] m_szData;
	}
}

Exercise_1::CMyString& Exercise_1::CMyString::operator=(const char* cpszCharsSequence)
{
	size_t nNewSize = GetStringLength(cpszCharsSequence);
	ReinitializeAndCopy(cpszCharsSequence, nNewSize);
	return *this;
}
Exercise_1::CMyString Exercise_1::CMyString::operator+(const CMyString& cStringToAdd) const
{
	CMyString newString = this->data();
	newString.AppendToString(cStringToAdd.data());

	return newString;
}
// Checks if each character in strings are equal, also empty strings are equal
bool Exercise_1::CMyString::operator==(const CMyString& cStringToCompare) const
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
