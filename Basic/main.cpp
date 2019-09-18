#include "Basic.h"

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");
	Code code;
	code.GetCode(argv[1]);//argv[1]
	code.PrintCode();
	code.Interpretation();
	return 0;
}