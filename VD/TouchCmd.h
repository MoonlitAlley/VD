#pragma once

#include "Command.h"

class TouchCmd : public Command
{
public:
	bool Execute(VirtualDisk* virtualdisk);
};