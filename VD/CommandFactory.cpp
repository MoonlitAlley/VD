#include "CommandFactory.h"
#include <iostream>
#include "MyString.h"

#include "ClsCmd.h"

#include"CdCmd.h"
#include "DirCmd.h"


#include"MdCmd.h"
#include "DelCmd.h"
#include "RdCmd.h"
#include "TouchCmd.h"

#include "MklinkCmd.h"
#include "RenCmd.h"
#include "MoveCmd.h"
#include "CopyCmd.h"

#include "SaveCmd.h"
#include "LoadCmd.h"
CommandFactory::CommandFactory()
{
	command = NULL;
}

CommandFactory::~CommandFactory()
{
	//系统最后执行的一条命令，置空
	if (command != NULL)
	{
		delete command;
		command = NULL;
	}
}

Command * CommandFactory::CreatCommand(const string& cmd)
{
	
	//先将命令置空，方便下面判断返回
	if (command != NULL)
	{
		delete command;
		command = NULL;
	}

	//特殊命令预处理
	
	string tempStr = cmd;
	if (cmd == "cd..")
	{
		tempStr = "cd ..";
	}
	vector<string> items = StrProcess::SplitNoEmpty(tempStr, ' ');

	if (items.empty())
	{
		return NULL;
	}
	CmdTypeAnalyse(items[0]);

	switch (commandType)
	{
	case clsCmd:
		command = new ClsCmd();
		break;

	case cdCmd:
		command = new CdCmd();
		break;
	case saveCmd:
		command = new SaveCmd();
		break;
	case loadCmd:
		command = new LoadCmd();
		break;

	case dirCmd:
		command = new DirCmd();
		command->cmdParaCollection.m_definedOptions.insert("/s");
		command->cmdParaCollection.m_definedOptions.insert("/ad");
		break;
	case touchCmd:
		command = new TouchCmd();
		break;
	case rdCmd:
		command = new RdCmd();
		command->cmdParaCollection.m_definedOptions.insert("/s");
		break;
	case delCmd:
		command = new DelCmd();
		command->cmdParaCollection.m_definedOptions.insert("/s");
		break;
	case mdCmd:
		command = new MdCmd();
		break;

	case renCmd:
		command = new RenCmd();
		break;
	case moveCmd:
		command = new MoveCmd();
		command->cmdParaCollection.m_definedOptions.insert("/y");
		break;
	case copyCmd:
		command = new CopyCmd();
		command->cmdParaCollection.m_definedOptions.insert("/y");
		break;
	case mklinkCmd:
		command = new MklinkCmd();
		//本系统中不考虑/d参数
		//command->cmdParaCollection.m_definedOptions.insert("/d");
		break;

	default:
		break;
	}

	if (!command){	return NULL;	}

	//设置命令所支持的路径数量
	SetCmdPathCount();
	//解析用户输入，将得到的参数及路径放入到命令参数集合中
	if (!command->cmdParaCollection.SplitUserInput(tempStr))
	{
		cout << "命令参数或路径错误 " << endl;
		return NULL;
	}
	return command;
}

//根据cmdElent中的命令字符串，分析出命令ID
void CommandFactory::CmdTypeAnalyse(const string& cmdName)
{
	string CommandCollection[13] = { "cls" ,"cd","save","load", "dir","touch","rd","del","md","ren","copy" ,"move" ,"mklink" };

	commandType = -1;
	for (int i = 0; i < sizeof(CommandCollection) / sizeof(CommandCollection[0]); i++)
	{
		if (cmdName == CommandCollection[i])
		{
			commandType = i;
		}
	}
}

void CommandFactory::SetCmdPathCount()
{
	//设置命令可支持的路径数量
	if (commandType == clsCmd)
	{
		command->cmdParaCollection.SetPathCount(0, 0);
	}
	if (commandType == cdCmd)
	{
		command->cmdParaCollection.SetPathCount(0, 1);
	}
	if (commandType == dirCmd)
	{
		command->cmdParaCollection.SetPathCount(0, -1);
	}
	if (commandType == saveCmd || commandType == loadCmd)
	{
		command->cmdParaCollection.SetPathCount(1, 1);
	}
	if (commandType == touchCmd || commandType == rdCmd || commandType == delCmd || commandType == mdCmd)
	{
		command->cmdParaCollection.SetPathCount(1, -1);
	}
	if (commandType == copyCmd  || commandType == renCmd || commandType == mklinkCmd)
	{
		command->cmdParaCollection.SetPathCount(2, 2);
	}
	if (commandType == moveCmd)
	{
		command->cmdParaCollection.SetPathCount(1, 2);
	}
}

