#pragma once

#include "CmdParameters.h"
#include "CellNode.h"
using namespace std;

class VirtualDiskInside;
class Command
{
public:
	Command();
	~Command();
	CmdParameters cmdParaCollection;
public:
	virtual bool Execute(VirtualDiskInside* virtualdiskInside);
protected:
	VirtualDiskInside* m_VirtualDisk;

};
