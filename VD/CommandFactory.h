#pragma once
#include "Command.h"

using namespace std;

class CommandFactory
{
private:
	Command* command;
	string commandString;
	int commandType;

private:
	void CmdTypeAnalyse(string name);
	void SetCmdPathCount();
public:
	CommandFactory();
	~CommandFactory();
	Command* CreatCommand(string cmd);
};