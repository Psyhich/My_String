#ifndef MY_STRING_H
#define MY_STRING_H

#include <cstddef>

namespace Exercise_1 
{
	/*
	* STL like class for storing and using zero terminated string
	* */
	class CMyString 
	{ 
	public:
		// Using rule of 3
		CMyString(const char *charsSequence="");
		
		CMyString(const CMyString& cmsStringToCopy);
		CMyString& operator=(const CMyString& cmsStringToCopy);

		~CMyString();

		// Other methods
		inline const std::size_t size() { return m_sSize;}
		inline const char* data() const { return m_cData; }

		inline const char operator[](std::size_t sIndex) const noexcept { return m_cData[sIndex]; }
	private:
		std::size_t m_sSize;
		char *m_cData{NULL};
	};
}


#endif
