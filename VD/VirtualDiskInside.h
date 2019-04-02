#pragma once

#include "Command.h"

class VirtualDiskInside
{
public:
	//构造-析构
	VirtualDiskInside();
	~VirtualDiskInside();

	//格式化-根节点创建
	bool InitFileSystem();

	bool Execute(Command* cmd);

	CellNode* GetNodeByPath(const Path &path);

	CellNode* LookingTarget(CellNode* node);
	CellNode* LookingForTaget(CellNode * node);

	//用户交互
	void LogMsgToConsole(string msg);
	string AskForUserInput(const string& output);


	//GET-SET
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
	

public:
	//用于显示
	Path workingPath_CD = Path("c:/");
private:

	CellNode* rootNode;
	CellNode* workingNode;
	list<CellNode*> workingPath;
	string workingPathString;
};