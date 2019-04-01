#pragma once

#include "Command.h"

class VirtualDiskInside
{
public:

private:
	CellNode* rootNode;
	CellNode* workingNode;
	list<CellNode*> workingPath;
	string workingPathString;

	Command* cmdObject;



public:
	VirtualDiskInside();
	~VirtualDiskInside();

	bool InitFileSystem();

	bool Execute(Command* cmd);

	CellNode* GetNodeByPath(const Path &path);

	CellNode* LookingTarget(CellNode* node);
	CellNode* LookingForTaget(CellNode * node);


	void LogMsgToConsole(string msg);
	string AskForUserInput(const string& output);

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