#include <iostream>
#include <ws2tcpip.h>
#include <string>
#pragma comment(lib, "ws2_32.lib") //tell compilder link to winsock library (32 bit file)

using namespace std; //bad boy

void main() 
{
	string ipAddress = "127.0.0.1";			//IP address of the server
	int port = 54000;						//liestening port # of the server

	//initialise winsock

	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "can't start winsock, err #" << wsResult << endl;
		return;
	}

	//create socket

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);			//socket is the identifier our program will use to communicate through the network with the remote machine
	if (sock == INVALID_SOCKET)
	{
		cerr << "can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	//fill in a hint structure
	//tell winsock which server and port we want to connect to

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);							//host to network short
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);	//c_str convert to c style string

	//connet to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "can't connect to server, err #" << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	//do-while loop to send and receive data

	char buf[4096];
	string userInput;

	do
	{
		//prompt the user for text
		cout << "> ";
		getline(cin, userInput); //links multiple words from the user

		if (userInput.size() > 0)
		{
			//send the text
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0); //+1 accounts for additional 0 charecter in string
			if (sendResult != SOCKET_ERROR)
			{
				//wait for response
				ZeroMemory(buf, 4096); //zeroes out buffer
				int bytesRecieved = recv(sock, buf, 4096, 0);
				if (bytesRecieved > 0)
				{
					//echo response to console
					cout << "SERVER> " << string(buf, 0, bytesRecieved) << endl; //outputing text SERVER>, a new string made up of the contects of buffer (buf) from the index (0) and bytesRecieved number of charecters
				}
			}				
		}		
	} while (userInput.size() > 0);

	//close everything
	closesocket(sock);
	WSACleanup();
}

