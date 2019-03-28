#pragma once
#include "Command.h"

class MoveCmd : public Command
{
public:
	bool Execute(VirtualDisk* virtualdisk);
private:
	void MoveNode(CellNode* from, Path& to);
	bool conflict = false;
	bool needAsk = true;
};