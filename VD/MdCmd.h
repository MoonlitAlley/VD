#pragma once

#include "Command.h"
#include "Common.h"
class MdCmd :public Command
{
public:
	bool Execute(VirtualDisk* virtualdisk);
};