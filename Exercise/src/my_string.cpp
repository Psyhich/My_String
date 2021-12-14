#include <cstdarg>
#include <cstddef>
#include <new>
#include <iostream>
#include <cmath>
#include <limits>
#include <list>

#include "my_string.h"

char MyStructs::CMyString::ToLowerCase(char chCharToTranslate)
{
	constexpr const int ciACharValue = 32;
	if(chCharToTranslate >= 'A' && chCharToTranslate <= 'Z')
	{
		return chCharToTranslate + ciACharValue;
	}
	return chCharToTranslate;
}

char MyStructs::CMyString::ToUpperCase(char chCharToTranslate)
{
	constexpr const int ciACharValue = 32;
	if(chCharToTranslate >= 'a' && chCharToTranslate <= 'z')
	{
		return chCharToTranslate - ciACharValue;
	}
	return chCharToTranslate;
}

bool MyStructs::CMyString::CheckEqualityCaseInsensitive(const char* cszFirstString, const char *cszSecondString, size_t nLengthToCheck)
{
	for(size_t nIndex = 0; nIndex <= nLengthToCheck; nIndex++)
	{
		if(ToLowerCase(cszFirstString[nIndex]) != ToLowerCase(cszSecondString[nIndex]))
		{
			return false;
		}
	}
	return true;
}

bool MyStructs::CMyString::CheckEquality(const char* cszFirstString, const char *cszSecondString, size_t nLengthToCheck)
{
	for(size_t nIndex = 0; nIndex <= nLengthToCheck; nIndex++)
	{
		if(cszFirstString[nIndex] != cszSecondString[nIndex])
		{
			return false;
		}
	}
	return true;
}

MyStructs::CMyString MyStructs::CMyString::ParseFormater(const CMyString &cFormaterToParse, va_list &args)
{
	CMyString newString{nullptr};
	if(cFormaterToParse.size() <= 1)
	{
		return newString;
	}

	const char cchLastChar = cFormaterToParse[cFormaterToParse.size() - 2];
	switch (cchLastChar) {
		case 'd':
		{
			int parsedDecimal = va_arg(args, int);
			newString = CMyString::FromInt(parsedDecimal);
			break;
		}
		case 'f':
		{
			float parsedFloat = va_arg(args, double);
			newString = CMyString::FromDouble(parsedFloat);
			break;
		}
		case 'c':
		{
			char parsedChar = va_arg(args, int);
			newString = parsedChar;
			break;
		}
		case 's':
		{
			newString = va_arg(args, char*);
			break;
		}
		case '%':
		{
			newString = '%';
			break;
		}
	}

	return newString;
}

MyStructs::CMyString MyStructs::CMyString::ParseFormatString(const char *cszStringToFormat, size_t nSize, va_list &args) {

	CMyString formatedString{nullptr};
	if(cszStringToFormat == nullptr || nSize <= 1)
	{
		return formatedString;
	}

	size_t nNewSize = 0;
	bool bIsRedingFormater = false;

	std::list<CMyString> parameters{};
	CMyString readParam{};
	
	// Counting new size
	for(size_t nIndex = 0; nIndex < nSize; nIndex++)
	{
		const char cchCurrentChar = cszStringToFormat[nIndex];
		if(cchCurrentChar == '%' && !bIsRedingFormater)
		{
			bIsRedingFormater = true;
		} else if(bIsRedingFormater)
		{
			readParam = readParam + CMyString(cchCurrentChar);
			if(IsStopCharacter(cchCurrentChar))
			{
				bIsRedingFormater = false;

				CMyString newString = ParseFormater(readParam, args);
				// If we cannot parse it return nothing
				if(newString.data() == nullptr)
				{
					return CMyString(nullptr);
				}
				nNewSize += newString.size() - 1;

				parameters.push_back(newString);
				readParam = "";
			}
		} else {
			nNewSize += 1;
		}
	}

	formatedString.TryToAllocate(nNewSize);
	if(formatedString.data() == nullptr)
	{
		va_end(args);
		return formatedString;
	}

	// Now creating new string with all data inserted
	size_t nFormatedIndex = 0;
	bool bIsSkipping = false;
	for(size_t nUnformatedIndex = 0; nUnformatedIndex < nSize; nUnformatedIndex++)
	{
		const char cchCurrentChar = cszStringToFormat[nUnformatedIndex];
		if(bIsSkipping && IsStopCharacter(cchCurrentChar))
		{
			CMyString paramString = parameters.front();
			for(size_t nParamIndex = 0; nParamIndex < paramString.size() - 1; nParamIndex++)
			{
				formatedString[nFormatedIndex] = paramString[nParamIndex];
				++nFormatedIndex;
			}

			parameters.pop_front();
			bIsSkipping = false;
		} else if(cchCurrentChar == '%')
		{
			bIsSkipping = true;
		} else if(!bIsSkipping)
		{
			formatedString[nFormatedIndex] = cchCurrentChar;
			++nFormatedIndex;
		}
	}

	return formatedString;
}

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


MyStructs::CMyString::CMyString(char chCharToAssign)
{
	TryToAllocate(2);
	if(m_szData != nullptr)
	{
		m_szData[0] = chCharToAssign;
		m_szData[1] = '\0';
	}
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
		printf("Got out of bounds\n");
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
	substring.TryToAllocate(nCharactersCount + !IsReachedTerminator(nPosition, nCharactersCount));
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

	substring.m_szData[substring.size() - 1] = '\0';

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
		m_szData[nIndex] = ToUpperCase(m_szData[nIndex]);
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
		m_szData[nIndex] = ToLowerCase(m_szData[nIndex]);
	}
}

MyStructs::CMyString MyStructs::CMyString::Reverse() const noexcept
{
	if(m_szData == nullptr && m_nSize <= 2)
	{
		return CMyString(nullptr);
	}

	CMyString newString{*this};

	for(size_t nIndex = 0; nIndex < newString.size() / 2 - 1; nIndex++){
		std::swap(newString.m_szData[nIndex], newString.m_szData[newString.size() - nIndex - 2]);
	}

	return newString;
}


bool MyStructs::CMyString::Compare(const CMyString& cStringToCompare, bool bIsCaseSensitive) const noexcept
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
	
	if(bIsCaseSensitive)
	{
		return CheckEquality(data(), cStringToCompare.data(), size() - 1);
	} else 
	{
		return CheckEqualityCaseInsensitive(data(), cStringToCompare.data(), size() - 1);
	}
}


bool MyStructs::CMyString::Compare(const char* cszStringToCompare, bool bIsCaseSensitive) const noexcept
{
	if(m_szData == nullptr && cszStringToCompare == nullptr)
	{
		return true;
	}

	size_t nLength = GetStringLength(cszStringToCompare);
	if(nLength != m_nSize)
	{
		return false;
	}

	if(bIsCaseSensitive)
	{
		return CheckEquality(data(), cszStringToCompare, size() - 1);
	} else 
	{
		return CheckEqualityCaseInsensitive(data(), cszStringToCompare, size() - 1);
	}
}

bool MyStructs::CMyString::Compare(
	const CMyString& cStringToCompare, size_t nStartPos, 
	size_t nLength, bool bIsCaseSensitive) const noexcept
{

	if(IsOutOfBounds(nStartPos, nLength) || cStringToCompare.IsOutOfBounds(0, nLength))
	{
		printf("Given range is invalid!\n");
		return false;
	}
	
	// Subtracting to make it as length value, not the position(because we start from 0)
	if(nStartPos == 0)
	{
		--nLength;
	}

	if(bIsCaseSensitive)
	{
		return CheckEquality(data() + nStartPos, cStringToCompare.data(), nLength);
	} else 
	{
		return CheckEqualityCaseInsensitive(data() + nStartPos, cStringToCompare.data(), nLength);
	}
}
bool MyStructs::CMyString::Compare(
	const char* cszStringToCompare, size_t nStartPos, 
	size_t nLength, bool bIsCaseSensitive) const noexcept
{
	size_t nSecondStringLength = GetStringLength(cszStringToCompare);

	if(IsOutOfBounds(nStartPos, nLength) || nLength > nSecondStringLength)
	{
		printf("Given range is invalid!\n");
		return false;
	}

	// Subtracting to make it as length value, not the position(because we start from 0)
	if(nStartPos == 0)
	{
		--nLength;
	}
	
	if(bIsCaseSensitive)
	{
		return CheckEquality(
			data() + nStartPos, cszStringToCompare, nLength);
	} else 
	{
		return CheckEqualityCaseInsensitive(
			data() + nStartPos, cszStringToCompare, nLength);
	}
}



void MyStructs::CMyString::ReplaceWithString(
	const char* cszStartSequence, const char *cszStringReplace, 
	size_t nReplaceStart, size_t nReplaceLength) noexcept
{
	const size_t cnReplaceEnd = nReplaceStart + nReplaceLength;
	for(size_t nIndex = 0; nIndex < size() - 1; nIndex++)
	{
		if(nIndex < nReplaceStart || nIndex >= cnReplaceEnd)
		{
			m_szData[nIndex] = cszStartSequence[nIndex];
		} else 
		{
			m_szData[nIndex] = cszStringReplace[nIndex - nReplaceStart];
		}
	}

}

MyStructs::CMyString MyStructs::CMyString::Replace(
	const CMyString& cStrToInput, size_t nPos, size_t nLength) const noexcept
{
	CMyString newString{nullptr};
	if(nLength > cStrToInput.size() || nPos >= size())
	{
		printf("Given range is invalid!!\n");
		return newString;
	}

	if(nPos + nLength <= size())
	{
		if(nLength == cStrToInput.size())
		{
			// Minusing one to exclude terminant from cszStrToInput
			--nLength;
		}
		newString.m_nSize = size();
	} else 
	{
		newString.m_nSize = nPos + nLength + (nLength != cStrToInput.size());
	}

	newString.TryToAllocate(newString.m_nSize);
	if(newString.m_szData == nullptr)
	{
		return newString;
	}

	newString.ReplaceWithString(data(), cStrToInput.data(), nPos, nLength);

	newString.m_szData[newString.size() - 1] = '\0';

	return newString;
}

MyStructs::CMyString MyStructs::CMyString::Replace(
	const char* cszStrToInput, size_t nPos, size_t nLength) const noexcept
{
	CMyString newString{nullptr};
	const size_t cnStrLen = GetStringLength(cszStrToInput);

	if(nLength > cnStrLen || nPos >= size())
	{
		return newString;
	}
	
	if(nPos + nLength <= size())
	{
		if(nLength == cnStrLen)
		{
			// Minusing one to exclude terminant from cszStrToInput
			--nLength;
		}
		newString.m_nSize = size();
	} else 
	{
		newString.m_nSize = nPos + nLength + (nLength != cnStrLen);
	}

	newString.TryToAllocate(newString.m_nSize);
	if(newString.m_szData == nullptr)
	{
		return newString;
	}

	newString.ReplaceWithString(data(), cszStrToInput, nPos, nLength);

	newString.m_szData[newString.size() - 1] = '\0';

	return newString;
}

MyStructs::CMyString MyStructs::CMyString::Format(const CMyString& cFormatString...)
{
	std::va_list args;
	va_start(args, &cFormatString);
	
	CMyString formatedString = ParseFormatString(cFormatString.data(), cFormatString.size(), args);
	
	va_end(args);
	return formatedString;
}

MyStructs::CMyString MyStructs::CMyString::Format(const char* cszFormatString...)
{
	std::va_list args;

	va_start(args, cszFormatString);
	
	size_t nSize = GetStringLength(cszFormatString);

	CMyString formatedString = ParseFormatString(cszFormatString, nSize, args);
	
	va_end(args);
	return formatedString;
}

std::optional<int> MyStructs::CMyString::ToInt() const noexcept 
{
	if(data() == nullptr || size() <= 1)
	{
		return std::nullopt;
	}

	const bool cbIsNegative = (m_szData[0] == '-');
	int iConstructedNumber = 0;
	int iCurrentPower = 1;

	for(size_t nIndex = 0; nIndex < size() - 1 - cbIsNegative; nIndex++)
	{
		const int ciCurrentChar = (int)m_szData[size() - 2 - nIndex];
		if(ciCurrentChar >= 48 && ciCurrentChar <= 57)
		{
			iConstructedNumber += iCurrentPower  * (ciCurrentChar - 48);
		} else 
		{
			return std::nullopt;
		}
		iCurrentPower *= 10;
	}

	if(cbIsNegative)
	{
		iConstructedNumber *= -1;
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

	const bool cbIsNegative = iToConvert < 0;

	CMyString newString{nullptr};
	// Taking in mind '\0' and '-' sign if number is negative
	newString.TryToAllocate(nRadixCount + 1 + cbIsNegative);


	iNumber = std::abs(iToConvert);
	for(size_t nCurrentRadix = 0; nCurrentRadix < nRadixCount; nCurrentRadix++)
	{
		newString[nRadixCount - 1 - nCurrentRadix + cbIsNegative] = iNumber % 10 + 48;
		iNumber /= 10;
	}

	if(cbIsNegative)
	{
		newString[0] = '-';
	}
	newString[newString.size() - 1] = '\0';

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

MyStructs::CMyString MyStructs::CMyString::FromDouble(double dToConvert, size_t nPrecision) noexcept
{
	const bool cbIsNegative = dToConvert < 0;

	long long llIntegerPart = std::floor(std::abs(dToConvert));

	const double dFractionPart = std::abs(dToConvert) - llIntegerPart;
	long long llParsedFraction = std::abs(dFractionPart * 
		std::pow(10, std::numeric_limits<double>::digits10));
	
	// Counting radixes
	size_t nIntegerRadixCount = 0;
	long long llOperatedValue = llIntegerPart;
	while(llOperatedValue > 0)
	{
		++nIntegerRadixCount;
		llOperatedValue /= 10;
	}
	size_t nFractionRadixCount = 0;
	llOperatedValue = llParsedFraction;
	while(llOperatedValue > 0)
	{
		++nFractionRadixCount;
		llOperatedValue /= 10;
	}
	if(nPrecision < nFractionRadixCount)
	{
		for(size_t nRadix = 0; nRadix < nFractionRadixCount - nPrecision; nRadix++)
		{
			llParsedFraction /= 10;
		}
		nFractionRadixCount = nPrecision;
	}

	CMyString newString{nullptr};
	// Adding 1 for dot + 1 for \0 and taking in mind negative value
	newString.m_nSize = nIntegerRadixCount + nFractionRadixCount + 2 + cbIsNegative;
	newString.TryToAllocate(newString.m_nSize); 
	if(newString.data() == nullptr)
	{
		printf("Failed to alocate data\n");
		return newString;
	}

	
	for(size_t nIndex = cbIsNegative; nIndex < newString.size() - 1; nIndex++)
	{
		const size_t nRealIndex = newString.size() - nIndex - 1 - !cbIsNegative;
		if(nRealIndex < nIntegerRadixCount + cbIsNegative)
		{
			newString.m_szData[nRealIndex] = (char)(llIntegerPart % 10 + 48);
			llIntegerPart /= 10;
		} else if(nRealIndex > nIntegerRadixCount + cbIsNegative)
		{
			newString.m_szData[nRealIndex] = (char)(llParsedFraction % 10 + 48);
			llParsedFraction /= 10;
		}
	}

	newString.m_szData[nIntegerRadixCount + cbIsNegative] = '.';
	if(cbIsNegative)
	{
		newString.m_szData[0] = '-';
	}
	newString.m_szData[newString.size() - 1] = '\0';

	return newString;
}


MyStructs::CMyString& MyStructs::CMyString::operator=(const char* cpszCharsSequence)
{
	size_t nNewSize = GetStringLength(cpszCharsSequence);
	ReinitializeAndCopy(cpszCharsSequence, nNewSize);
	return *this;
}

MyStructs::CMyString& MyStructs::CMyString::operator=(char chCharToAssign)
{
	*this = CMyString(chCharToAssign);
	return *this;
}

MyStructs::CMyString MyStructs::CMyString::operator+(const CMyString& cStringToAdd) const noexcept
{
	CMyString newString = this->data();
	newString.AppendToString(cStringToAdd.data());

	return newString;
}
