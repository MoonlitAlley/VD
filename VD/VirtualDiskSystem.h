#pragma once

#include "CommandFactory.h"
#include "VirtualDiskInside.h"

//�������󳤶�
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

	//�ļ�ϵͳ��ʼ��
	bool InitSystem();

private:
	//ϵͳ����
	void RunSystem();

};