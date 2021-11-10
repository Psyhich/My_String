#include <gtest/gtest.h>

#include "my_string.h"
class CMyStringFixture : public ::testing::Test
{
protected:
	Exercise_1::CMyString cmsMyString;
	const size_t sTestDataLength{17};
	const char cTestData[17]{"Some test string"};

public:
	void SetUp() override
	{
		cmsMyString = Exercise_1::CMyString(cTestData);
	}
};

TEST_F(CMyStringFixture, DataCorrectnesTest) 
{

	ASSERT_EQ(sTestDataLength, cmsMyString.size());

	for(std::size_t sCurrentIndex = 0; sCurrentIndex < cmsMyString.size(); sCurrentIndex++)
	{
		ASSERT_EQ(cTestData[sCurrentIndex], cmsMyString[sCurrentIndex]);
	}
	ASSERT_EQ(cTestData[sTestDataLength - 1], '\0');
}

TEST_F(CMyStringFixture, DataCopyTest)
{
	Exercise_1::CMyString cmsCopiedString{cmsMyString};

	ASSERT_EQ(cmsMyString.size(), cmsCopiedString.size());

	for(std::size_t sCurrentIndex = 0; sCurrentIndex < cmsMyString.size(); sCurrentIndex++){
		ASSERT_EQ(cmsMyString[sCurrentIndex], cmsCopiedString[sCurrentIndex]);
	}

	Exercise_1::CMyString cmsOperatorCopiedString{cmsMyString};

	ASSERT_EQ(cmsMyString.size(), cmsOperatorCopiedString.size());

	for(std::size_t sCurrentIndex = 0; sCurrentIndex < cmsMyString.size(); sCurrentIndex++){
		ASSERT_EQ(cmsMyString[sCurrentIndex], cmsOperatorCopiedString[sCurrentIndex]);
	}
}

