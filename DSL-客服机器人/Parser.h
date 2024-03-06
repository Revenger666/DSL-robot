/*CODE BY 310-2021211338-郭柏彤 程序设计实践 大三上 DSL实现客服机器人*/

#ifndef PARSER_H
#define PARSER_H

//一些使用到的头文件定义
#include<vector>
#include<cstring>
#include<ctime>
#include<Windows.h>
#include<conio.h>
#include<map>
#include<iostream>
//宏定义，表示各个操作执行之后的状态
#define ERR 0 //程序运行错误
#define NORMAL 1 //输出后正常执行
#define JUMP 2 //步骤跳转，直接结束当前步骤遍历
#define HEAR 3 //接收到了用户输入，继续正常执行
#define SILENCE 4 //等待用户输入超时，跳过两个操作执行silence操作
#define DEFAULT 5 //结束客服机器人会话
#define EXIT 6 //无法识别用户输入，继续执行下一步的default

using namespace std;

//变量列表
class VarList
{
	public:
		//从变量名到变量值的映射
		map<string, string> var_list;
};

//操作类，父类
class Action
{
	public:
		string action_type;
		//虚函数，在继承的子类中详细定义每个操作的详细步骤
		virtual int Execute(string& now_step, string& now_input, VarList var) = 0;
		//用于测试的桩函数
		int TEST_Execute(string& now_step, string& now_input, VarList var);
};

//输出
class Speak :public Action
{
public:
	int Execute(string& now_step, string& now_input, VarList var);
	//Speak中要输出的词汇
	string words;
};

//根据用户输入进行分支跳转
class Branch : public Action
{
public:
	int Execute(string& now_step, string& now_input, VarList var);
	//从识别内容到跳转的步骤名的映射
	map<string, string> jump;
};

//等待用户输入
class Listen : public Action
{

public:
	int Execute(string& now_step, string& now_input, VarList var);
	//预设等待用户输入的时间为10S
	int waitTime = 10;
};

//退出客服机器人
class Exit : public Action
{
public:
	int Execute(string& now_step, string& now_input, VarList var);
};

//等待用户会话超时
class Silence : public Action
{
public:
	//下一步跳转的Step名称
	int Execute(string& now_step, string& now_input, VarList var);
	string jumpTo;
};

class Default : public Action
{
public:
	//下一步跳转的Step名称
	int Execute(string& now_step, string& now_input, VarList var);
	string jumpTo;
};

//步骤
class Step
{
public:
	//Step的名称
	string name;
	//Step中的各个Action
	vector<Action*> behavior;

};


class Parser
{
public:
	~Parser();
	//对脚本进行分析的函数，filename为脚本的文件名
	bool Parser_file(const char* filename);
	//测试桩函数，按照预设的一个最简单的脚本进行存储，可保证完全正确，以测试机器人执行部分的准确性
	bool TEST_Parser_file(const char* filename);
	//客服机器人执行函数
	void Run();

private:
	//生成对应的Action子类操作，并将操作添加到step_to_modify的stepList中
	bool SpeakProcess(Step& step_to_modify,vector<string> buffer);
	bool BranchProcess(Step& step_to_modify, vector<string> buffer);
	bool ListenProcess(Step& step_to_modify, vector<string> buffer);
	bool SilenceProcess(Step& step_to_modify, vector<string> buffer);
	bool ExitProcess(Step& step_to_modify, vector<string> buffer);
	bool DefaultProcess(Step& step_to_modify, vector<string> buffer);


public:
	//变量列表
	VarList var;
	//状态列表
	vector<Step> stepList;
	//从step名称到step列表中位置的映射
	map<string, int> stepPos;
	//入口的Step名
	string entry;
	//当前用户的输入
	string now_input;
	//当前的状态
	string now_step;
};

#endif // !INTERPRETER_H