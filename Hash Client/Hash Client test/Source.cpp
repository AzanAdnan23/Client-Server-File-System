#include <iostream>
#include <fstream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <conio.h>

#pragma comment(lib, "Ws2_32.lib")
using namespace std;

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	getaddrinfo("10.62.52.27", "543210", &hints, &result);

	SOCKET ssocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	connect(ssocket, result->ai_addr, (int)result->ai_addrlen);

	char buff[500];
	int bufflen = 500;
	int c;
	for (int j = 0;; j++)
	{
		cout << endl;
		cout << " [ Google Drive Sasta Version ]" << endl;
		cout << " Press 1 to Insert File" << endl;
		cout << " Press 2 to Delete file" << endl;
		cout << " Press 3 to Search File" << endl;
		cout << " Press 4 to Exit " << endl;
		cin >> c;

		if (c == 1)
		{ // Insert

			int count = 0;
			int count1 = 1;

			buff[0] = '1';

			cout << " Enter ID to Insert file      (press = when done) " << endl;

			for (int i = 1;; i++)
			{
				buff[i] = _getch();
				cout << buff[i];
				count1++;
				if (buff[i] == '=')
				{
					break;
				}
			}

			ifstream File;
			File.open("Geaan.txt", ios::binary);
			char temp;

			if (File.is_open())
			{
				cout << endl
					 << " File opened " << endl;
				int i = count1;
				buff[i] = '�';
				i++;
				while (File)
				{
					File.get(temp);
					buff[i] = temp;
					// cout << buff[i];
					count++;
					i++;
				}
			}
			else
			{
				cout << " File can't open or wrong file name " << endl;
			}
			File.close();

			buff[count + 1] = '�';

			send(ssocket, buff, bufflen, 0);
			cout << " File sended sucesfully" << endl;
		}
		else if (c == 2)
		{
			// Delete
			cout << " Enter ID to delete the file      (press = when done) " << endl;
			buff[0] = '2';
			int count = 0;
			for (int i = 1;; i++)
			{
				buff[i] = _getch();
				cout << buff[i];
				count++;
				if (buff[i] == '=')
				{
					break;
				}
			}
			// buff[count] = '.';
			send(ssocket, buff, bufflen, 0);
		}
		else if (c == 3)
		{
			// Search
			cout << " Enter ID to search file		(press = when done) " << endl;
			buff[0] = '3';
			int count = 0;
			for (int i = 1;; i++)
			{
				buff[i] = _getch();
				cout << buff[i];
				count++;
				if (buff[i] == '=')
				{
					break;
				}
			}
			send(ssocket, buff, bufflen, 0);

			char buffa[50];
			recv(ssocket, buffa, bufflen, 0);

			if (buffa[0] == '0')
			{
				cout << "file not avialble";
			}
			else if (buffa[0] == '1')
			{
				cout << endl
					 << " File found " << endl;
			}
		}
		else if (c == 4) // Exit
		{
			buff[0] = '4';
			send(ssocket, buff, bufflen, 0);
			cout << " Exit sucessfully " << endl;
			break;
		}
		else
		{
			cout << " wrong choice chuti karo " << endl;
		}
	}
	shutdown(ssocket, SD_SEND);
	closesocket(ssocket);
	WSACleanup();
}
