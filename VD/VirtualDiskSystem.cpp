#include "VirtualDiskSystem.h"
#include <iostream>
VirtualDiskSystem::VirtualDiskSystem()
{
	virtualDiskInside = new VirtualDiskInside();
	command = NULL;
}

VirtualDiskSystem::~VirtualDiskSystem()
{
	delete virtualDiskInside;
	virtualDiskInside = NULL;
	command = NULL;
}


bool VirtualDiskSystem::InitSystem()
{
	//执行文件系统的初始化工作
	if (!virtualDiskInside->InitFileSystem())
	{
		return false;
	}

	//初始化系统的命令
	string initCmd;
	/*string initCmd = "md /\"bin\"";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));*/

	initCmd = "md /\"b in\"/st";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));

	initCmd = "touch /\"b in\"/1.txt";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));
	initCmd = "touch /\"b in\"/11.txt";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));
	initCmd = "touch /\"b in\"/2.txt";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));



	initCmd = "mklink sym /\"b in\"";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));
	initCmd = "mklink sym1 sym";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));


	initCmd = "md baa";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));

	initCmd = "touch /baa/1.txt";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));
	initCmd = "touch /baa/11.txt";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));
	initCmd = "touch /baa/2.txt";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));

	initCmd = "touch /1.txt";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));
	initCmd = "touch /11.txt";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));
	initCmd = "touch /2.txt";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));


	initCmd = "mklink  s.txt /1.txt";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));
	initCmd = "mklink y.txt s.txt";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));
	//std::cout << "Program initialization completed!\n";
	RunSystem();
	return true;

}

void VirtualDiskSystem::RunSystem()
{
	bool flag = true;

	

	std::cout << "Program initialization completed!\n";
	while (1)
	{

		if (flag)
		{
			//string workingPathString = virtualDisk.GetWorkingPathString();
			//std::cout << virtualDiskInside->GetWorkingPathString() << "> ";

			//NEW
			std::cout << virtualDiskInside->workingPath_CD.str() << "> ";

			flag = false;
		}

		//读取用户命令 ，临时保存进
		char cmd[MAX_CMD_CHAR];
		std::cin.getline(cmd, MAX_CMD_CHAR);

		if (cmd == "exit")
			break;

		//判断是否是退出系统的指令
			//如果是退出系统，则break跳出当前循环接收

		//不是退出系统，开始构建命令并执行
		if (cmd[0] != '\0')
		{
			//临时保存用户的输入
			//userInput.wholeCmd = cmd;

			//使用命令工厂创建任务
			string cmdtemp = cmd;
			if (cmdtemp == "cd..")
			{
				cmdtemp = "cd ..";
			}
			command = commandFactory.CreatCommand(cmdtemp);
			if (command != NULL)
			{
				//命令创建成功，开始执行
				virtualDiskInside->Execute(command);
			}
			else
			{
				cout << "\n命令创建失败，请重新输入（多次失败请检查系统）：" << endl;
			}

			for (int i = 0; i < MAX_CMD_CHAR; i++)
			{
				cmd[i] = '\0';
			}
			flag = true;
		}

	}
	return;

}
