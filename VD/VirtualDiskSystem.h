#pragma once

#include "CommandFactory.h"
#include "VirtualDiskInside.h"

//命令的最大长度
#define MAX_CMD_CHAR 100

class VirtualDiskSystem
{
private:
	char cmd[MAX_CMD_CHAR];
	Command* command;
	CommandFactory commandFactory;
	VirtualDiskInside* virtualDiskInside;

public:
	VirtualDiskSystem();
	~VirtualDiskSystem();

	//文件系统初始化
	bool InitSystem();

private:
	//系统运行
	void RunSystem();

};