
#include <winsock2.h>
#include <stdio.h>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

struct dane{
	SOCKET si;
	char nick[80];

}dane_dla_klienta[5];

//vector<HANDLE> watki;
HANDLE watki[5];

DWORD WINAPI klient(void *arg);

int i = 0;

int main()
{
	DWORD id;
	

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
		si = accept(s, (struct sockaddr FAR *) &sc, &lenc);		if (si != INVALID_SOCKET) {			dane_dla_klienta[i].si = si;			watki[i] = CreateThread(NULL, 0, klient, (void*)&dane_dla_klienta[i], 0, &id);
			i++;		}	}

	
	WSACleanup();
	return 0;
}


DWORD WINAPI klient(void *arg) {
	
	dane *dane = (struct dane*)arg;
	int dlug;
	char buf[80];
	char buf_nick[80];
	char buff_with_nick[160]; 

	//wys�anie dost�pnych os�b
	for (int j = 0; j < i; j++) {
		if (dane_dla_klienta[j].si != dane->si)	{//inny ni� wysy�aj�cy
			strcpy(buf_nick, dane_dla_klienta[j].nick);
			dlug = strlen(buf_nick) + 1;
			buf_nick[dlug] = '\0';
			send(dane->si, buf_nick, dlug, 0);  //wys�anie do klient�w wiadomo�ci
		}
	}

	//podnieranie nicku
	char nick[] = "Podaj nick: \0";
	dlug = strlen(nick) + 1;
	buf[dlug] = '\0';
	send(dane->si, nick, dlug, 0);

	recv(dane->si, buf_nick, 80, 0);
	strcpy(dane->nick, buf_nick);

	while (recv(dane->si, buf, 80, 0) > 0)
	{
		if (strcmp(buf, "KONIEC") == 0)
		{
			closesocket(dane->si);
			WSACleanup();
			return 0;
		}
		//printf("\n%s: %s", dane->nick,buf);

		strcpy(buff_with_nick, buf_nick); // tablica char�w z samym nickiem
		strcat(buff_with_nick, ": ");		//dodoawanie do tablicy dodatkowego tekstu
		strcat(buff_with_nick, buf);
		printf("\n%s", buff_with_nick);

		dlug = strlen(buff_with_nick) + 1;
		buff_with_nick[dlug] = '\0';
		for (int j = 0; j < i; j++) {
			if (dane_dla_klienta[j].si != dane->si)	//inny ni� wysy�aj�cy
				send(dane_dla_klienta[j].si, buff_with_nick, dlug, 0);  //wys�anie do klient�w wiadomo�ci
		}


	}
	

	return 0;
}
