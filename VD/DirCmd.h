#pragma once

#include "Command.h"
#include "Path.h"
class DirCmd : public Command
{
public:
	bool Execute(VirtualDiskInside* virtualdiskInside);

	//��������Ҫ��ʾ�ļ������ļ��������ļ���С���ܴ�С���޸�ʱ�䡢�ļ�����
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
	//��¼�Ѿ�������Ľڵ���Ϣ����ֹѭ�����
	set<ProcessedInfo> m_proCessed;
};
