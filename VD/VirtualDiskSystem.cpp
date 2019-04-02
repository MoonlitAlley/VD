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
	//ִ���ļ�ϵͳ�ĳ�ʼ������
	if (!virtualDiskInside->InitFileSystem())
	{
		return false;
	}

	//��ʼ��ϵͳ������
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

		//��ȡ�û����� ����ʱ�����
		char cmd[MAX_CMD_CHAR];
		std::cin.getline(cmd, MAX_CMD_CHAR);

		if (cmd == "exit")
			break;

		//�ж��Ƿ����˳�ϵͳ��ָ��
			//������˳�ϵͳ����break������ǰѭ������

		//�����˳�ϵͳ����ʼ�������ִ��
		if (cmd[0] != '\0')
		{
			//��ʱ�����û�������
			//userInput.wholeCmd = cmd;

			//ʹ���������������
			string cmdtemp = cmd;
			if (cmdtemp == "cd..")
			{
				cmdtemp = "cd ..";
			}
			command = commandFactory.CreatCommand(cmdtemp);
			if (command != NULL)
			{
				//������ɹ�����ʼִ��
				virtualDiskInside->Execute(command);
			}
			else
			{
				cout << "\n�����ʧ�ܣ����������루���ʧ������ϵͳ����" << endl;
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
