
// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// ImgPrcsTest.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"


long double fmaxl(long double a,long double b )
{
	return (long double) a>b ? a:b;
}
double fmax(double a,double b )
{
	return (double) a>b ? a:b;
}

double fminl(double a,double b )
{
	return (double) a<b ? a:b;
}

int isnan(double nan)
{
	return nan == NAN;
}
