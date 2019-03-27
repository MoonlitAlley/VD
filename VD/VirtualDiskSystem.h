#pragma once

#include "CommandFactory.h"
#include "VirtualDisk.h"

//�������󳤶�
#define MAX_CMD_CHAR 100

class VirtualDiskSystem
{
private:
	char cmd[MAX_CMD_CHAR];
	Command* command;
	CommandFactory commandFactory;
	VirtualDisk* virtualDisk;

public:
	VirtualDiskSystem();


	//�ļ�ϵͳ��ʼ��
	bool InitSystem();

private:
	//ϵͳ����
	void RunSystem();

};