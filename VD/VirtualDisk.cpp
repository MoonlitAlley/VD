#include "VirtualDisk.h"

//VirtualDisk��(��Ҫ�ṩ�޲ι��캯��)
VirtualDisk::VirtualDisk()
{
	virtualDiskInside = new VirtualDiskInside();
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
bool VirtualDisk::ececuteCmd(string cmdStr)
{
	command = commandFactory.CreatCommand(cmdStr);
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
	return virtualDiskInside->GetWorkingPathString();
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
