#pragma once

#include "Command.h"
#include "Path.h"
class RenCmd : public Command
{
public: 
	bool Execute(VirtualDisk* virtualdisk);
};