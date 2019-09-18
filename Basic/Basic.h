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
	LET,
	END
}State;//состояния

typedef enum Errors
{
	SUCCESS,
	NOT_FOUND_END_OF_STRING,
	INVALID_NAME_OF_VARIABLE, 
	SYNTAX_ERROR,
	UNDEFINED_VARIABLE,
	GOTO_ERROR,
	GOSUB_ERROR,
	RETURN_ERROR,
	ERROR_OF_EVALUATE,
	ERROR_OF_EXPRESSION
}Errors;//типы ошибок

typedef struct Error
{
	Errors code;//код ошибки
	int numOfStr;//номер строки с ошибкой
}Error;

typedef struct Str
{
	int number;//хранит номер строки
	State statement;//хранит команду
	string expression;//хранит обрабатываемое выражение
}Str;//тип строки в бейсик

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
}union_t;//хранит данные о встречаемом типе объекта в математической строке

typedef struct polish_t
{
	enum_t type;//тип
	union_t result;//резлуьтат
}polish_t;//элемент польской записи


class Code
{
private:
	map <int, Str> line;//хранит строки программы
	map <string, int> vars;//хранит имена переменных, которые встречаются в программе
	map <string, int> varsInit;//хранит данные об инициализации
	void InputInterpretation(string expression);//обрабатывает input
	void PrintInterpretation(string expression);//обратывает print
	void GoToInterpretation(string expression);//обрабатывает goto
	void GoSubInterpretation(string expression);//обрабатывает gosub
	int IfInterpretation(string expression);//обрабатывает if
	void RerurnInterpretation();//обрабатывает return
	void DefaultInterpetation(string expression);//обрабатывает математические выражения
	void PrintErrors();//печать ошибок
	void IsAlphabet(char* str);//вспомогательная функция для проверки алфавита
	vector <Error> err;//хранит все встречающиеся обрабатываемые ошибки
	Errors error = SUCCESS;
	Error el;//
	vector <int> numOfLines;//хранит все номера строк
	int numOfCurLine;//номер текущей строки
	int numOfGoSub;//номер строки с gosub
	int flagOfLet = 1;
//работа с польской записью:
	vector <polish_t> poli;//хранит элементы польской записи
	polish_t curElem;//вспомогателньая переменная
	string stack;//вспомогательный стек
	vector <int> stack_p;//стек польской записи
	int IsItNum(char symb);//проверка на цифру
	int IsItSymb(char symb);//проверка на букву
	int IsItFunc(char symb);//проверка на функцию
	int atoi(string str);//перевд строки в число
	int IsAlphabet(string str);//проверка на допутимые символы
	int IsNumber(string str);//проверка на число
	int flagOfEvaluate;
public:
	void GetCode(char* indata);//получить код из файла
	void PrintCode();//печать кода в консоль
	void Interpretation();//основная функция выполнения кода
	void ReversePolishEntry(string mystring);//разбор строки на обратную польскую запись
	int EvaluatePolish();//вычисление выражение по обратной польской записи
	~Code() {};//пустой деструктор
};
