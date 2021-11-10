#include "my_string.h"
#include <cstddef>

std::size_t Exercise_1::CMyString::GetStringLength(const char *cpszStringToCount) noexcept {
	std::size_t nStringLength = 0;
	char cCurrentChar = cpszStringToCount[0];
	while(cCurrentChar != '\0')
	{
		++nStringLength;
		cCurrentChar = cpszStringToCount[nStringLength];
	}
	// Adding 1 because string is zero terminated
	return nStringLength + 1;
}

void Exercise_1::CMyString::CopyString(const char *cpszStringToCopy, char *pszStringToPaste, std::size_t nCountOfChars)
{
	for(std::size_t nIndex = 0; nIndex < nCountOfChars; nIndex++)
	{
		pszStringToPaste[nIndex] = cpszStringToCopy[nIndex];
	}
}

Exercise_1::CMyString::CMyString(const char* cszCharsSequence) 
{
	m_nSize = GetStringLength(cszCharsSequence);
	m_szData = new char[m_nSize];

	CopyString(cszCharsSequence, m_szData, m_nSize);
}

Exercise_1::CMyString::CMyString(const Exercise_1::CMyString& cStringToCopy)
{
	// Cleaning data we used before
	bool shouldCleanup = m_szData != nullptr && m_nSize != cStringToCopy.m_nSize;
	if(shouldCleanup)
	{
		delete[] m_szData;
	}
	// Doing check if we need to reinitialize the char array
	if(m_szData == nullptr || shouldCleanup)
	{
		m_nSize = cStringToCopy.m_nSize;
		m_szData = new char[m_nSize];
	}
	
	CopyString(cStringToCopy.data(), m_szData, m_nSize);
}

Exercise_1::CMyString& Exercise_1::CMyString::operator=(const Exercise_1::CMyString& cStringToCopy)
{
	if(&cStringToCopy == this) 
	{
		return *this;
	}
	// Cleaning data we used before
	bool shouldCleanup = m_szData != nullptr && m_nSize != cStringToCopy.m_nSize;
	if(shouldCleanup)
	{
		delete[] m_szData;
	}
	// Doing check if we need to reinitialize the char array
	if(m_szData == nullptr || shouldCleanup)
	{
		m_nSize = cStringToCopy.m_nSize;
		m_szData = new char[m_nSize];
	}
	
	CopyString(cStringToCopy.data(), m_szData, m_nSize);

	return *this;
}

// Destructor
Exercise_1::CMyString::~CMyString()
{
	if(m_szData != nullptr){
		delete[] m_szData;
	}
}
