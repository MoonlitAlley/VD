#pragma once
#include "Command.h"
class CdCmd :public Command
{
public:
	bool Execute(VirtualDiskInside* virtualdiskInside);
};