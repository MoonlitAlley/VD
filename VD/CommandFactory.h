#pragma once
#include "Command.h"

using namespace std;

class CommandFactory
{
public:
	OutputMsg outputMsg;
	Command* command;
	string commandString;
	int commandType;
	string cmdPara;
private:
	int CmdTypeAnalyse(string name);
	void SetCmdPathCount();
public:
	CommandFactory();
	Command* CreatCommand(string cmd);
};