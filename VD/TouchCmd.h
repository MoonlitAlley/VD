#pragma once

#include "Command.h"

class TouchCmd : public Command
{
public:
	bool Execute(VirtualDiskInside* virtualdiskInside);
private:
	CellNode* GetDstNodeByPath(const Path &to);
};