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

	//得到根节点
	CellNode* GetRootNode();
	void SetRootNode(CellNode*);

	CellNode* GetWorkingNode();
	void SetWorkingNode(CellNode*);

	//得到当前工作节点
	Path GetWorkingPath();

	//得到当前工作路径的字符表示
	string GetWorkingPathString();

	bool IfNodeBeUsing(CellNode* node);

};