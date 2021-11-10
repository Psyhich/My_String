#include "my_string.h"

int main (int argc, char *argv[])
{
	Exercise_1::CMyString cmsMyString{"Hello World"};
	Exercise_1::CMyString cmsCopiedString{cmsMyString};
	Exercise_1::CMyString cmsCopiedString2 = cmsCopiedString;

	return 0;
}
