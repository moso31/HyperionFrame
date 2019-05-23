#include "header.h"
#include "RandomNumberGenerator.h"

HUInt ulrand()
{
	return (
		(((HULong)rand() << 24) & 0xFF000000ul)
		| (((HULong)rand() << 12) & 0x00FFF000ul)
		| (((HULong)rand()) & 0x00000FFFul));
}

HFloat randRange01()
{
	HFloat result = (HFloat)ulrand() / UINT_MAX;
	return max(result, H_ONE_MINUS_EPSILON);
}
