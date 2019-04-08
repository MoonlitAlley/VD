#pragma once

#include "Command.h"
#include "FileNode.h"
class CopyCmd : public Command
{
public:
	bool Execute(VirtualDiskInside* virtualdiskInside);

private:
	void Copy(Path& src, Path& dst, bool first);

	void CopyNodeToReal(Path&src, Path& dst);
	void CopyRealToNode(Path&src, CellNode* dst);

	CellNode* GetDstNodeByPath(const Path &to);

private:
	bool needAsk;
};