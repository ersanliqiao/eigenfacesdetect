#pragma once
#include <iostream>
#include <direct.h>
#include <vector>
#include <string>
#include <cassert>
#include <io.h>

using namespace std;

class CBrowseDir
{
protected:
	char m_szInitDir[_MAX_PATH];
public:
	CBrowseDir();
	//设置初始目录为dir，如果返回false，表示目录不可用
	bool SetInitDir(const char* dir);
	//开始遍历初始目录下由filespec指定后缀的文件
	bool BeginBrowse(const char* filespec);
	vector<string> BeginBrowseFilenames(const char* filespec);
protected:
	//遍历目录dir下的后缀为filespec的文件
	bool BrowseDir(const char* dir, const char* filespec);
	//找到指定的文件后进行处理
	virtual bool ProcessFile(const char* filename);
	//函数BrowseDir每进入一个目录，就调用ProcessDir
	//把正在处理的目录名与父目录名传过去
	//用户可以覆写该函数,加入自己的处理代码  
	//比如用户可以在这里统计子目录的个数  
	virtual bool ProcessDir(const char* curdir, const char* parentdir);
	vector<string> GetDirFilenames(const char* dir, const char* filespec);
};

class smaller
{
public:
	bool operator () (const std::string& lhs, const std::string&  rhs)
	{
		assert(!lhs.empty() && !rhs.empty());
		int lpos[2], rpos[2];
		lpos[0] = lhs.find_last_of('\\');
		rpos[0] = lhs.find('.');
		lpos[1] = rhs.find_last_of('\\');
		rpos[1] = rhs.find_last_of('.');
		std::string ltemp(lhs.begin() + lpos[0] + 1, lhs.begin() + rpos[0]);
		std::string rtemp(rhs.begin() + lpos[1] + 1, rhs.begin() + rpos[1]);
		int lnum = 0, rnum = 0;
		sscanf_s(ltemp.c_str(), "%d", &lnum);
		sscanf_s(rtemp.c_str(), "%d", &rnum);
		return (lnum < rnum);
	}
};