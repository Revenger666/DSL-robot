/*CODE BY 310-2021211338-����ͮ �������ʵ�� ������ DSLʵ�ֿͷ�������*/
/*����һ������������õ�׮�����Ĵ���*/

#include"Parser.h"
#include <Windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;

//����׮��ģ��ִ�к�������ִ���ҷ�����һ����״̬��
int Action::TEST_Execute(string& now_step, string& now_input, VarList var)
{
	int x;
	srand(time(0));
	x = rand()%7;//�������״̬0-6��״̬�б�μ�parser.h�еĺ궨��
	return x;
}

//����׮��Ϊ��ģ���Ѿ��ɹ����벢ʶ�����һ���ű����������Ƿ�����������ʶ��õĽű������пͷ������ˡ�
bool Parser::TEST_Parser_file(const char* filename)
{
	/*
	һ���򵥵Ĳ�����STEP��

	Step TEST_speak_step
	Speak this is a speak step which is used to test program.
	Exit
	End
	
	*/
	entry = "TEST_speak_step";//���Step

	Step new_step;
	new_step.name = "TEST_speak_step";
	//Speak����
	Speak* new_speak = new Speak;
	new_speak->words = "this is a speak step which is used to test program.";
	new_speak->action_type = "Speak";
	new_step.behavior.push_back(new_speak);
	//Exit����
	Exit* new_exit = new Exit;
	new_exit->action_type = "Exit";
	new_step.behavior.push_back(new_exit);
	//Step�����б����
	stepList.push_back(new_step);
	stepPos.insert(pair<string, int>(stepList[0].name, 0));
	
	return true;//����ֵһ���ǳɹ���
}