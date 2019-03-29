#pragma once

#include "Command.h"

class CopyCmd : public Command
{
public:
	bool Execute(VirtualDiskInside* virtualdiskInside);
};