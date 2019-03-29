#pragma once

#include "Command.h"
#include "Path.h"
class DirCmd : public Command
{
public:
	bool Execute(VirtualDiskInside* virtualdiskInside);

	//该命令需要显示文件数、文件夹数、文件大小、总大小、修改时间、文件类型
	void RecursionPrintItem(const Path& path);

	void CheckDirWithFiles(CellNode* node, const string & filesname);
	void PrintItem(const Path& path);
	string MakeOneOutput(CellNode* node, const string &name);
	string NumberToString(size_t num, size_t length);
	void oneFoldSummary();
	void CountSummary(const list<CellNode*> &nodes, size_t& files, size_t & dirs, size_t & sizes);

private:
	list<CellNode*> totalNode;
	list<CellNode*> oneFoldNode;

	struct ProcessedInfo
	{
		CellNode* node;
		string wildcard;
		friend bool operator<(const ProcessedInfo& arg1, const ProcessedInfo  & arg2)
		{
			if (arg1.node < arg2.node)
			{
				return true;
			}
			if (arg1.node > arg2.node)
			{
				return false;
			}
		}
	};
	//记录已经处理过的节点信息，防止循环输出
	set<ProcessedInfo> m_proCessed;
};
