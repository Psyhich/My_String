#include <gtest/gtest.h>
#include <cstring>

#include "my_string.h"

#define TEST_DATA_LENGTH 17
#define SECOND_TEST_DATA_LENGTH 19

class CMyStringBaseFunctionalityFixture : public ::testing::Test
{
protected:
	MyStructs::CMyString myString;
	MyStructs::CMyString secondString;

	const size_t nTestDataLength{TEST_DATA_LENGTH};
	const char szTestData[TEST_DATA_LENGTH]{"Some test string"};

	const size_t nSecondStringLength{SECOND_TEST_DATA_LENGTH};
	const char szSecondTestString[SECOND_TEST_DATA_LENGTH]{"Second test string"};

public:
	void SetUp() override
	{
		myString = MyStructs::CMyString(szTestData);
		secondString = MyStructs::CMyString(szSecondTestString);
	}
};

// -----------------------------
// |  Base functionality tests |
// -----------------------------

TEST_F(CMyStringBaseFunctionalityFixture, DataCorrectnesTest) 
{
	ASSERT_EQ(nTestDataLength, myString.size());
	ASSERT_STREQ(szTestData, myString.data());
	ASSERT_EQ(szTestData[nTestDataLength - 1], '\0');
}

TEST_F(CMyStringBaseFunctionalityFixture, DataCopyTest)
{
	MyStructs::CMyString copiedString{myString};
	ASSERT_EQ(myString.size(), copiedString.size());
	ASSERT_STREQ(myString.data(), copiedString.data());

	MyStructs::CMyString operatorCopiedString = myString;
	ASSERT_EQ(myString.size(), operatorCopiedString.size());
	ASSERT_STREQ(myString.data(), operatorCopiedString.data());

	MyStructs::CMyString operatorCopiedChars = szTestData;
	ASSERT_EQ(myString.size(), operatorCopiedChars.size());
	ASSERT_STREQ(myString.data(), operatorCopiedChars.data());
}

TEST_F(CMyStringBaseFunctionalityFixture, AdditionOperatorTest)
{
	MyStructs::CMyString concatenatedString = myString + secondString; 
	ASSERT_EQ(concatenatedString.size(), myString.size() + secondString.size() - 1);

	char* pszConcatenatedChars = new char[nTestDataLength + nSecondStringLength - 1];
	pszConcatenatedChars[0] = '\0';
	std::strcat(pszConcatenatedChars, szTestData);
	std::strcat(pszConcatenatedChars, szSecondTestString);

	ASSERT_STREQ(concatenatedString.data(), pszConcatenatedChars);
	delete[] pszConcatenatedChars;
}

TEST_F(CMyStringBaseFunctionalityFixture, InsertionTest)
{
	size_t nNewSize = myString.size() + secondString.size() - 1;
	myString.Insert(secondString, 3);

	ASSERT_EQ(nNewSize, myString.size());

	char *szRightInserted = new char[nNewSize];
	std::strncpy(szRightInserted, szTestData, 3);
	szRightInserted[3] = '\0';
	std::strcat(szRightInserted, secondString.data());
	std::strcat(szRightInserted, szTestData + 3);

	ASSERT_STREQ(myString.data(), szRightInserted);

	delete[] szRightInserted;
}

TEST_F(CMyStringBaseFunctionalityFixture, BeginInsertionTest)
{
	MyStructs::CMyString copyString = secondString + myString;
	myString.Insert(secondString, 0);

	ASSERT_EQ(copyString.size(), myString.size());

	ASSERT_STREQ(myString.data(), copyString.data());
}

TEST_F(CMyStringBaseFunctionalityFixture, EndInsertionTest)
{
	// Testing this with append function because it's already tested
	MyStructs::CMyString copyString = myString + secondString;
	
	myString.Insert(secondString, myString.size() - 1);

	ASSERT_EQ(copyString.size(), myString.size());

	ASSERT_STREQ(myString.data(), copyString.data());
}

TEST_F(CMyStringBaseFunctionalityFixture, PartialInsertionTest)
{
	MyStructs::CMyString str = myString;
	constexpr const int ciInsertIndex = 2;
	str.Insert(secondString, ciInsertIndex, 4);

	ASSERT_EQ(str.size(), myString.size() + 4);

	char *szRightInserted = new char[str.size()]; // We can use size here because we checked it
	std::strncpy(szRightInserted, szTestData, ciInsertIndex);
	szRightInserted[ciInsertIndex] = '\0';
	std::strncat(szRightInserted, secondString.data(), 4);
	std::strcat(szRightInserted, szTestData + ciInsertIndex);

	ASSERT_STREQ(str.data(), szRightInserted);

	delete[] szRightInserted;

}

TEST_F(CMyStringBaseFunctionalityFixture, NullptrConstructor)
{
	MyStructs::CMyString nullptrStr{nullptr};

	ASSERT_EQ(nullptrStr.size(), 0);
	ASSERT_EQ(nullptrStr.data(), nullptr);
}

TEST_F(CMyStringBaseFunctionalityFixture, NullptrAssignOperator)
{
	MyStructs::CMyString nullptrStr = nullptr;

	ASSERT_EQ(nullptrStr.size(), 0);
	ASSERT_EQ(nullptrStr.data(), nullptr);
}

TEST_F(CMyStringBaseFunctionalityFixture, NullptrAppends)
{
	MyStructs::CMyString basicString{"Hello"};
	MyStructs::CMyString safeCopy{"Hello"};
	MyStructs::CMyString nullptrStr = nullptr;

	basicString = basicString + nullptrStr;

	ASSERT_EQ(basicString.size(), safeCopy.size());
	ASSERT_STREQ(basicString.data(), safeCopy.data());

	basicString = nullptr;

	basicString = basicString + nullptrStr;
	ASSERT_EQ(basicString.size(), 0);
	ASSERT_EQ(basicString.data(), nullptr);
}

TEST_F(CMyStringBaseFunctionalityFixture, NullptrInsert)
{
	MyStructs::CMyString basicString{"Hello world"};
	MyStructs::CMyString stringCopy{basicString};
	const size_t nSizeBefore = basicString.size();
	basicString.Insert(nullptr, 1);

	ASSERT_EQ(basicString.size(), nSizeBefore);
	ASSERT_STREQ(basicString.data(), stringCopy.data());
}

TEST_F(CMyStringBaseFunctionalityFixture, WrongInsertionTest)
{
	MyStructs::CMyString basicString{"Hello world"};
	MyStructs::CMyString stringCopy{basicString};
	const size_t nSizeBefore = basicString.size();
	basicString.Insert("Some string", 100);

	ASSERT_EQ(basicString.size(), nSizeBefore);
	ASSERT_STREQ(basicString.data(), stringCopy.data());

	basicString.Insert("Some other string", 1, 100);

	ASSERT_EQ(basicString.size(), nSizeBefore);
	ASSERT_STREQ(basicString.data(), stringCopy.data());
}

// -----------------------------
// |		Equality tests	   |
// -----------------------------

TEST(CMyStringEqualityTest, SuccessEqualityTest)
{
	MyStructs::CMyString str1 = "Hello world";
	MyStructs::CMyString str2 = "Hello world";
	
	ASSERT_EQ(str1 == str2, true);

	str2[0] = 'a';
	ASSERT_EQ(str1 == str2, false);

	str2 = str2 + "Hi";

	ASSERT_EQ(str1 == str2, false);

	MyStructs::CMyString nullPtrString = nullptr;
	ASSERT_EQ(nullPtrString == str1, false);
	ASSERT_EQ(nullPtrString == nullPtrString, true);

	str1 = "Same other1";
	ASSERT_EQ(str1 == str2, false);
}

// -----------------------------
// |		Deletion tests	   |
// -----------------------------

TEST(CMyStringDeletionTest, SuccessDeletionTest)
{
	//"Hello, world"
	const char *cszFirstIteration = "He world";
	const char *cszSecondIteration = "world";
	MyStructs::CMyString strToDeleteFrom{"Hello, world"};

	//"He world"
	strToDeleteFrom.Delete(2, 4);
	ASSERT_STREQ(strToDeleteFrom.data(), cszFirstIteration);

	//"world"
	strToDeleteFrom.Delete(0, 3);
	ASSERT_STREQ(strToDeleteFrom.data(), cszSecondIteration);

	//nullptr
	strToDeleteFrom.Delete(0, strToDeleteFrom.size() - 1);
	ASSERT_EQ(strToDeleteFrom.data(), nullptr);
}

TEST(CMyStringDeletionTest, ExhaustiveDeletionTest)
{
	// Doing exhaustive trim
	MyStructs::CMyString mainString = "aaaaaaaaaaaaaaaaaaaaaaaa";
	const size_t cnMaxCount = mainString.size();
	size_t nCurrentCount = 0;
	while(nCurrentCount <= cnMaxCount)
	{
		mainString.Delete(0, 1);
		++nCurrentCount;
		if(mainString.size() == 0)
		{
			break;
		}
	}
	ASSERT_EQ(nCurrentCount, cnMaxCount - 1);
}

TEST(CMyStringDeletionTest, FailDeletionTest)
{
	const char* szStartingString = "Hello";
	MyStructs::CMyString startingString{szStartingString};

	startingString.Delete(10, 10);
	ASSERT_STREQ(startingString.data(), szStartingString);

	startingString.Delete(0, 10);
	ASSERT_STREQ(startingString.data(), szStartingString);

	// Trying to delete null terminant
	startingString.Delete(startingString.size() - 1, 1);
	ASSERT_STREQ(startingString.data(), szStartingString);
}

// -----------------------------
// |		Substring tests	   |
// -----------------------------

TEST(CMyStringSubstringTest, SuccessSubstringTest)
{
	const char* cszBaseString = "Hello world";
	const char* cszSubstring = "llo wor";

	MyStructs::CMyString myString{cszBaseString};
	MyStructs::CMyString substring = myString.Substring(2, 7);

	ASSERT_STREQ(substring.data(), cszSubstring);
}

TEST(CMyStringSubstringTest, FailSubstringTest)
{
	const char* cszBaseString = "Hello world";

	MyStructs::CMyString myString{cszBaseString};

	MyStructs::CMyString substring = myString.Substring(2, 0);
	ASSERT_EQ(substring.size(), 0);
	ASSERT_EQ(substring.data(), nullptr);

	substring = myString.Substring(100, 0);
	ASSERT_EQ(substring.size(), 0);
	ASSERT_EQ(substring.data(), nullptr);

	substring = myString.Substring(0, 100);
	ASSERT_EQ(substring.size(), 0);
	ASSERT_EQ(substring.data(), nullptr);

	substring = myString.Substring(100, 100);
	ASSERT_EQ(substring.size(), 0);
	ASSERT_EQ(substring.data(), nullptr);
}

// -----------------------------
// |		Find tests		   |
// -----------------------------

TEST(CMyStringFindTest, SuccessFindTest)
{
	const char *cszFirstSubstring = "Hello";
	const char *cszSecondSubstring = "string";
	const char *cszThirdSubstring = "string!";

	const MyStructs::CMyString mainString{"Hello, this is a test string for a test, Hi this is a test string!"};
	// Calculating position of found string

	// Using pointer arithmetics to calculate found position
	size_t cnFoundPos = (strstr(mainString.data(), cszFirstSubstring) - 
		mainString.data()) / sizeof(char);
	auto position = mainString.Find(cszFirstSubstring);
	ASSERT_EQ(*position, cnFoundPos);

	cnFoundPos = (strstr(mainString.data(), cszSecondSubstring) - 
		mainString.data()) / sizeof(char);
	position = mainString.Find(cszSecondSubstring);
	ASSERT_EQ(*position, cnFoundPos);

	cnFoundPos = (strstr(mainString.data(), cszThirdSubstring) - 
		mainString.data()) / sizeof(char);
	position = mainString.Find(cszThirdSubstring);
	ASSERT_EQ(*position, cnFoundPos);

	const MyStructs::CMyString copyString{cszThirdSubstring};
	position = mainString.Find(copyString);
	ASSERT_EQ(*position, cnFoundPos);
}

TEST(CMyStringFindTest, FailFindTest)
{
	const char *nullString = "";

	const MyStructs::CMyString stringToSearch{"Hi hi hi hi hi hi hi"};

	auto nFoundPos = stringToSearch.Find(nullString);
	ASSERT_EQ(nFoundPos, std::nullopt);

	nFoundPos = stringToSearch.Find(nullptr);
	ASSERT_EQ(nFoundPos, std::nullopt);
}

// -----------------------------
// |		Trim tests		   |
// -----------------------------

TEST(CMyStringTrimTest, SuccessTrimTest)
{
	// Adding 1 to size because strlen excludes '\0'
	const char *cszFirstResult = " test, bbbbb other test, cccccc lul";
	const size_t cnFirstSize = strlen(cszFirstResult) + 1;
	const char *cszSecondResult = " other test, cccccc lul";
	const size_t cnSecondSize = strlen(cszSecondResult) + 1;
	const char *cszThirdResult = " lul";
	const size_t cnThirdSize = strlen(cszThirdResult) + 1;

	MyStructs::CMyString mainString{"aaaaa test, bbbbb other test, cccccc lul"};

	MyStructs::CMyString trimmed = mainString.Trim(0, mainString.size(), 'a');
	ASSERT_EQ(trimmed.size(), cnFirstSize);
	ASSERT_STREQ(trimmed.data(), cszFirstResult);

	trimmed = trimmed.Trim(7, 28, 'b');
	ASSERT_EQ(trimmed.size(), cnSecondSize);
	ASSERT_STREQ(trimmed.data(), cszSecondResult);

	trimmed = trimmed.Trim(13, 10, 'c');
	ASSERT_EQ(trimmed.size(), cnThirdSize);
	ASSERT_STREQ(trimmed.data(), cszThirdResult);
}

TEST(CMyStringTrimTest, ExhaustiveTrimTest)
{
	// Doing exhaustive trim
	MyStructs::CMyString mainString = "aaaaaaaaaaaaaaaaaaaaaaaa";
	auto trimmed = mainString.Trim(0, mainString.size(), 'a');
	const size_t cnNewSize = 0;

	ASSERT_EQ(trimmed.size(), cnNewSize);
	ASSERT_STREQ(trimmed.data(), nullptr);
}

TEST(CMyStringTrimTest, FailTrimTests)
{
	const char *cszMainString = "My main test string that would be tested by tests";
	MyStructs::CMyString mainString{cszMainString};

	auto trimmed = mainString.Trim(0, 0);
	ASSERT_STREQ(trimmed.data(), nullptr);

	trimmed = mainString.Trim(1000, 1);
	ASSERT_STREQ(trimmed.data(), nullptr);

	trimmed = mainString.Trim(1, 1000);
	ASSERT_STREQ(trimmed.data(), nullptr);
}

// -----------------------------
// |	ToUpperCase tests	   |
// -----------------------------

TEST(CMyStringToUpperCaseTest, SuccessToUpperCaseTest)
{
	const char *cszStartVariant = "hia world"; 
	const char *cszRightResult = "HIA WORLD";
	const char *cszSecondRightResult = "HIa world";
	const char *cszThirdRightResult = "HIa woRLd";

	MyStructs::CMyString mainString{cszStartVariant};

	mainString.ToUpperCase();
	ASSERT_STREQ(mainString.data(), cszRightResult);

	mainString = cszStartVariant;
	mainString.ToUpperCase(0, 2);
	ASSERT_STREQ(mainString.data(), cszSecondRightResult);

	mainString.ToUpperCase(6, 8);
	ASSERT_STREQ(mainString.data(), cszThirdRightResult);
}

TEST(CMyStringToUpperCaseTest, FailUpperCaseTest)
{
	const char *cszMainString = "Some trial string";
	MyStructs::CMyString mainString{cszMainString};

	mainString.ToUpperCase(40, 1);
	ASSERT_STREQ(mainString.data(), cszMainString);

	mainString.ToUpperCase(0, 100);
	ASSERT_STREQ(mainString.data(), cszMainString);

	mainString.ToUpperCase(0, 0);
	ASSERT_STREQ(mainString.data(), cszMainString);
}

// -----------------------------
// |	ToLowerCase tests	   |
// -----------------------------

TEST(CMyStringToLowerCaseTest, SuccessToLowerCaseTest)
{
	const char *cszStartVariant = "HIA WORLD"; 
	const char *cszRightResult = "hia world";
	const char *cszSecondRightResult = "hiA WORLD";
	const char *cszThirdRightResult = "hiA WOrlD";

	MyStructs::CMyString mainString{cszStartVariant};

	mainString.ToLowerCase();
	ASSERT_STREQ(mainString.data(), cszRightResult);

	mainString = cszStartVariant;
	mainString.ToLowerCase(0, 2);
	ASSERT_STREQ(mainString.data(), cszSecondRightResult);

	mainString.ToLowerCase(6, 8);
	ASSERT_STREQ(mainString.data(), cszThirdRightResult);

}

TEST(CMyStringToLowerCaseTest, FailLowerCaseTest)
{
	const char *cszMainString = "Some trial string";
	MyStructs::CMyString mainString{cszMainString};

	mainString.ToLowerCase(40, 1);
	ASSERT_STREQ(mainString.data(), cszMainString);

	mainString.ToLowerCase(0, 100);
	ASSERT_STREQ(mainString.data(), cszMainString);

	mainString.ToLowerCase(0, 0);
	ASSERT_STREQ(mainString.data(), cszMainString);
}

// -----------------------------
// |		ToInt tests		   |
// -----------------------------

TEST(CMyStringToIntTest, SuccessToIntTest)
{
	MyStructs::CMyString numberString{"100"};
	std::optional<int> parsedValue{atoi(numberString.data())};
	ASSERT_EQ(parsedValue, numberString.ToInt());
	
	numberString = "2394";
	parsedValue = 2394;
	ASSERT_EQ(parsedValue, numberString.ToInt());
}

TEST(CMyStringToIntTest, FailToIntTest)
{
	MyStructs::CMyString numberString = "This is not int";
	std::optional<int> parsedValue = std::nullopt;

	ASSERT_EQ(parsedValue, numberString.ToInt());

	numberString = "22.8";
	ASSERT_EQ(parsedValue, numberString.ToInt());

	numberString = "";
	ASSERT_EQ(parsedValue, numberString.ToInt());

	numberString = nullptr;
	ASSERT_EQ(parsedValue, numberString.ToInt());
}

// -----------------------------
// |		ToDouble tests	   |
// -----------------------------

TEST(CMyStringToDoubleTest, SuccessToDoubleTest)
{
	MyStructs::CMyString myNewDoubleString{"1.0987"};
	double dRealValue = 1.0987;
	ASSERT_DOUBLE_EQ(*myNewDoubleString.ToDouble(), dRealValue);

	myNewDoubleString = "987";
	dRealValue = 987;
	ASSERT_DOUBLE_EQ(*myNewDoubleString.ToDouble(), dRealValue);

	myNewDoubleString = "1.000001";
	dRealValue = 1.000001;
	ASSERT_DOUBLE_EQ(*myNewDoubleString.ToDouble(), dRealValue);

	myNewDoubleString = "901283.6666";
	dRealValue = 901283.6666;
	ASSERT_DOUBLE_EQ(*myNewDoubleString.ToDouble(), dRealValue);

	myNewDoubleString = ".6666";
	dRealValue = 0.6666;
	ASSERT_DOUBLE_EQ(*myNewDoubleString.ToDouble(), dRealValue);

	myNewDoubleString = "-45.78";
	dRealValue = -45.78;
	ASSERT_DOUBLE_EQ(*myNewDoubleString.ToDouble(), dRealValue);
}

TEST(CMyStringToDoubleTest, FailToDoubleTest)
{
	MyStructs::CMyString myNewDoubleString = "This is fully not a number";
	ASSERT_EQ(myNewDoubleString.ToDouble(), std::nullopt);

	myNewDoubleString = "0.-45";
	ASSERT_EQ(myNewDoubleString.ToDouble(), std::nullopt);

	myNewDoubleString = "0..45";
	ASSERT_EQ(myNewDoubleString.ToDouble(), std::nullopt);

	myNewDoubleString = "0.45.";
	ASSERT_EQ(myNewDoubleString.ToDouble(), std::nullopt);
}

