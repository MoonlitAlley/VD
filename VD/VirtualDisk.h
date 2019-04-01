#pragma once
#include <string>
#include "Path.h"
#include "VirtualDiskSystem.h"
#include "CommandFactory.h"
#include "VirtualDiskInside.h"
#include "Command.h"

class VirtualDisk
{
public:
	VirtualDisk();
	~VirtualDisk();

	bool formatDisk();

	bool executeCmd(string cmdStr);

	string getCurPath();

	bool containNode(string Path, int& size, int &type);

	string getLinkNode(string path);


private:
	Command* command;
	CommandFactory commandFactory;
	VirtualDiskInside* virtualDiskInside;
};