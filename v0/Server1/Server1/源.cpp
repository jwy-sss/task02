#include "winsock2.h"
#include <WS2tcpip.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
using namespace std;
int main(int argc, char* argv[])
{
	const int BUF_SIZE = 64;
	WSADATA			wsd;			    //WSADATA����
	SOCKET			sServer;		    //�������׽���
	SOCKET			sClient;		    //�ͻ����׽���
	SOCKADDR_IN		servAddr;		    //��������ַ
	SOCKADDR_IN     clientAddr;         //�ͻ��˵�ַ
	int				nAddrLen = sizeof(clientAddr);
	char			bufSend[BUF_SIZE];	//�������ݻ�����
	char			bufRecv[BUF_SIZE];  //�������ݻ�����
	int				retVal;			    //����ֵ
	char*			closeSymbol = "0";  //����ͨ�ŵı�־
										// ������׽��ֵ�ַ 
	servAddr.sin_family = AF_INET;        //Э��
	servAddr.sin_port = htons(4999);      //�˿�
	inet_pton(AF_INET, "127.0.0.1", (void*)&servAddr.sin_addr.S_un.S_addr);
	// ��ʼ���׽��ֶ�̬��	
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		cout << "WSAStartup failed !" << endl;
		return 1;
	}
	// ����������׽���
	sServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == sServer)
	{
		cout << "socket failed!" << endl;
		WSACleanup();			 //�ͷ��׽�����Դ;
		return  -1;
	}
	else
	{
		cout << "�����͵�������'0'�����ʾ������˴�UDPͨ��\n";
		cout << "Server UDP Socket init!" << endl;		
		cout << "Server UDP Socket IP: 127.0.0.1" << endl;
		cout << "Server UDP Socket Port: 4999" << endl;
	}
	// �׽��ְ�IP�Ͷ˿�
	if (SOCKET_ERROR == bind(sServer, (LPSOCKADDR)&servAddr, sizeof(SOCKADDR_IN)))
	{
		cout << "bind failed!" << endl;
		closesocket(sServer);	//�رշ�����׽���
		WSACleanup();			//�ͷ��׽�����Դ;
		return -1;
	}
	else
	{
		cout << "Server UDP Socket bind IP & Port !" << endl;
	}
	// ѭ���ȴ������˿ڷ������ݣ��ӿͻ��˽������� & ��ͻ��˷�������
	while (true) {
		// ��ʼ������ռ�
		ZeroMemory(bufRecv, BUF_SIZE);
		// ���տͻ��˷��͵�buf��Ϣ
		retVal = recvfrom(sServer, bufRecv, BUF_SIZE, 0, (sockaddr *)&clientAddr, &nAddrLen);
		if (SOCKET_ERROR == retVal)
		{// ����ʧ����رշ���˿ͻ����׽���
			cout << "Recv Failed!" << endl;
			closesocket(sServer);	//�رշ�����׽���
			WSACleanup();			//�ͷ��׽�����Դ;
			return -1;
		}
		// ȷ�Ͽͻ��˷��͵���Ϣ

		bufRecv[retVal] = '\0';			// ���յ����һλ��Ϊ\0���������̵�����
		char str[INET_ADDRSTRLEN];
		cout << "Client IP��" << inet_ntop(AF_INET, &clientAddr.sin_addr, str, sizeof(str)) << endl;
		cout << "Data recv from Client UDP Socket: " << bufRecv << endl;
		// ���ͻ��˷��͵�������'0'�����ʾ�ͻ���������˴�UDPͨ��		
		if (!strcmp(bufRecv, closeSymbol))
		{
			cout << "Client UDP Socket wants to finish this communication" << endl;
			break;
		}
		// ��sendBuf��Ϣ���͵��ͻ���
		cout << "Data send to Client UDP Socket: ";
		cin >> bufSend;
		sendto(sServer, bufSend, strlen(bufSend), 0, (sockaddr *)&clientAddr, nAddrLen);
		// ������˷��͵�������'0'�����ʾ�����������˴�UDPͨ��	
		if (!strcmp(bufSend, closeSymbol))
		{
			cout << "Server UDP Socket wants to finish this communication" << endl;
		break;
		}
	}
	// �˳�
	closesocket(sServer);	//�رշ�����׽���
	WSACleanup();			//�ͷ��׽�����Դ;
	Sleep(5000);
	return 0;
}
