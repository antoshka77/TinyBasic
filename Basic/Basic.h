#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>

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
	GOTO_ERROR
}Errors;

typedef struct Str
{
	int number;
	State statement;
	string expression;
}Str;

class Code
{
private:
	vector <Str> line;
	map <string, int> vars;
	map <string, int> varsInit;
	void InputInterpretation(string expression);
	void PrintInterpretation(string expression);
	void GoToInterpretation(string expression);
	void IsAlphabet(char* str);
	Errors error = SUCCESS;
	int numOfCurLine;
public:
	void GetCode();
	void PrintCode();
	void Interpretation();
};