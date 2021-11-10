#include "my_string.h"

Exercise_1::CMyString::CMyString(const char* charsSequence) 
{
	size_t sCurrentIndex = 0;
	char cCurrentChar = charsSequence[sCurrentIndex];
	while(cCurrentChar != '\0')
	{
		sCurrentIndex++;
		cCurrentChar = charsSequence[sCurrentIndex];
	}
	m_sSize = sCurrentIndex + 1;
	m_cData = new char[m_sSize];

	
	for(sCurrentIndex = 0; sCurrentIndex < m_sSize; sCurrentIndex++)
	{
		m_cData[sCurrentIndex] = charsSequence[sCurrentIndex];
	}
}

// Copy operators
Exercise_1::CMyString::CMyString(const Exercise_1::CMyString& cmsStringToCopy)
{
	// Cleaning data we used before
	if(m_cData != NULL)
	{
		delete[] m_cData;
	}

	m_sSize = cmsStringToCopy.m_sSize;
	m_cData = new char[m_sSize];
	
	for(size_t sCurrentIndex = 0; sCurrentIndex < m_sSize; sCurrentIndex++)
	{
		m_cData[sCurrentIndex] = cmsStringToCopy.m_cData[sCurrentIndex];
	}
}

Exercise_1::CMyString& Exercise_1::CMyString::operator=(const Exercise_1::CMyString& cmsStringToCopy)
{
	if(&cmsStringToCopy == this) 
	{
		return *this;
	}
	// Cleaning data we used before
	if(m_cData != NULL)
	{
		delete[] m_cData;
	}

	m_sSize = cmsStringToCopy.m_sSize;
	m_cData = new char[m_sSize];
	
	for(size_t sCurrentIndex = 0; sCurrentIndex < m_sSize; sCurrentIndex++)
	{
		m_cData[sCurrentIndex] = cmsStringToCopy.m_cData[sCurrentIndex];
	}

	return *this;
}

// Destructor
Exercise_1::CMyString::~CMyString()
{
	delete[] m_cData;
}
