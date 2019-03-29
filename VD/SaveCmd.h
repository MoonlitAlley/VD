#pragma once


#include "Command.h"

class SaveCmd : public Command
{
public:
	bool Execute(VirtualDiskInside* virtualdiskInside);
};