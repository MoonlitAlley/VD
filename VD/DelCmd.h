#pragma once

#include "Command.h"

class DelCmd :public Command
{
public:
	bool Execute(VirtualDisk* virtualdisk);
private:
	void DisPatchPathItem(CellNode* node, const string& wildcard);
	void RemoveFoldLinkFile(CellNode* fold, CellNode* deep, const string& files);
};