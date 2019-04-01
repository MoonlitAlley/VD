#include "VirtualDisk.h"

//VirtualDisk��(��Ҫ�ṩ�޲ι��캯��)
VirtualDisk::VirtualDisk()
{
	virtualDiskInside = new VirtualDiskInside();
	//��ʼ��ϵͳ������
	string initCmd = "md /\"bin\"";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));

	initCmd = "md /\"b in\"/st";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));

	initCmd = "touch /\"b in\"/1.txt";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));
	initCmd = "touch /\"b in\"/11.txt";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));
	initCmd = "touch /\"b in\"/2.txt";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));



	initCmd = "mklink /d sym /\"b in\"";
	virtualDiskInside->Execute(commandFactory.CreatCommand(initCmd));
	initCmd = "mklink /d sym1 sym";
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
}

VirtualDisk::~VirtualDisk()
{
}

//��ʽ���������
bool VirtualDisk::formatDisk()
{
	return virtualDiskInside->InitFileSystem();
}

//ִ����Ӧ������
bool VirtualDisk::executeCmd(string cmdStr)
{
	string cmdtemp = cmdStr;
	if (cmdtemp == "cd..")
	{
		cmdtemp = "cd ..";
	}
	command = commandFactory.CreatCommand(cmdtemp);
	if (command != NULL)
	{
		//������ɹ�����ʼִ��
		return virtualDiskInside->Execute(command);
	}
	return false;
}

//��õ�ǰ�ļ�������·��
string VirtualDisk::getCurPath()
{
	//return virtualDiskInside->GetWorkingPathString();

	//NEW
	return virtualDiskInside->workingPath_CD.str();	
}


//���path·��������·�����Ľڵ���ڣ��򷵻�true�����򷵻�false��������ڣ�size��ʾ�ýڵ�Ĵ�С��type��ʾ�ýڵ�����ͣ�1��ʾ�ļ��У� 2��ʾ�ļ��� 3��ʾ�������ӣ�����������ڣ�sizeΪ - 1��typeΪ0��
bool VirtualDisk::containNode(string pathstr, int & size, int & type)
{
	Path tempPath(pathstr);
	CellNode* tempNode = virtualDiskInside->GetNodeByPath(tempPath);

	if (!tempNode)
	{
		size = -1;
		type = 0;
		return false;
	}
	else if (tempNode->GetNodeType()&FileNodeType::LINK)
	{
		size = 0;
		type = 3;
		return true;
	}
	else if (tempNode->GetNodeType()&FileNodeType::FOLD)
	{
		size = 0;
		type = 1;
		return true;
	}
	else if (tempNode->GetNodeType()&FileNodeType::FILE_CUSTOM)
	{
		size = tempNode->Content().size();
		type = 2;
		return true;
	}
	else
	{
		size = -1;
		type = 0;
		return false;
	}
}

//����·����ָ��ķ������ӣ����ظ÷������������ӵĽڵ��·����������ӵĽڵ㲻���ڣ����ؿ��ַ�������
string VirtualDisk::getLinkNode(string path)
{
	CellNode* tempNode = virtualDiskInside->GetNodeByPath(path);
	if (!tempNode)
	{
		return "";
	}
	else
	{
		tempNode = virtualDiskInside->LookingForTaget(tempNode);

		if (!tempNode)
		{
			return "";
		}
		else
		{
			return tempNode->GetNodePath().str();
		}
	}
}
