#pragma once

#include "Command.h"
#include "FoldNode.h"
#include "CellNode.h"

class VirtualDiskInside
{
public:
	//����-����
	VirtualDiskInside();
	~VirtualDiskInside();

	//��ʽ��-���ڵ㴴��
	bool InitFileSystem();

	bool Execute(Command* cmd);

	CellNode* GetNodeByPath(const Path &path);

	CellNode* LookingTarget(CellNode* node);
	CellNode* LookingForTaget(CellNode * node);

	//�û�����
	void LogMsgToConsole(const string& msg);
	string AskForUserInput(const string& output);


	//GET-SET
	//�õ����ڵ�
	CellNode* GetRootNode();
	void SetRootNode(FoldNode*);

	CellNode* GetWorkingNode();
	void SetWorkingNode(CellNode*);

	//�õ���ǰ�����ڵ�
	Path GetWorkingPath();
	//�õ���ǰ����·�����ַ���ʾ
	const string GetWorkingPathString();

	bool IfNodeBeUsing(CellNode* node);
	

public:
	//������ʾ
	Path workingPath_CD = Path("c:/");
private:

	FoldNode* rootNode;
	CellNode* workingNode;
	list<CellNode*> workingPath;
	string workingPathString;
};