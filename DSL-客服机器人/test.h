#include "Parser.h"

using namespace std;


class TEST_Action
{
public:
	string action_type;
	virtual int Execute(string& now_step, string& now_input, VarList var) = 0;
};

class Speak :public TEST_Action
{

public:
	int Execute(string& now_step, string& now_input, VarList var);
	//Speak中要输出的词汇
	string words;
};

class Branch : public TEST_Action
{
public:
	int Execute(string& now_step, string& now_input, VarList var);
	map<string, string> jump;
};

class Listen : public TEST_Action
{
public:
	int Execute(string& now_step, string& now_input, VarList var);
	int waitTime = 10;
};

class Exit : public TEST_Action
{
public:
	int Execute(string& now_step, string& now_input, VarList var);
};

class Silence : public TEST_Action
{
public:
	//下一步跳转的Step名称
	int Execute(string& now_step, string& now_input, VarList var);
	string jumpTo;
};

class Default : public TEST_Action
{
public:
	//下一步跳转的Step名称
	int Execute(string& now_step, string& now_input, VarList var);
	string jumpTo;
};

class TEST_Step
{
public:
	//Step的名称
	string name;
	//Step中的各个Action
	vector<TEST_Action*> behavior;

};

