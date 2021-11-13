#include "my_string.h"
#include <cstddef>

std::size_t Exercise_1::CMyString::GetStringLength(const char *cszStringToCount) noexcept {
	std::size_t nStringLength = 0;
	char cCurrentChar = cszStringToCount[0];
	while(cCurrentChar != '\0')
	{
		++nStringLength;
		cCurrentChar = cszStringToCount[nStringLength];
	}
	// Adding 1 because string is zero terminated
	return nStringLength + 1;
}

void Exercise_1::CMyString::CopyString(const char *cszStringToCopy)
{
	std::size_t nIndex = 0;
	for(; cszStringToCopy[nIndex] != '\0'; nIndex++)
	{
		m_szData[nIndex] = cszStringToCopy[nIndex];
	}
	m_szData[nIndex] = '\0';
}

void Exercise_1::CMyString::AppendToString(const char *cszStringToAppend)
{
	
	size_t nTerminantIndex = GetStringLength(m_szData) - 1;
	for(size_t nIndex = nTerminantIndex; true; nIndex++)
	{
		const char ccCharToAppend = cszStringToAppend[nIndex - nTerminantIndex];
		m_szData[nIndex] = ccCharToAppend;
		if(ccCharToAppend == '\0')
		{
			break;
		}
	}

}

void Exercise_1::CMyString::Insert(const char* cszStringToInsert, size_t nStartPosition, size_t nTimesToInsert)
{
	const size_t nLengthOfInserted = GetStringLength(cszStringToInsert);
	if(nStartPosition > size() || nLengthOfInserted == 0)
	{
		return; // Or it is better to throw? //TODO clarify which variant is better
	}

	const size_t nNewSize = size() + (nLengthOfInserted - 1) * nTimesToInsert; // Subtracting 1, because we need only one '\0'
	char *szNewString = new char[nNewSize]; 

	size_t nCurrentPosition = 0;
	// Copying first part
	for(; nCurrentPosition < nStartPosition; nCurrentPosition++)
	{
		szNewString[nCurrentPosition] = m_szData[nCurrentPosition];
	}
	// Beggining multiple insertion of given strings 
	size_t nEndPosition = nStartPosition; // Excluding '\0' part
	for(size_t nCountOfInsertions = 0; nCountOfInsertions < nTimesToInsert; nCountOfInsertions++)
	{
		nEndPosition += nLengthOfInserted - 1;
		for(; nCurrentPosition < nEndPosition; nCurrentPosition++)
		{
			szNewString[nCurrentPosition] = cszStringToInsert[nLengthOfInserted - 1 - (nEndPosition - nCurrentPosition)];
		}
	}
	// Appending last part
	for(; nCurrentPosition < nNewSize; nCurrentPosition++)
	{
		szNewString[nCurrentPosition] = m_szData[nStartPosition + nCurrentPosition - nEndPosition];
	}

	delete[] m_szData;
	m_szData = szNewString;
	m_nSize = nNewSize;
}

void Exercise_1::CMyString::ReinitializeAndCopy(const char* cszStringToCopy, const std::size_t& nStringLength)
{
	bool shouldCleanup = m_szData != nullptr && m_nSize != nStringLength;
	if(shouldCleanup)
	{
		delete[] m_szData;
	}
	// Doing check if we need to reinitialize the char array
	if(m_szData == nullptr || shouldCleanup)
	{
		m_nSize = nStringLength;
		m_szData = new char[m_nSize];
	}
	
	CopyString(cszStringToCopy);
}

Exercise_1::CMyString::CMyString(const char* cszCharsSequence) 
{
	m_nSize = GetStringLength(cszCharsSequence);
	m_szData = new char[m_nSize];

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
	if(m_szData != nullptr){
		delete[] m_szData;
	}
}

Exercise_1::CMyString& Exercise_1::CMyString::operator=(const char* cpszCharsSequence)
{
	std::size_t nNewSize = GetStringLength(cpszCharsSequence);
	ReinitializeAndCopy(cpszCharsSequence, nNewSize);
	return *this;
}
Exercise_1::CMyString Exercise_1::CMyString::operator+(const CMyString& cStringToAdd) const
{
	CMyString newString;
	newString.m_nSize = size() + cStringToAdd.size() - 1; // Removing 1 because string can contain only 1 terminator('\0')
	newString.m_szData = new char[newString.size()];
	// Copying both parts of string
	newString.CopyString(data());
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
		if(data()[nIndex] != cStringToCompare[nIndex])
		{
			return false;
		}
	}

	return true;
}
