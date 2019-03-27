#pragma once

#include "Command.h"

class VirtualDisk
{
public:

private:
	CellNode* rootNode;
	CellNode* workingNode;
	list<CellNode*> workingPath;
	string workingPathString;

	Command* cmdObject;



public:
	VirtualDisk();
	~VirtualDisk();

	bool InitFileSystem();

	void Execute(Command* cmd);

	CellNode* GetNodeByPath(const Path &path);

	CellNode* LookingTarget(CellNode* node);
	CellNode* LookingForTaget(CellNode * node);


	void LogMsgToConsole(string msg);

	//�õ����ڵ�
	CellNode* GetRootNode();
	void SetRootNode(CellNode*);

	CellNode* GetWorkingNode();
	void SetWorkingNode(CellNode*);

	//�õ���ǰ�����ڵ�
	Path GetWorkingPath();

	//�õ���ǰ����·�����ַ���ʾ
	string GetWorkingPathString();

	bool IfNodeBeUsing(CellNode* node);

};