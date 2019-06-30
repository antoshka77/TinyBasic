#include "Basic.h"

int main()
{
	setlocale(LC_ALL, "rus");
	Code code;
	cout << "Hello";
	code.GetCode();
	code.PrintCode();
	code.Interpretation();
	return 0;
}