/*CODE BY 310-2021211338-郭柏彤 程序设计实践 大三上 DSL实现客服机器人*/
/*主函数*/

#include <iostream>
#include "Parser.h"

using namespace std;

int main(int argc, char* argv[])
{
	//修改此文件名，可以改变读取的文件
	const char* filename = "in.txt";
	Parser script;
	
	//这是一个测试桩函数，若不测试注释掉即可
	//if(script.TEST_Parser_file(filename))
	if (script.Parser_file(filename))//编译脚本成功
	{
		/*
		//可实现对脚本文件分析后的整体结构再输出，以检查是否正确识别了脚本文件。
		for (int i = 0; i < script.stepList.size(); i++)
		{
			cout<<"stepname:" << script.stepList[i].name << endl;
			cout <<"stepbehaviour_size: " << script.stepList[i].behavior.size() << endl;
			for (int j = 0; j < script.stepList[i].behavior.size(); j++)
			{
				cout << "stepbehaviour: " << script.stepList[i].behavior[j]->action_type<<endl;
			}

		}
		*/
		script.Run();//运行客服机器人
	}
	else
	{
		cout << "脚本文件编译错误！";
		return 0;
	}
	return 0;
}
