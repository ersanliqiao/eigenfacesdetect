#include "BrowDir.h"

CBrowseDir::CBrowseDir()
{
	if (getcwd(m_szInitDir, _MAX_PATH) == NULL)
	{
		cerr<<"Current dir is not correct!"<<endl;
		return;
	}
	int len = strlen(m_szInitDir);
	if (m_szInitDir[len - 1] != '\\')
	{
		strcat(m_szInitDir, "\\");
	}
}
bool CBrowseDir::SetInitDir(const char* dir)
{
	if (_fullpath(m_szInitDir, dir, _MAX_PATH) == NULL)
	{
		cerr<<"Not correct filepath!"<<endl;
		return false;
	}
	//判断目录是否存在
	if (_chdir(m_szInitDir) != 0)
	{
		cerr<<"current dir not exist!"<<endl;
		return false;
	}
	//如果目录的最后一个字母不是'\',则在最后加上一个'\'  
	int len = strlen(m_szInitDir);
	if (m_szInitDir[len - 1] != '\\')
		strcat(m_szInitDir, "\\");
	return true;
}

bool CBrowseDir::BeginBrowse(const char* filespec)
{
	ProcessDir(m_szInitDir, NULL);
	return BrowseDir(m_szInitDir, filespec);
}

vector<string> CBrowseDir::BeginBrowseFilenames(const char* filespec)
{
	ProcessDir(m_szInitDir, NULL);
	return GetDirFilenames(m_szInitDir, filespec);
}

vector<string> CBrowseDir::GetDirFilenames(const char* dir, const char* filespec)
{
	_chdir(dir);
	long hFile;
	_finddata_t findinfo;
	vector<string> filename_vec;
	filename_vec.clear();
	if ((hFile = _findfirst(filespec, &findinfo)) != -1)
	{
		do 
		{
			//判断是否为文件
			if (!(findinfo.attrib & _A_SUBDIR))
			{
				char filename[_MAX_PATH];
				strcpy(filename, dir);
				strcat(filename, findinfo.name);
				filename_vec.push_back(filename);
			}
		} while (_findnext(hFile, &findinfo) == 0);
		_findclose(hFile);
	}
	_chdir(dir);
	if ((hFile = _findfirst("*.*", &findinfo)) != -1)
	{
		do 
		{
			//判断是否子文件目录
			if ((findinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(findinfo.name, ".") != 0 && strcmp(findinfo.name, "..") != 0)
				{
					char subdir[_MAX_PATH];
					strcpy(subdir, dir);
					strcat(subdir, findinfo.name);
					strcat(subdir, "\\");
					ProcessDir(subdir, dir);
					//收集子文件目录下的符合后缀的文件
					vector<string> temp = GetDirFilenames(subdir, filespec);
					filename_vec.insert(filename_vec.end(), temp.begin(), temp.end());
				}
			}
		} while (_findnext(hFile, &findinfo) == 0);
		_findclose(hFile);
	}
	return filename_vec;
}
bool CBrowseDir::BrowseDir(const char* dir, const char* filespec)
{
	_chdir(dir);
	long hFile;
	_finddata_t findinfo;
	if ((hFile = _findfirst(filespec, &findinfo)) != NULL)
	{
		do 
		{
			if (!(findinfo.attrib & _A_SUBDIR))
			{
				char filename[_MAX_PATH];
				strcpy(filename, dir);
				strcat(filename, findinfo.name);
				cout<<"filename = "<<filename<<endl;
				if (!ProcessFile(filename))
					return false;
			}
		} while (_findnext(hFile, &findinfo) == 0);
		_findclose(hFile);
	}
	_chdir(dir);
	if ((hFile = _findfirst("*.*", &findinfo)) != -1)
	{
		do 
		{
			if ((findinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(findinfo.name, ".") != 0 && strcmp(findinfo.name, "..") != 0)
				{
					char subdir[_MAX_PATH];
					strcpy(subdir, dir);
					strcat(subdir, findinfo.name);
					strcat(subdir, "\\");
					ProcessDir(subdir, dir);
					if (!BrowseDir(subdir, filespec))
					    return false;
				}
			}
		} while (_findnext(hFile, &findinfo) == 0);
		_findclose(hFile);
	}
	return true;
}

bool CBrowseDir::ProcessFile(const char* filename)
{
	return true;
}

bool CBrowseDir::ProcessDir(const char* curdir, const char* parentdir)
{
	return true;
}