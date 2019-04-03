#pragma once

#include "Command.h"

class RdCmd : public Command
{
public:
	bool Execute(VirtualDiskInside* virtualdiskInside);
private:
	CellNode* GetDstNodeByPath(const Path &to);
};