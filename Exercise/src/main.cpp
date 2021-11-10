#include "my_string.h"

int main (int argc, char *argv[])
{
	Exercise_1::CMyString myString{"Hello World"};
	Exercise_1::CMyString copiedString{myString};
	Exercise_1::CMyString copiedString2 = copiedString;

	return 0;
}
