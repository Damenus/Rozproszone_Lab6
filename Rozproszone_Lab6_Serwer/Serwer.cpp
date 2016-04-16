
#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

struct klient{
	char nick[10];
	HANDLE klient;
}klienci[5];

struct dane_dla_klienta{
	SOCKET si;

}dane_dla_klienta[5];

HANDLE watki[5];

DWORD WINAPI klient(void *arg);

int main()
{
	WSADATA wsas;
	int result;
	WORD wersja;
	wersja = MAKEWORD(1, 1);
	result = WSAStartup(wersja, &wsas);

	SOCKET s;
	s = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in sa;
	memset((void *)(&sa), 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);	sa.sin_addr.s_addr = inet_addr("127.0.0.1");
	result = bind(s, (struct sockaddr FAR*)&sa, sizeof(sa));

	result = listen(s, 5);

	SOCKET si;
	struct sockaddr_in sc;
	int lenc;
	for (;;)
	{
		lenc = sizeof(sc);
		si = accept(s, (struct sockaddr FAR *) &sc, &lenc);	}

	WSACleanup();
	return 0;
}

DWORD WINAPI klient(void *arg) {
	
	//SOCKET si = (SOCKET)arg->si;
	char buf[80];
	while (recv(si, buf, 80, 0) > 0)
	{
		if (strcmp(buf, "KONIEC") == 0)
		{
			//closesocket(si);
			WSACleanup();
			return 0;
		}
		printf("\n%s", buf);
	}
	

	return 0;
}
