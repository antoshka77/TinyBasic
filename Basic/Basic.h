#pragma warning (disable: 4996)
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <cstdlib>

using namespace std;

typedef enum State 
{
	DEFAULT,
	PRINT,
	INPUT,
	IF,
	GOTO,
	GOSUB,
	RETURN,
	END
}State;

typedef enum Errors
{
	SUCCESS,
	NOT_FOUND_END_OF_STRING,
	INVALID_NAME_OF_VARIABLE, 
	SYNTAX_ERROR,
	UNDEFINED_VARIABLE,
	GOTO_ERROR,
	GOSUB_ERROR,
	RETURN_ERROR
}Errors;

typedef struct Error
{
	Errors code;
	int numOfStr;
}Error;

typedef struct Str
{
	int number;
	State statement;
	string expression;
}Str;

typedef enum enum_t
{
	number,
	func,
	var
}enum_t;

typedef struct union_t
{
	int number;
	char func;
	string var;
}union_t;

typedef struct polish_t
{
	enum_t type;
	union_t result;
}polish_t;


class Code
{
private:
	vector <Str> line;
	map <string, int> vars;
	map <string, int> varsInit;
	void InputInterpretation(string expression);
	void PrintInterpretation(string expression);
	void GoToInterpretation(string expression);
	void GoSubInterpretation(string expression);
	int IfInterpretation(string expression);
	void RerurnInterpretation();
	void DefaultInterpetation(string expression);
	void PrintErrors();
	void IsAlphabet(char* str);
	vector <Error> err;
	Errors error;
	Error el;
	int numOfCurLine;
	int numOfGoSub;

	vector <polish_t> poli;
	polish_t curElem;
	string stack;
	vector <int> stack_p;
	int IsItNum(char symb);
	int IsItSymb(char symb);
	int IsItFunc(char symb);
	int atoi(string str);
	int IsAlphabet(string str);
	int IsNumber(string str);
public:
	void GetCode();
	void PrintCode();
	void Interpretation();
	void ReversePolishEntry(string mystring);
	int EvaluatePolish();
	~Code() {};
};
