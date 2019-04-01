#include "VirtualDisk.h"

//VirtualDisk类(需要提供无参构造函数)
VirtualDisk::VirtualDisk()
{
	virtualDiskInside = new VirtualDiskInside();
	//初始化系统的命令
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
