#define _CRT_SECURE_NO_WARNINGS
#include "VirtualDisk.h"
#include <io.h>
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
	virtualDiskInside->LogMsgToConsole("bool VirtualDisk::containNode : " + pathstr);

	Path path(pathstr);


	if (path.IsReal())
	{
		//����ʵ����·��

		_finddata_t findData;
		intptr_t fileHandle = _findfirst(path.str().c_str(), &findData);

		if (fileHandle == -1)
		{
			//����ʧ�ܣ�������·������
			size = -1;
			type = 0;
			return false;
		}
		if (findData.attrib &_A_ARCH)
		{
			//��һ���������ļ�
			//��ȡ��С
			CellNode* tempNode = new CellNode();

			FILE* fin = fopen(path.str().c_str(), "rb");
			if (!fin)
			{
				return false;
			}
			char buff[1024];
			//�����ļ���β�Ļ���һֱ������ȡ
			while (!feof(fin))
			{
				size_t count = fread(buff, 1, sizeof(buff), fin);
				tempNode->Content().insert(tempNode->Content().end(), buff, buff + count);
			}
			fclose(fin);

			size = tempNode->Content().size();
			type = 2;

			delete tempNode;
			tempNode = NULL;
			return true;
		}
		else if (findData.attrib & _A_SUBDIR)
		{
			size = 0;
			type = 1;
			return true;
		}

		size = -1;
		type = 0;
		return false;
	}



	
	
	CellNode* tempNode = NULL;

	//CellNode* tempNode = virtualDiskInside->GetNodeByPath(path);


	vector<string> item = path.split();
	CellNode* curNode = virtualDiskInside->GetNodeByPath(path.StartNode());
	curNode = virtualDiskInside->LookingForTaget(curNode);


	for (size_t i = 0; i < item.size(); i++)
	{

		tempNode = curNode->GetNode(item[i]);

		if (++i < item.size())
		{
			--i;
			tempNode = virtualDiskInside->LookingForTaget(tempNode);
		}

		if (!tempNode)
		{
			break;
		}
		else if (tempNode->GetNodeType()& FOLD)
		{
			curNode = tempNode;
		}
		else if(tempNode->GetNodeType()& FILE_CUSTOM)
		{

			//��һ���ļ�,�ж��Ƿ������սڵ�
			if (++i < item.size())
			{
				
				return false;
			}
			--i;
			break;
		}
	}

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

	size = -1;
	type = 0;
	return false;	
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
