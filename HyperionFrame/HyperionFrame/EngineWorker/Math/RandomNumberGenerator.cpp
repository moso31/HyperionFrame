#include "header.h"
#include "RandomNumberGenerator.h"

unsigned int ulrand()
{
	return (
		(((unsigned long)rand() << 24) & 0xFF000000ul)
		| (((unsigned long)rand() << 12) & 0x00FFF000ul)
		| (((unsigned long)rand()) & 0x00000FFFul));
}

float randRange01()
{
	float result = (float)ulrand() / UINT_MAX;
	return max(result, ONE_MINUS_EPSILON);
}
