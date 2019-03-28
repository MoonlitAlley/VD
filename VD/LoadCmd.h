#pragma once

#include"Command.h"

class LoadCmd : public Command
{
public:
	bool Execute(VirtualDisk* virtualdisk);
};