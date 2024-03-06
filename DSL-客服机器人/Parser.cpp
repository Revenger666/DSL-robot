/*CODE BY 310-2021211338-郭柏彤 程序设计实践 大三上 DSL实现客服机器人*/
/*脚本分析的具体函数定义*/

#include "Parser.h"
#include<fstream>
#include<sstream>
#include<iostream>
#include<iomanip>
#include<algorithm>
#include<cstring>
#include<vector>
#include<cstring> 
#include<map>
#include<set>
using namespace std;

//析构函数
Parser::~Parser()
{
	for (auto& iter : stepList)
	{
		for (auto& it : iter.behavior)
		{
			delete(it);
		}
	}
}

//读取脚本文件，并对脚本进行分析
bool Parser::Parser_file(const char* filename)
{
	//通过流操作来实现对脚本文件的逐行读取
	ifstream file;
	file.open(filename);
	
	string key; //操作名称
	string temp,t;//用于将流输入的逐行内容存储至临时字符串数组buffer中，并按空格分隔
	int now_line = 0;//当前行数

	while (!file.eof())//当脚本读取未完时
	{
		vector<string> buffer;
		//读取一行文件
		now_line++;
		getline(file, temp);
		stringstream linestream;
		linestream.str(temp);
		while (linestream >> t) buffer.push_back(t);//存至buffer中，以字符串的形式
		//读取行首关键字
		if (buffer.size()) //跳过空行
		{
			key = buffer[0];
			if (key == "Set")//SET类
			{
				if (buffer.size() != 3)
				{
					cout << "脚本编译出现错误，错误位于" << now_line << "行。" << endl;
					return false;
				}
				string name = buffer[1];
				string value = buffer[2];
				var.var_list.insert(pair<string, string>(name, value));
			}
			else if (key == "Entry")//设置入口类
			{
				if (buffer.size() != 2)
				{
					cout << "脚本编译出现错误，错误位于" << now_line << "行。" << endl;
					return false;
				}
				entry = buffer[1];
			}
			else if (key == "Step")//设置新步骤
			{
				Step new_step;
				if (buffer.size() != 2)
				{
					cout << "脚本编译出现错误，错误位于" << now_line << "行。" << endl;
					return false;
				}
				new_step.name = buffer[1];

				//创建一个新step
				//再读一行
				now_line++;
				buffer.clear();
				getline(file, temp);
				stringstream linestream;
				linestream.str(temp);
				while (linestream >> t) buffer.push_back(t);
				
				int listen_flag = 0;//实现脚本的语法检查，要求listen后必须依次为Branch,Default,Silence
				if (buffer.size())
				{
					while (buffer[0] != "End")//读取到的行里都是这个STEP的内容
					{
						if (buffer.size())
						{
							if (buffer[0] == "Speak")//Speak操作
							{
								if (!SpeakProcess(new_step, buffer))
								{
									cout << "脚本编译出现错误，错误位于" << now_line << "行。" << endl;
									return false;
								}
							}
							if (buffer[0] == "Listen")//Listen操作
							{
								listen_flag = 3;
								if (!ListenProcess(new_step, buffer))
								{
									cout << "脚本编译出现错误，错误位于" << now_line << "行。" << endl;
									return false;
								}
							}
							if (buffer[0] == "Branch")//Branch操作
							{
								if (listen_flag != 3)//要求Branch必须在Listen后
								{
									cout << "脚本编译出现错误，错误位于" << now_line << "行。" << endl;
									return false;
								}
								listen_flag--;
								if (!BranchProcess(new_step, buffer))
								{
									cout << "脚本编译出现错误，错误位于" << now_line << "行。" << endl;
									return false;
								}
							}
							if (buffer[0] == "Default")//Default操作
							{
								listen_flag--;
								if (!DefaultProcess(new_step, buffer))
								{
									cout << "脚本编译出现错误，错误位于" << now_line << "行。" << endl;
									return false;
								}
							}
							if (buffer[0] == "Exit")//Exit操作
							{
								ExitProcess(new_step, buffer);
							}
							if (buffer[0] == "Silence")//Silence操作
							{
								if (listen_flag != 1)//要求Silence必须是Listen后的第三个操作
								{
									//cout << listen_flag << endl;
									cout << "脚本编译出现错误，错误位于" << now_line << "行。" << endl;
									return false;
								}
								listen_flag--;
								if (!SilenceProcess(new_step, buffer))
								{
									cout << "脚本编译出现错误，错误位于" << now_line << "行。" << endl;
									return false;
								}
							}
							//错误的操作名称，将直接报错
							if (buffer[0] != "Speak" && buffer[0] != "Listen" && buffer[0] != "Branch" && buffer[0] != "Default" && buffer[0] != "Exit" && buffer[0] != "Silence" && buffer[0] != "End")
							{
								cout << "脚本编译出现错误，错误位于" << now_line << "行。" << endl;
								return false;
							}
						}

						//再读一行
						now_line++;
						buffer.clear();
						getline(file, temp);
						stringstream linestream;
						linestream.str(temp);
						while (linestream >> t) buffer.push_back(t);
						/*
						for (int i = 0; i < buffer.size(); i++)
						{
							cout << buffer[i] << " ";
						}
						cout << endl;
						*/
					}
				}

				//读到End了，当前Step创建完成，将这个Step放入Step列表中
				stepList.push_back(new_step);
				buffer.clear();//清空临时存储用的字符串Vector
			}
		}	
	}
	//构造从step_name到step_pos的映射
	for (int pos = 0; pos < stepList.size(); pos++)
	{
		stepPos.insert(pair<string, int>(stepList[pos].name, pos));
	}
	return true;
}

//Speak步骤的具体构建过程
bool Parser::SpeakProcess(Step& step_to_modify, vector<string> buffer)
{
	Speak* new_speak = new Speak;//新建一个Speak步骤
	string word;
	word = buffer[1];
	for (int i = 2; i < buffer.size(); i++)
	{
		word = word +' '+ buffer[i];
	}
	new_speak->words = word;//将后面的内容存到这个Spwak要输出的字符串中
	new_speak->action_type = "Speak";
	step_to_modify.behavior.push_back(new_speak);//将该动作放入该STEP的步骤列表里
	return true;
}
//Branch步骤的具体构建过程
//Branch 2 查询 ask 充值 charge
bool Parser::BranchProcess(Step& step_to_modify, vector<string> buffer)
{
	Branch* new_branch = new Branch;//新建一个Branch步骤
	string branch_num_string = buffer[1];
	int branch_number = atoi(branch_num_string.c_str());//将参数个数从字符转为Int
	int temp_buf_pos = 2;
	//cout << branch_number<<endl;
	if (buffer.size() != branch_number * 2 + 2)//参数个数出现错误
	{
		cout << "脚本编译出现错误，请检查Branch分支数目"<< endl;
		return false;
	}
	for (int i = 1; i <= branch_number; i++)//将后面的每种跳转情况依次存储到列表中
	{
		string key_word = buffer[temp_buf_pos];
		temp_buf_pos++;
		string step_name = buffer[temp_buf_pos];
		temp_buf_pos++;
		new_branch->jump.insert(pair<string, string>(key_word, step_name));
	}
	new_branch->action_type = "Branch";
	step_to_modify.behavior.push_back(new_branch);//将该步骤放入当前Step的列表中
	return true;
}
//Listen 10
//Listen步骤的具体创建过程
bool Parser::ListenProcess(Step& step_to_modify, vector<string> buffer)
{
	if (buffer.size() != 2)//参数个数错误
	{
		cout << "脚本编译出现错误，请检查Listen后参数个数" << endl;
		return false;
	}
	Listen* new_listen = new Listen;//新建一个Listen操作
	string wait_time_string = buffer[1];
	int wait_time_number = atoi(wait_time_string.c_str());
	//将等待时间转为int类后存储
	new_listen->waitTime = wait_time_number;
	new_listen->action_type = "Listen";
	step_to_modify.behavior.push_back(new_listen);//将该操作放入当前的Step列表中
	return true;
}
//Silence步骤的具体创建过程
bool Parser::SilenceProcess(Step& step_to_modify, vector<string> buffer)
{
	if (buffer.size() != 2)//参数个数错误
	{
		cout << "脚本编译出现错误，请检查Silence后参数个数" << endl;
		return false;
	}
	Silence* new_silence = new Silence;//新建一个Silence操作
	string next_step = buffer[1];
	new_silence->jumpTo = next_step;//将要跳转到的步骤名称存到该步骤的跳转字符串中
	new_silence->action_type = "Silence";
	step_to_modify.behavior.push_back(new_silence);//将当前操作放入当前Step的列表中
	return true;
}

//Exit操作的具体创建步骤
bool Parser::ExitProcess(Step& step_to_modify, vector<string> buffer)
{
	if (buffer.size() != 1)//后面有多余的参数
	{
		cout << "脚本编译出现错误，请检查Exit后参数个数" << endl;
		return false;
	}
	Exit* new_exit = new Exit;
	new_exit->action_type = "Exit";
	step_to_modify.behavior.push_back(new_exit);//将当前操作放入当前Step的列表中
	return true;
}

//Default操作的具体步骤
bool Parser::DefaultProcess(Step& step_to_modify, vector<string> buffer)
{
	if (buffer.size() != 2)//参数个数错误
	{
		cout << "脚本编译出现错误，请检查Default后参数个数" << endl;
		return false;
	}
	Default* new_default = new Default;//新建一个Default操作
	string next_step = buffer[1];
	new_default->jumpTo = next_step;//将要跳转到的步骤名称放入当前步骤的对应字符串中
	new_default->action_type = "Default";
	step_to_modify.behavior.push_back(new_default);//将当前操作放入当前Step的列表中
	return true;
}