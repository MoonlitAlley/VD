#define _CRT_SECURE_NO_WARNINGS
#include "VirtualDisk.h"
#include <io.h>
//VirtualDisk类(需要提供无参构造函数)
VirtualDisk::VirtualDisk()
{
	virtualDiskInside = new VirtualDiskInside();
}

VirtualDisk::~VirtualDisk()
{
}

//格式化虚拟磁盘
bool VirtualDisk::formatDisk()
{
	return virtualDiskInside->InitFileSystem();
}

//执行相应的命令
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
		//命令创建成功，开始执行
		return virtualDiskInside->Execute(command);
	}
	return false;
}

//获得当前文件夹所在路径
string VirtualDisk::getCurPath()
{
	//return virtualDiskInside->GetWorkingPathString();

	//NEW
	return virtualDiskInside->workingPath_CD.str();	
}
 

//如果path路径（绝对路径）的节点存在，则返回true，否则返回false。如果存在，size表示该节点的大小，type表示该节点的类型（1表示文件夹， 2表示文件， 3表示符号链接）；如果不存在，size为 - 1，type为0。
bool VirtualDisk::containNode(string pathstr, int & size, int & type)
{
	virtualDiskInside->LogMsgToConsole("bool VirtualDisk::containNode : " + pathstr);

	Path path(pathstr);


	if (path.IsReal())
	{
		//是真实磁盘路径

		_finddata_t findData;
		intptr_t fileHandle = _findfirst(path.str().c_str(), &findData);

		if (fileHandle == -1)
		{
			//查找失败，可能是路径出错
			size = -1;
			type = 0;
			return false;
		}
		if (findData.attrib &_A_ARCH)
		{
			//是一个正常的文件
			//读取大小
			CellNode* tempNode = new CellNode();

			FILE* fin = fopen(path.str().c_str(), "rb");
			if (!fin)
			{
				return false;
			}
			char buff[1024];
			//不到文件结尾的话就一直继续读取
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

			//是一个文件,判断是否是最终节点
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

//根据路径所指向的符号链接，返回该符号链接所链接的节点的路径（如果链接的节点不存在，返回空字符串）。
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
