#include "Command.h"

Command::Command()
{
	m_VirtualDisk = NULL;
}

Command::~Command()
{
	m_VirtualDisk = NULL;
}

bool Command::Execute(VirtualDiskInside * virtualdisk)
{
	return false;
}