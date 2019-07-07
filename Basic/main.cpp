#include "Basic.h"

int main()
{
	setlocale(LC_ALL, "rus");
	Code code;
	code.GetCode();
	code.PrintCode();
	code.Interpretation();
	return 0;
}