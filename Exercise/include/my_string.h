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
		CMyString(const char *cszCharsSequence="");
		
		CMyString(const CMyString& cStringToCopy);
		CMyString& operator=(const CMyString& cStringToCopy);

		~CMyString();

		inline std::size_t size() const noexcept { return m_nSize;}
		inline const char* data() const noexcept { return m_szData; }
		void Insert(const char* cszStringToInsert, size_t nStartPosition, size_t nTimesToInsert=1);

		CMyString& operator=(const char* cpcCharsSequence);
		CMyString operator+(const CMyString& cStringToAdd) const;
		bool operator==(const CMyString& cStringToCompare) const;

		inline char operator[](std::size_t sIndex) const { return m_szData[sIndex]; }
		inline char& operator[](std::size_t sIndex) { return m_szData[sIndex]; }
	private:
		// In my task to write everything by myself, so I dont use C's string.h 
		static std::size_t GetStringLength(const char *cszStringToCount) noexcept;
		void CopyString(const char *cszStringToCopy);
		void AppendToString(const char *cszStringToAppend);
		void ReinitializeAndCopy(const char* cpszStringToCopy, const std::size_t& nStringLength);
		static char* TryToAllocate(const size_t nLength) noexcept;
	private:
		std::size_t m_nSize{0};
		char *m_szData{nullptr};
	};
}


#endif // MY_STRING_G
