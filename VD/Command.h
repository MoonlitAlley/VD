#pragma once

#include "CmdParameters.h"
#include "CellNode.h"
using namespace std;

class VirtualDisk;
class Command
{
public:
	CmdParameters cmdParaCollection;
public:
	virtual bool Execute(VirtualDisk* virtualdisk);

private:
	VirtualDisk* virtualdisk;
};
