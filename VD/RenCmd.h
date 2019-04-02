#pragma once

#include "Command.h"
#include "Path.h"
class RenCmd : public Command
{
public: 
	bool Execute(VirtualDiskInside* virtualdiskInside);

private:
	bool SetTargetCellName(CellNode * targetNode, string applyName);
};