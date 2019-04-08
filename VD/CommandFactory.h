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
	void CmdTypeAnalyse(const string& cmdName);
	void SetCmdPathCount();
public:
	CommandFactory();
	~CommandFactory();
	Command* CreatCommand(const string& cmd);
};