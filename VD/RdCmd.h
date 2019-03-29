#pragma once

#include "Command.h"

class RdCmd : public Command
{
public:
	bool Execute(VirtualDiskInside* virtualdiskInside);
};