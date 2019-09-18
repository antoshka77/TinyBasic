#include "Basic.h"

istream& operator >> (istream& is, Str& string)
{
	Str new_el;
	if (is >> ws && is >> new_el.number && getline(is, new_el.expression, '\n'))
	{
		new_el.expression.erase(0, 1);
		new_el.statement = DEFAULT;
		if (int pos = new_el.expression.find("IF") != -1)
		{
			new_el.statement = IF;
			new_el.expression.erase(pos - 1, 3);
		}
		else
		{
			if (int pos = new_el.expression.find("PRINT") != -1)
			{
				new_el.statement = PRINT;
				new_el.expression.erase(pos - 1, 6);
			}
			if (int pos = new_el.expression.find("INPUT") != -1)
			{
				new_el.statement = INPUT;
				new_el.expression.erase(pos - 1, 6);
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
			if (int pos = new_el.expression.find("LET") != -1)
			{
				new_el.statement = LET;
				new_el.expression.erase(pos - 1, 4);
			}
			if (int pos = new_el.expression.find("END") != -1)
			{
				new_el.statement = END;
				new_el.expression.erase(pos - 1, 4);
			}
		}
		string = new_el;
	}
	return is;
}

void Code::GetCode(char* indata)
{
	Str curElem;
	ifstream in(indata);
	while (in >> curElem)
	{
		line[curElem.number] = curElem;
		numOfLines.push_back(curElem.number);
	}
};

void Code::PrintCode()
{
	for (int i = 0; i < (int)numOfLines.size(); i++)
	{
		cout << line[numOfLines[i]].number << " ";
		switch(line[numOfLines[i]].statement)
		{
			case PRINT:
			{
				cout << "PRINT " << line[numOfLines[i]].expression << endl;
				break;
			}
			case INPUT:
			{
				cout << "INPUT " << line[numOfLines[i]].expression << endl;
				break;
			}
			case IF:
			{
				cout << "IF " << line[numOfLines[i]].expression << endl;
				break;
			}
			case GOTO:
			{
				cout << "GOTO " << line[numOfLines[i]].expression << endl;
				break;
			}
			case GOSUB:
			{
				cout << "GOSUB " << line[numOfLines[i]].expression << endl;
				break;
			}
			case RETURN:
			{
				cout << "RETURN" << line[numOfLines[i]].expression << endl;
				break;
			}
			case LET:
			{
				cout << "LET " << line[numOfLines[i]].expression << endl;
				break;
			}
			case END:
			{
				cout << "END" << line[numOfLines[i]].expression << endl;
				break;
			}
			default:
			{
				cout << line[numOfLines[i]].expression << endl;
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
				el.code = NOT_FOUND_END_OF_STRING;
				el.numOfStr = line[numOfLines[numOfCurLine]].number;
				err.push_back(el);
				return;
			}
		}
		if (!(expression.empty()) && (expression[0] != ';') && flagOfString == 1)
		{
			el.code = SYNTAX_ERROR;
			el.numOfStr = line[numOfLines[numOfCurLine]].number;
			err.push_back(el);
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
			else//переменная одна
			{
				expression.copy(buffer, expression.size());
				buffer[expression.size()] = '\0';
			}
			IsAlphabet(buffer);
			if (error == INVALID_NAME_OF_VARIABLE)
			{
				el.code = INVALID_NAME_OF_VARIABLE;
				el.numOfStr = line[numOfLines[numOfCurLine]].number;
				err.push_back(el);
				error = SUCCESS;
				return;
			}
			else
			{
				curString = string(buffer);
				cin >> vars[curString];//ввожу значение переменной
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
				el.code = NOT_FOUND_END_OF_STRING;
				el.numOfStr = line[numOfLines[numOfCurLine]].number;
				err.push_back(el);
				return;
			}
		}
		if (!(expression.empty()) && (expression[0] != ';') && flagOfString == 1)
		{
			el.code = SYNTAX_ERROR;
			el.numOfStr = line[numOfLines[numOfCurLine]].number;
			err.push_back(el);
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
				el.code = INVALID_NAME_OF_VARIABLE;
				el.numOfStr = line[numOfLines[numOfCurLine]].number;
				err.push_back(el);
				error = SUCCESS;
				return;
			}
			else
			{
				curString = string(buffer);
				if (varsInit[curString] == 1)
					cout << vars[curString];
				else
				{
					el.code = UNDEFINED_VARIABLE;
					el.numOfStr = line[numOfLines[numOfCurLine]].number;
					err.push_back(el);
					return;
				}
				if (pos != -1)
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
	for (int i = 0; i < int(numOfLines.size()); i++)
	{
		if (numOfLines[i] == curLine)
		{
			numOfCurLine = i;
			break;
		}
	} 
}

void Code::GoSubInterpretation(string expression)
{
	char buf[20];
	expression.copy(buf, expression.size());
	buf[expression.size()] = '\0';
	int curLine = atoi(buf);
	numOfGoSub = numOfCurLine;
	for (int i = 0; i < int(numOfLines.size()); i++)
	{
		if (numOfLines[i] == curLine)
		{
			numOfCurLine = i;
			break;
		}
	}
}

int Code::IfInterpretation(string expression)//считаем что в выражениях нет пробелов
{
	int pos[6];
	int i;
	pos[0] = expression.find("<=");
	pos[1] = expression.find("<>");
	pos[2] = expression.find(">=");
	pos[3] = expression.find("<");
	pos[4] = expression.find("=");
	pos[5] = expression.find(">");
	for (i = 0; i < 6; i++)
	{
		if (pos[i] != -1)
			break;
	}
	string buf_1(expression.begin(), expression.begin() + pos[i]);
	ReversePolishEntry(buf_1);
	int res_1 = EvaluatePolish();
	if (i >= 0 && i < 3)
		pos[i] += 2;
	else
		pos[i] += 1;
	int pos_th = expression.find("THEN");
	string buf_2(expression.begin() + pos[i], expression.begin() + pos_th - 1);
	ReversePolishEntry(buf_2);
	int res_2 = EvaluatePolish();
	switch (i)
	{
	case 0:
		return (res_1 <= res_2) ? 1 : 0;
	case 1:
		return (res_1 != res_2) ? 1 : 0;
	case 2:
		return (res_1 >= res_2) ? 1 : 0;
	case 3:
		return (res_1 < res_2) ? 1 : 0;
	case 4:
		return (res_1 == res_2) ? 1 : 0;
	case 5:
		return (res_1 > res_2) ? 1 : 0;
	default: 
		return 0;
	}
}

void Code::RerurnInterpretation()
{
	if (numOfGoSub != 0)
	{
		numOfCurLine = numOfGoSub;
		numOfGoSub = 0;
		return;
	}
	else
	{
		el.code = RETURN_ERROR;
		el.numOfStr = line[numOfLines[numOfCurLine]].number;
		err.push_back(el);
		return;
	}
}

void Code::PrintErrors()
{
	cout << endl;
	for (int i = 0; i < int(err.size()); i++)
	{
		switch (err[i].code)
		{
		case NOT_FOUND_END_OF_STRING:
		{
			cout << "ERROR IN LINE " << err[i].numOfStr << ": NOT FOUND END OF STRING" << endl;
			break;
		}
		case INVALID_NAME_OF_VARIABLE:
		{
			cout << "ERROR IN LINE " << err[i].numOfStr << ": INVALID NAME OF VARIABLE" << endl;
			break;
		}
		case SYNTAX_ERROR:
		{
			cout << "ERROR IN LINE " << err[i].numOfStr << ": NOT FOUND ';' AFTER STRING" << endl;
			break;
		}
		case UNDEFINED_VARIABLE:
		{
			cout << "ERROR IN LINE " << err[i].numOfStr << ": UNDEFINED VARIABLE" << endl;
			break;
		}
		case GOTO_ERROR:
		{
			cout << "ERROR IN LINE " << err[i].numOfStr << ": UNDEFINED NUMBER OF STRING" << endl;
			break;
		}
		case GOSUB_ERROR:
		{
			cout << "ERROR IN LINE " << err[i].numOfStr << ": UNDEFINED NUMBER OF STRING" << endl;
			break;
		}
		case RETURN_ERROR:
		{
			cout << "ERROR IN LINE " << err[i].numOfStr << ": YOU HAVEN'T GOSUB FOR THIS RETURN" << endl;
			break;
		}
		case ERROR_OF_EVALUATE:
		{
			cout << "ERROR IN LINE " << err[i].numOfStr << ": ERROR OF EVALUATE" << endl;
			break;
		}
		case ERROR_OF_EXPRESSION:
		{
			cout << "ERROR IN LINE " << err[i].numOfStr << ": UNKNOWN SYMBOL IN MATH EXPRESSION" << endl;
			break;
		}
		}
	}
}

void Code::DefaultInterpetation(string expression)
{
	while (!expression.empty())
	{
		int pos = expression.find("=");
		string var(expression, 0, pos);
		expression.erase(0, pos + 1);
		pos = expression.find(":");
		if (pos != -1)
		{
			string buf(expression, 0, pos);
			ReversePolishEntry(buf);
			vars[var] = EvaluatePolish();
			if (flagOfEvaluate == 0 && varsInit[var] == 1 || flagOfLet == 0 || varsInit[var] == 1)
				varsInit[var] = 1;
			else
				varsInit[var] = 0;
			expression.erase(0, pos + 1);
		}
		else
		{
			ReversePolishEntry(expression);
			vars[var] = EvaluatePolish();
			if (flagOfEvaluate == 0 && varsInit[var] == 1 || flagOfLet == 0 || varsInit[var] == 1)
				varsInit[var] = 1;
			else
				varsInit[var] = 0;
			expression.clear();
		}
	}
}

void Code::Interpretation()
{
	string curString;
	numOfCurLine = 0;
	int flag = 0;//дефолтное состояние для if
	while (line[numOfLines[numOfCurLine]].statement != END)
	{
		flagOfLet = 1;
		curString = line[numOfLines[numOfCurLine]].expression; //чтобы не портить исходные строки в процессе интерпретации
		switch (line[numOfLines[numOfCurLine]].statement)
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
			if (IfInterpretation(curString) == 1)
			{
				int pos = curString.find("THEN") + 5;
				curString.erase(0, pos);
				if (pos = curString.find("PRINT") != -1)
				{
					curString.erase(pos - 1, 6);
					PrintInterpretation(curString);
					cout << endl;
					flag++;
					numOfCurLine++;
				}
				if (pos = curString.find("INPUT") != -1)
				{
					curString.erase(pos - 1, 6);
					InputInterpretation(curString);
					flag++;
					numOfCurLine++;
				}
				if (pos = curString.find("GOTO") != -1)
				{
					curString.erase(pos - 1, 5);
					GoToInterpretation(curString);
					flag++;
				}
				if (pos = curString.find("GOSUB") != -1)
				{
					curString.erase(pos - 1, 6);
					GoSubInterpretation(curString);
					flag++;
				}
				if (pos = curString.find("RETURN") != -1)
				{
					curString.erase(pos - 1, 7);
					flag++;
					numOfCurLine++;
				}
				if (pos = curString.find("LET") != -1)
				{
					curString.erase(pos - 1, 4);
					DefaultInterpetation(curString);
					flag++;
				}
				if (pos = curString.find("END") != -1)
				{
					curString.erase(pos - 1, 4);
					flag++;
					exit(0);
				}
				if (flag == 0)
				{
					DefaultInterpetation(curString);
					numOfCurLine++;
				}
			}
			else
				numOfCurLine++;
			flag = 0;
			break;
		}
		case GOTO:
		{
			GoToInterpretation(curString);
			break;
		}
		case GOSUB:
		{
			GoSubInterpretation(curString);
			break;
		}
		case RETURN:
		{
			RerurnInterpretation();
			numOfCurLine++;
			break;
		}
		case LET:
		{
			flagOfLet = 0;
			DefaultInterpetation(curString);
			numOfCurLine++;
			break;
		}
		default:
		{
			DefaultInterpetation(curString);
			numOfCurLine++;
			break;
		}
		}
	}
	PrintErrors();
}

int Code::IsItNum(char symb)
{
	return (symb >= '0' && symb <= '9') ? 1 : 0;
}

int Code::IsItSymb(char symb)
{
	return (symb >= 'A' && symb <= 'Z') ? 1 : 0;
}

int Code::IsItFunc(char symb)
{
	return (symb == '+' || symb == '-' || symb == '*' || symb == '/') ? 1 : 0;
}

int Code::atoi(string str)
{
	int num = 0;
	int flag = 0;
	for (int i = 0; i < int(str.length()); i++)
		num = num * 10 + str[i] - 0x30;
	return num;
}

int Priority(char symb)
{
	switch (symb)
	{
	case '*':
		return 2;
	case '/':
		return 2;
	case '-':
		return 1;
	case '+':
		return 1;
	default:
		return 0;
	}
}

int Code::IsAlphabet(string str)//все переменные - комбинации с большой буквы
{
	int flag = 1;
	for (int i = 0; i < int(str.size()); i++)
	{
		if (str[i] > 'Z' || str[i] < 'A')
		{
			flag = 0;
			return flag;
		}
		i++;
	}
	return flag;
}

int Code::IsNumber(string str)
{
	int flag = 1;
	for (int i = 0; i < int(str.size()); i++)
	{
		if (str[i] > '9' || str[i] < '0')
		{
			flag = 0;
			return flag;
		}
		i++;
	}
	return flag;
}

void Code::ReversePolishEntry(string mystring)
{
	int i = 0;
	string buf;
	int countBuf = 1;
	int countStack = 0;
	int flag = 1;
	string nullstr = "0";
	int shift = mystring.size();
	stack_p.clear();
	stack.clear();
	poli.clear();
	while (shift != -1)
	{
		if (IsItNum(mystring[i]) == 0 && IsItSymb(mystring[i]) == 0 && IsItFunc(mystring[i]) == 0 && mystring[i] != ')' && mystring[i] != '(' && shift != 0)
		{
			el.code = ERROR_OF_EXPRESSION;
			el.numOfStr = line[numOfLines[numOfCurLine]].number;
			err.push_back(el);
			error = ERROR_OF_EXPRESSION;
			return;
		}
		if (IsItNum(mystring[i]) == 1 || IsItSymb(mystring[i]) == 1)
		{
			countBuf++;
			buf.push_back(mystring[i]);
		}
		else
		{
			if (countBuf != 1)
			{
				if (IsAlphabet(buf))
				{
					if (varsInit[buf] == 1)
					{
						curElem.result.number = vars[buf];
						curElem.type = number;
						poli.push_back(curElem);
					}
					else
					{
						error = UNDEFINED_VARIABLE;
						el.code = UNDEFINED_VARIABLE;
						el.numOfStr = line[numOfLines[numOfCurLine]].number;
						err.push_back(el);
						return;
					}
				}
				else
				{
					if (IsNumber(buf))
					{
						curElem.result.number = atoi(buf);
						curElem.type = number;
						poli.push_back(curElem);
					}
					else
					{
						error = INVALID_NAME_OF_VARIABLE;
						el.code = INVALID_NAME_OF_VARIABLE;
						el.numOfStr = line[numOfLines[numOfCurLine]].number;
						err.push_back(el);
						return;
					}
				}
			}
			countBuf = 1;
			buf.erase(0, buf.size());
			if (mystring[i] == '(')
				stack.push_back(mystring[i]);
			if (IsItFunc(mystring[i]) == 1)
			{
				if (!stack.empty())
				{
					while (Priority(mystring[i]) <= Priority(stack[stack.size() - 1]))
					{
						curElem.result.func = stack[stack.size() - 1];
						curElem.type = func;
						poli.push_back(curElem);
						stack.pop_back();
						if (stack.empty())
							break;
					}
				}
				stack.push_back(mystring[i]);
			}
			if (mystring[i] == ')')
			{
				while (stack[stack.size() - 1] != '(')
				{
					curElem.result.func = stack[stack.size() - 1];
					curElem.type = func;
					poli.push_back(curElem);
					stack.pop_back();
				}
				stack.pop_back();
			}
		}
		i++;
		shift--;
	}
	while (!stack.empty())
	{
		curElem.result.func = stack[stack.size() - 1];
		curElem.type = func;
		poli.push_back(curElem);
		stack.pop_back();
	}
}

int Code::EvaluatePolish()
{
	flagOfEvaluate = 0;
	if (error != SUCCESS)
	{
		flagOfEvaluate = 1;
		el.code = ERROR_OF_EVALUATE;
		el.numOfStr = line[numOfLines[numOfCurLine]].number;
		err.push_back(el);
		error = SUCCESS;
		return -1;
	}
	for (int i = 0; i < int(poli.size()); i++)
	{
		if (poli[i].type == number)
			stack_p.push_back(poli[i].result.number);
		if (poli[i].type == 1)
		{
			switch (poli[i].result.func)
			{
			case '*':
				stack_p[stack_p.size() - 2] = stack_p[stack_p.size() - 2] * stack_p[stack_p.size() - 1];
				stack_p.pop_back();
				break;
			case '/':
				stack_p[stack_p.size() - 2] = stack_p[stack_p.size() - 2] / stack_p[stack_p.size() - 1];
				stack_p.pop_back();
				break;
			case '+':
				stack_p[stack_p.size() - 2] = stack_p[stack_p.size() - 2] + stack_p[stack_p.size() - 1];
				stack_p.pop_back();
				break;
			case '-':
				stack_p[stack_p.size() - 2] = stack_p[stack_p.size() - 2] - stack_p[stack_p.size() - 1];
				stack_p.pop_back();
				break;
			}
		}
	}
	return stack_p[0];
}
