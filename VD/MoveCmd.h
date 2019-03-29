#pragma once
#include "Command.h"

class MoveCmd : public Command
{
public:
	bool Execute(VirtualDiskInside* virtualdiskInside);
private:
	void MoveNode(CellNode* from, Path& to);
	bool conflict = false;
	bool needAsk = true;
};