#pragma once

#include "Command.h"

class MklinkCmd : public Command
{
public:
	bool Execute(VirtualDiskInside* virtualdiskInside);
};
