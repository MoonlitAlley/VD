#pragma once

#include "Command.h"

class ClsCmd : public  Command
{
public:
	bool Execute(VirtualDisk* virtualdisk);
};
