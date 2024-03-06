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
	//Speak��Ҫ����Ĵʻ�
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
	//��һ����ת��Step����
	int Execute(string& now_step, string& now_input, VarList var);
	string jumpTo;
};

class Default : public TEST_Action
{
public:
	//��һ����ת��Step����
	int Execute(string& now_step, string& now_input, VarList var);
	string jumpTo;
};

class TEST_Step
{
public:
	//Step������
	string name;
	//Step�еĸ���Action
	vector<TEST_Action*> behavior;

};

