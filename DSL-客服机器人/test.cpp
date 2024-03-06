/*CODE BY 310-2021211338-郭柏彤 程序设计实践 大三上 DSL实现客服机器人*/
/*这是一个定义测试所用的桩函数的代码*/

#include"Parser.h"
#include <Windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;

//测试桩，模拟执行函数正常执行且返回了一定的状态、
int Action::TEST_Execute(string& now_step, string& now_input, VarList var)
{
	int x;
	srand(time(0));
	x = rand()%7;//随机生成状态0-6，状态列表参见parser.h中的宏定义
	return x;
}

//测试桩，为了模拟已经成功读入并识别完成一个脚本，来测试是否能正常按照识别好的脚本来运行客服机器人。
bool Parser::TEST_Parser_file(const char* filename)
{
	/*
	一个简单的测试用STEP：

	Step TEST_speak_step
	Speak this is a speak step which is used to test program.
	Exit
	End
	
	*/
	entry = "TEST_speak_step";//入口Step

	Step new_step;
	new_step.name = "TEST_speak_step";
	//Speak操作
	Speak* new_speak = new Speak;
	new_speak->words = "this is a speak step which is used to test program.";
	new_speak->action_type = "Speak";
	new_step.behavior.push_back(new_speak);
	//Exit操作
	Exit* new_exit = new Exit;
	new_exit->action_type = "Exit";
	new_step.behavior.push_back(new_exit);
	//Step加入列表操作
	stepList.push_back(new_step);
	stepPos.insert(pair<string, int>(stepList[0].name, 0));
	
	return true;//返回值一定是成功的
}