#include "VirtualDiskSystem.h"
#include <iostream>
VirtualDiskSystem::VirtualDiskSystem()
{
	virtualDiskInside = new VirtualDiskInside();
}


bool VirtualDiskSystem::InitSystem()
{
	//ִ���ļ�ϵͳ�ĳ�ʼ������
	if (!virtualDiskInside->InitFileSystem())
	{
		return false;
	}

	//std::cout << "Program initialization completed!\n";
	RunSystem();
	return true;

}

void VirtualDiskSystem::RunSystem()
{
	bool flag = true;

	//��ʼ��ϵͳ������
	string initCmd = "md /\"bin\"";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));

	initCmd = "md /\"b in\"/st";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));



	std::cout << "Program initialization completed!\n";
	while (1)
	{

		if (flag)
		{
			//string workingPathString = virtualDisk.GetWorkingPathString();

			std::cout << virtualDiskInside->GetWorkingPathString() << "> ";
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
			command = commandFactory.CreatCommand(cmd);
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
