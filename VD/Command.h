#pragma once

#include "CmdParameters.h"
#include "CellNode.h"
using namespace std;

class VirtualDiskInside;
class Command
{
public:
	CmdParameters cmdParaCollection;
public:
	virtual bool Execute(VirtualDiskInside* virtualdiskInside);
protected:
	VirtualDiskInside* m_VirtualDisk;
};
