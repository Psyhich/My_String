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

		inline size_t size() const noexcept { return m_nSize; }
		inline const char* data() const noexcept { return m_szData; }

		void Insert(const char* cszStringToInsert, const size_t nStartPosition, const size_t nCountOfCharsToInsert);

		inline void Insert(const char* cszStringToInsert, size_t nStartPosition)
		{
			Insert(cszStringToInsert, nStartPosition, GetStringLength(cszStringToInsert) - 1);
		}
		inline void Insert(const CMyString &stringToInsert, size_t nStartPosition, const size_t nCountOfCharsToInsert)
		{
			Insert(stringToInsert.data(), nStartPosition, nCountOfCharsToInsert);
		}
		inline void Insert(const CMyString &stringToInsert, size_t nStartPosition)
		{
			Insert(stringToInsert.data(), nStartPosition, stringToInsert.size() - 1);
		}

		void Delete(size_t nPosition, size_t nCharactersCount) noexcept;
		CMyString Substring(size_t nPosition, size_t nCharactersCount) const noexcept;

		size_t Find(const char *szStringToFind) const noexcept;
		size_t Find(const CMyString &stringToFind) const noexcept;

		CMyString Trim(size_t nPosition, size_t nCharactersCount) noexcept;

		void ToUpperCase(size_t nStartPos, size_t nLasPos) noexcept;
		void ToUpperCase() noexcept;
		void ToLowerCase(size_t nStartPos, size_t nLasPos) noexcept;
		void ToLowerCase() noexcept;

		CMyString& operator=(const char* cpcCharsSequence);
		CMyString operator+(const CMyString& cStringToAdd) const;
		bool operator==(const CMyString& cStringToCompare) const;

		inline char operator[](std::size_t sIndex) const { return m_szData[sIndex]; }
		inline char& operator[](std::size_t sIndex) { return m_szData[sIndex]; }
	private:
		// In my task I should write everything by myself, so I dont use C's string.h 
		static size_t GetStringLength(const char *cszStringToCount) noexcept;
		void CopyString(const char *cszStringToCopy);
		void AppendToString(const char *cszStringToAppend);
		void ReinitializeAndCopy(const char* cpszStringToCopy, const size_t& nStringLength);
		static char* TryToAllocate(const size_t nLength) noexcept;
	private:
		size_t m_nSize{0};
		char *m_szData{nullptr};
	};
}


#endif // MY_STRING_G
