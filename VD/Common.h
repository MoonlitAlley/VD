#pragma once

#include <string>
#include <algorithm>
#include<regex>

using namespace std;

namespace Tools
{
	bool isWildcard(const string& pattern);

	bool IsWildcardMatch(const string& wildcard, const string &str);

	bool IsLegalPathName(const string & pathName);

	bool IsLegalFileName(const string& filename);
}


class OutputMsg
{
public:
	string Command_Illegal = "命令格式非法";
	string Path_IsNotExist = "路径不存在";
	string Path_IsAlreadyExist = "文件或文件夹已经存在";
	string QuotationMarks_NotMatch = "引号不匹配";
	string PathName_Illegal = "路径名非法";
	string Dir_BeingUse = "另一个程序正在使用此文件，进程无法访问";

	//string CommandCollection[13] = { "cls" ,"cd","save","load", "dir","touch","rd","del","md","ren","copy" ,"move" ,"mklink" };
};


//文件节点的类型
enum FileNodeType
{
	FILE_CUSTOM = 1,	//文件		为防止与系统关键字冲突，需要使用的地方请加上作用域操作符 ::

	FOLD = 2,	//文件夹
	
	LINK = 4,	//符号链接
};

//命令元素类型
enum CmdElementType
{
	CMD_NAME = 0,	//命令名称
	CMD_PARA = 1,	//命令参数
	CMD_PATH = 2,	//命令路径
};
//命令类型/命令ID
enum CommandID
{
	clsCmd = 0,	//不支持通配符	不支持参数

	cdCmd,		//不支持通配符	不支持参数
	saveCmd,	//不支持通配符	不支持参数
	loadCmd,	//不支持通配符	不支持参数

	dirCmd,			//			/s递归输出目录及其所有子目录下的文件 		/d 只输出其子目录
	touchCmd,	//不支持通配符	不支持参数
	rdCmd,		//不支持通配符	/s递归删除目录及其所有子目录下的文件 
	delCmd,			//			/s递归删除目录及其所有子目录下的文件 
	mdCmd,		//不支持通配符	不支持参数

	renCmd,		//不支持通配符	不支持参数
	copyCmd,		//第二条路径不支持通配符	/y遇到同名的文件或文件夹时执行覆盖
	moveCmd,		//第二条路径不支持通配符	/y遇到同名的文件或文件夹时执行覆盖
	mklinkCmd,	//不支持通配符	/d 创建目录的符号链接，，，不使用参数时不可对目录创建符号链接
};