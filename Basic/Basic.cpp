#include "Basic.h"

ifstream indata("code.txt");

istream& operator >> (istream& is, Str& string)
{
	Str new_el;
	if (is >> ws && is >> new_el.number && getline(is, new_el.expression, '\n'))
	{
		new_el.expression.erase(0, 1);
		new_el.statement = DEFAULT;
		if (int pos = new_el.expression.find("PRINT") != -1)
		{
			new_el.statement = PRINT;
			new_el.expression.erase(pos-1, 6);
		}
		if (int pos = new_el.expression.find("INPUT") != -1)
		{
			new_el.statement = INPUT;
			new_el.expression.erase(pos - 1, 6);
		}
		if (int pos = new_el.expression.find("IF") != -1)
		{
			new_el.statement = IF;
			new_el.expression.erase(pos - 1, 3);
		}
		if (int pos = new_el.expression.find("GOTO") != -1)
		{
			new_el.statement = GOTO;
			new_el.expression.erase(pos - 1, 5);
		}
		if (int pos = new_el.expression.find("GOSUB") != -1)
		{
			new_el.statement = GOSUB;
			new_el.expression.erase(pos - 1, 6);
		}
		if (int pos = new_el.expression.find("RETURN") != -1)
		{
			new_el.statement = RETURN;
			new_el.expression.erase(pos - 1, 7);
		}
		if (int pos = new_el.expression.find("END") != -1)
		{
			new_el.statement = END;
			new_el.expression.erase(pos - 1, 4);
		}
		string = new_el;
	}
	return is;
}

void Code::GetCode()
{
	Str curElem;
	while (indata >> curElem)
		line.push_back(curElem);
};

void Code::PrintCode()
{
	for (int i = 0; i < (int)line.size(); i++)
	{
		cout << line[i].number << " ";
		switch(line[i].statement)
		{
			case PRINT:
			{
				cout << "PRINT " << line[i].expression << endl;
				break;
			}
			case INPUT:
			{
				cout << "INPUT " << line[i].expression << endl;
				break;
			}
			case IF:
			{
				cout << "IF " << line[i].expression << endl;
				break;
			}
			case GOTO:
			{
				cout << "GOTO " << line[i].expression << endl;
				break;
			}
			case GOSUB:
			{
				cout << "GOSUB " << line[i].expression << endl;
				break;
			}
			case RETURN:
			{
				cout << "RETURN" << line[i].expression << endl;
				break;
			}
			case END:
			{
				cout << "END" << line[i].expression << endl;
				break;
			}
			default:
			{
				cout << line[i].expression << endl;
				break;
			}
		}
	}
}

void Code::IsAlphabet(char* str)//все переменные - комбинации с большой буквы
{
	int i = 0;
	while (str[i] != '\0')
	{
		if (str[i] > 'Z' || str[i] < 'A')
		{
			error = INVALID_NAME_OF_VARIABLE;
			break;
		}
		i++;
	}
}

void Code::InputInterpretation(string expression)//после строки обязательно ; и переменная
{
	int pos;
	char buffer[255];
	string curString;
	int flagOfString = 0;
	while (!expression.empty())
	{
		if (expression[0] == '"')
		{
			expression.erase(0, 1);
			pos = expression.find('"');
			if (pos != -1)
			{
				expression.copy(buffer, pos);
				buffer[pos] = '\0';
				cout << buffer;
				expression.erase(0, pos + 1);
				flagOfString += 1;
			}
			else
			{
				cout << "ERROR IN LINE " << (numOfCurLine + 1) * 10 << ": NOT FOUND END OF STRING" << endl;
				error = NOT_FOUND_END_OF_STRING;
				return;
			}
		}
		if (!(expression.empty()) && (expression[0] != ';') && flagOfString == 1)
		{
			error = SYNTAX_ERROR;
			cout << "ERROR IN LINE " << (numOfCurLine + 1) * 10 << ": NOT FOUND ';' AFTER STRING" << endl;
			return;
		}
		if ((expression[0] == ';' && flagOfString <= 1) || (expression[0] <= 'Z' && expression[0] >= 'A'))//может не быть строки-приглашения, а может быть
		{
			if (expression[0] == ';')
				expression.erase(0, 1);
			pos = expression.find(";");
			if (pos != -1)//если переменная не одна
			{
				expression.copy(buffer, pos);
				buffer[pos] = '\0';
			}
			else
			{
				expression.copy(buffer, expression.size());
				buffer[expression.size()] = '\0';
			}
			IsAlphabet(buffer);
			if (error == INVALID_NAME_OF_VARIABLE)
			{
				cout << "ERROR IN LINE " << (numOfCurLine + 1) * 10 << ": INVALID NAME OF VARIABLE" << endl;
				return;
			}
			else
			{
				curString = string(buffer);
				cin >> vars[curString];
				varsInit[curString] = 1;//переменная создана и проинициализирована
				expression.erase(0, curString.size());
			}
		}
	}
}

void Code::PrintInterpretation(string expression)
{
	int pos;
	char buffer[255];
	string curString;
	int flagOfString = 0;
	while (!expression.empty())
	{
		flagOfString = 0;
		if (expression[0] == '"')
		{
			expression.erase(0, 1);
			pos = expression.find('"');
			if (pos != -1)
			{
				expression.copy(buffer, pos);
				buffer[pos] = '\0';
				cout << buffer;
				expression.erase(0, pos + 1);
				flagOfString += 1;
			}
			else
			{
				cout << "ERROR IN LINE " << (numOfCurLine + 1) * 10 << ": NOT FOUND END OF STRING" << endl;
				error = NOT_FOUND_END_OF_STRING;
				return;
			}
		}
		if (!(expression.empty()) && (expression[0] != ';') && flagOfString == 1)
		{
			error = SYNTAX_ERROR;
			cout << "ERROR IN LINE " << (numOfCurLine + 1) * 10 << ": NOT FOUND ';' AFTER STRING" << endl;
			return;
		}
		if ((expression[0] == ';' && flagOfString <= 1) || (expression[0] <= 'Z' && expression[0] >= 'A'))//может не быть строки-приглашения, а может быть
		{
			if (expression[0] == ';')
				expression.erase(0, 1);
			pos = expression.find(";");//рассмотри случай когда после переменной идет строка и потеряна ;
			if (pos != -1)//если переменная не одна
			{
				expression.copy(buffer, pos);
				buffer[pos] = '\0';
			}
			else
			{
				expression.copy(buffer, expression.size());
				buffer[expression.size()] = '\0';
			}
			IsAlphabet(buffer);
			if (error == INVALID_NAME_OF_VARIABLE)
			{
				cout << "ERROR IN LINE " << (numOfCurLine + 1) * 10 << ": INVALID NAME OF VARIABLE" << endl;
				return;
			}
			else
			{
				curString = string(buffer);
				if (varsInit[curString] == 1)
					cout << vars[curString];
				else
				{
					error = UNDEFINED_VARIABLE;
					cout << "ERROR IN LINE " << (numOfCurLine + 1) * 10 << ": UNDEFINED VARIABLE" << endl;
					return;
				}
				if (pos != -1)//в инпуте можно обработать ошибку связанную с двойными строками таким образом, так как при удалении остается точка с запятой
					expression.erase(0, curString.size() + 1);
				else
					expression.erase(0, curString.size());
			}
		}
	}
}

void Code::GoToInterpretation(string expression)
{
	char buf[20];
	expression.copy(buf, expression.size());
	buf[expression.size()] = '\0';
	int curLine = atoi(buf);
	if (line.size() * 10 < curLine || curLine < 0 || curLine % 10 != 0)
	{
		error = GOTO_ERROR;
		cout << "ERROR IN LINE " << (numOfCurLine + 1) * 10 << ": UNDEFINED NUMBER OF STRING" << endl;
		return;
	}
	else
		numOfCurLine = int(curLine / 10 - 1); 
}

void Code::Interpretation()
{
	string curString;
	numOfCurLine = 0;
	while (line[numOfCurLine].statement != END)
	{
		curString = line[numOfCurLine].expression; //чтобы не портить исходные строки в процессе интерпретации
		switch (line[numOfCurLine].statement)
		{
		case PRINT:
		{
			PrintInterpretation(curString);
			cout << endl;
			numOfCurLine++;
			break;
		}
		case INPUT:
		{
			InputInterpretation(curString);
			numOfCurLine++;
			break;
		}
		case IF:
		{
			numOfCurLine++;
			break;
		}
		case GOTO:
		{
			GoToInterpretation(curString);
			break;
		}
		case GOSUB:
		{
			numOfCurLine++;
			break;
		}
		case RETURN:
		{
			numOfCurLine++;
			break;
		}
		default:
		{
			numOfCurLine++;
			break;
		}
		}
	}
}