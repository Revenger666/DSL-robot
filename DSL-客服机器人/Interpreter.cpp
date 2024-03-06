/*CODE BY 310-2021211338-郭柏彤 程序设计实践 大三上 DSL实现客服机器人*/
/*解释器，执行识别后的脚本，各种执行函数的具体定义*/

#include "Parser.h"
#include<Windows.h>

using namespace std;

//Speak操作的具体执行过程
int Speak::Execute(string& now_step, string& now_input, VarList var)
{
	//cout << "this is speak execute!" << endl;
	const char* word = words.c_str();//将字符串转换为字符数组的形式便于操作
	if (strlen(word) == 0) return ERR;
	string var_name;//变量的名称
	int  var_flag = 0;//当前是否正在识别变量，0代表没有，1代表在识别变量名中（读到了$)
	for (int i=0;i<strlen(word);i++)
	{
		if (word[i] != '$'&&var_flag==0)//如果不是变量名就直接输出
		{
			cout << word[i];
		}
		else if (word[i] == '$'&&var_flag==0)//读到了变量的前标识符
		{
			var_flag = 1;
		}
		else if (word[i] != '$' && var_flag == 1)//读取变量中，存入临时的变量名字符串
		{
			var_name += word[i];
		}
		else if (word[i] == '$' && var_flag == 1)//读到了变量的后标识符$
		{
			var_flag = 0;
			string temp_value = var.var_list.find(var_name)->second;//在变量列表中查找该变量对应的值
			cout << temp_value;//将值输出
			var_name = "";//临时变量名清空
		}
	}
	cout << endl;
	return NORMAL;
}

//Branch操作的具体执行过程定义
int Branch::Execute(string& now_step, string& now_input, VarList var)
{
	if (jump.find(now_input) == jump.end())//对于分支中没有找到用户输入对应的跳转操作
	{
		return DEFAULT;
	}
	//找到了，直接更改当前的Step
	string jump_to_name = jump.find(now_input)->second;
	//cout << "Branch:jump tp step: " << jump_to_name<<endl;
	now_step = jump_to_name;
	
	return JUMP;
}

//Listen操作的具体执行过程定义
int Listen::Execute(string& now_step, string& now_input, VarList var)
//实现等待输入
{
	time_t timeBegin = time(0);//等待计时
	//cout << "Listening:" << endl;
	while (true)
	{
		if (_kbhit())//检测到输入，取消计时，读取用户的输入
		{
			//cout << "检测到输入" << endl;
			cin >> now_input;
			return HEAR;
		}
		if (time(0)-timeBegin >=waitTime )//超时，返回Silence
		{
			//cout << "超时，断开"<<endl;
			return SILENCE;
		}
	}
	return ERR;
}

//Exit操作的具体执行过程定义
int Exit::Execute(string& now_step, string& now_input, VarList var)
{
	//直接退出程序
	//cout << "exit,return 0." << endl;
	return EXIT;
}

//Silence操作的具体执行过程定义
int Silence::Execute(string& now_step, string& now_input, VarList var)
{
	//cout << "silence:jump tp step:" << jumpTo << endl;
	//修改当前的Step,然后直接同于Default操作返回JUMP状态
	now_step = jumpTo;
	return JUMP;
}

//Default操作的具体执行过程定义
int Default::Execute(string& now_step, string& now_input, VarList var)
{
	//cout << "default:jump tp step:" << jumpTo<<endl;
	//修改当前的Step,然后直接返回JUMP状态
	now_step = jumpTo;
	return JUMP;
}


//主执行函数
void Parser::Run()
{
	//开始，STEP设置为入口STEP
	now_step = entry;
	while (true)
	{
		//每次操作停顿0.5S
		Sleep(500);
		//顺序执行当前STEP的一系列步骤
		int step_pos = stepPos.find(now_step)->second;
		//cout <<"now_step name is:" << stepList[step_pos].name << endl;
		int break_flag = 0;//用于在执行过程中遇到JUMP状态直接退出当前的Step操作遍历
		int silence_flag = 0;//用于遇到超时状态时计算三个步骤，找到对应的Silence操作
		Step step_now_step = stepList[step_pos];
		for (auto& iter : step_now_step.behavior)//遍历当前Step的所有操作
		{
			if (break_flag == 1)//JUMP状态，直接退出遍历
			{
				break;
			}
			if (silence_flag > 0)//Silence状态，要跳过两个操作
			{
				silence_flag--;
				continue;
			}
			int state = iter->Execute(now_step, now_input, var);//State为当前的状态，状态宏定义参见parser.h
			//int state = iter->TEST_Execute(now_step, now_input, var);
			//cout << state <<endl;
			switch (state)
			{
				case EXIT:
					//cout << "EXiT!" << endl;
					return;
				case ERR:
					cout << "客服机器人产生了意外的错误，即将结束程序！" << endl;
					return;
				case JUMP:
					break_flag = 1;
					//cout << "JUMP" << endl;
					break;
				case HEAR:
					//cout << "HEAR" << endl;
					break;
				case DEFAULT:
					//cout << "DEFAULT" << endl;
					break;
				case NORMAL:
					//cout << "NORMAL" << endl;
					break;
				case SILENCE:
					//cout << "SILENCE" << endl;
					silence_flag = 2;//实现跳过两个步骤
					break;
				default:
					cout << "客服机器人产生了意外的错误，即将结束程序！" << endl;
					return;
			}
		}
	}
	return;
}
