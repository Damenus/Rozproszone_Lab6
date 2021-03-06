
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

struct dane{
	SOCKET s;
}dane_watku;

DWORD WINAPI wysylanie(void *arg);
DWORD WINAPI odbieranie(void *arg);

HANDLE wys, odb;

int main()
{
	
	DWORD id;

	SOCKET s;
	struct sockaddr_in sa;
	WSADATA wsas;
	WORD wersja;
	wersja = MAKEWORD(2, 0);
	WSAStartup(wersja, &wsas);
	s = socket(AF_INET, SOCK_STREAM, 0);
	memset((void *)(&sa), 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);
	sa.sin_addr.s_addr = inet_addr("127.0.0.1"); 

	int result;
	result = connect(s, (struct sockaddr FAR *) &sa, sizeof(sa));
	if (result == SOCKET_ERROR)
	{
		printf("\nB��d po��czenia!");
		return 0;
	}	else  if (result == 0) {
		dane_watku.s = s;
		wys = CreateThread(NULL, 0, wysylanie, (void*)&dane_watku, 0, &id);
		odb = CreateThread(NULL, 0, odbieranie, (void*)&dane_watku, 0, &id);
	}

	while (true) {
		for (;;);
	}

	
	closesocket(s);
	WSACleanup();

	return 0;
}

DWORD WINAPI wysylanie(void *arg) {

	dane *dane = (struct dane*)arg;
	int dlug;
	char buf[80];
	for (;;)
	{
		fgets(buf, 80, stdin);
		dlug = strlen(buf); buf[dlug - 1] = '\0';
		send(dane->s, buf, dlug, 0);
		if (strcmp(buf, "KONIEC") == 0)
			break;
	}

	return 0;
}

DWORD WINAPI odbieranie(void *arg) {

	dane *dane = (struct dane*)arg;
	char buf[80];
	while (recv(dane->s, buf, 80, 0) > 0)
	{
		if (strcmp(buf, "KONIEC") == 0)
		{
			closesocket(dane->s);
			WSACleanup();
			return 0;
		}
		printf("%s\n", buf);
	}
	return 0;
}
