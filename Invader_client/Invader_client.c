#include "Main_client.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <process.h> 

#define BUF_SIZE 100
#define NAME_SIZE 20

unsigned WINAPI SendMsg(void* arg);
unsigned WINAPI RecvMsg(void* arg);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

static SOCKET sock;
#if 0
int main(int argc, char* argv[])
{
	
}
#endif
#define USER_MAX_SIZE		4
UPOINT user_pos[USER_MAX_SIZE];
struct USER_ADDR_INFO
{
	SOCKET sock;
	SOCKADDR_IN sock_addr;
	int clntAdrSz;
};
struct USER_ADDR_INFO user_addr_info[USER_MAX_SIZE];
UPOINT ptthisMypos;
UPOINT ptthisMypos2;
int    timeflag = FALSE;
int    score, hiscore = 2000, killnum;
char* Aboom[8];

static void ErrorHandling(char*);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	//SOCKET sock;
	int clntAdrSz;
	SOCKADDR_IN servAdr, clntAdr;
	UPOINT        ptend;
	UPOINT temp_pos;
	UPOINT server_pos;
	int temp_pos_len;
	int recv_len;
	int recv_cnt;
	struct USER_ADDR_INFO tmp_user;
	//char message[sizeof(struct USER_ADDR_INFO)];
	char message[50];

	//SOCKET hSock;
	//SOCKADDR_IN servAdr;
	HANDLE hSndThread, hRcvThread;
#if 0
	if (argc != 4) {
		printf("Usage : %s <IP> <port> <name>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	sprintf(name, "[%s]", argv[3]);
	hSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));

	if (connect(hSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("connect() error");


	closesocket(hSock);
	WSACleanup();
	return 0;

#endif




	int	loop = 1;

	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

#if 0
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
		ErrorHandling("UDP socket creation error");
#endif
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		ErrorHandling("socket error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));

#if 0
	connect(sock, (SOCKADDR*)&servAdr, sizeof(servAdr));
#endif
	if (connect(sock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("connect() error");
	else
		puts("Connected.............");


	hSndThread =
		(HANDLE)_beginthreadex(NULL, 0, SendMsg, (void*)&sock, 0, NULL);
	hRcvThread =
		(HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)&sock, 0, NULL);

	WaitForSingleObject(hSndThread, INFINITE);
	WaitForSingleObject(hRcvThread, INFINITE);

	temp_pos.x = 100;
	temp_pos.y = 200;

#if 0
	while (1)
	{
		fputs("Insert message(q to quit): ", stdout);
		//fgets(message, sizeof(message), stdin);
		//if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
		//	break;

		//send(sock, message, strlen(message), 0);
		send(sock, &temp_pos, sizeof(UPOINT), 0);
		//strLen = recv(sock, message, sizeof(message) - 1, 0);
		temp_pos_len = recv(sock, &server_pos, sizeof(UPOINT), 0);
		//message[strLen] = 0;
		//printf("Message from server: %s", message);
		printf("Message from server: %d %d\n", server_pos.x, server_pos.y);
	}
	closesocket(sock);
	WSACleanup();
	//return 0;
#endif
#if 1
	while (0)
	{
		Sleep(1000);
		printf("hmm\n");
		//fputs("Insert message(q to quit, s to start): ", stdout);
		//fputs("input(r or s) : ", stdout);
		//fgets(message, sizeof(message), stdin);
		//if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
		//	break;
		//send(sock, message, strlen(message), 0);
		recv_cnt = recv(sock, message, sizeof(message) - 1, 0);
		//message[recv_cnt] = 0;
		//printf("message from server : %s\n", message);
#endif
#if 0
		//str_len = write(sock, message, strlen(message));
		recv_len = 0;
		//while (recv_len < str_len)
		while (recv_len < sizeof(struct USER_ADDR_INFO))
		{
			printf("hello2\n");
			//recv_cnt = read(sock, &message[recv_len], 50 - 1);
			recv_cnt = recv(sock, &message[recv_len], 50 - 1, 0);
			//recv_cnt = read(sock, &tmp_user, sizeof(struct USER_ADDR_INFO));
			//recv_cnt = recv(sock, &tmp_user, sizeof(struct USER_ADDR_INFO), 0);

			printf("hello3\n");
			if (recv_cnt == -1)
				ErrorHandling("read() error");
			recv_len += recv_cnt;
		}
		//message[recv_len] = 0;
		//tmp_user = (struct USER_ADDR_INFO)message;
		memcpy(&tmp_user, message, sizeof(struct USER_ADDR_INFO));
		//printf("Message from server: %d %d\n", tmp_user.sock, tmp_user.clntAdrSz);
		printf("hello\n");
#endif
#if 0
		send(sock, message, strlen(message), 0);
		strLen = recv(sock, message, BUF_SIZE - 1, 0);
		message[strLen] = 0;
		printf("Message from server: %s", message);
#endif
	}
	printf("end\n");
	//closesocket(sock);
	//WSACleanup();

	return 0;

	Aboom[0] = "i<^>i";
	Aboom[1] = "i(*)i";
	Aboom[2] = "(* *)";
	Aboom[3] = "(** **)";
	Aboom[4] = " (* *) ";
	Aboom[5] = "  (*)  ";
	Aboom[6] = "   *   ";
	Aboom[7] = "       ";
	ptend.x = 36;
	ptend.y = 12;

	while (loop)
	{
		DWORD         thisTickCount = GetTickCount();
		DWORD         bcount = thisTickCount;
		int           bp = 0;

		play();

		for (;;)
		{
			if (timeflag == FALSE)
			{
				thisTickCount = GetTickCount();

				if (thisTickCount - bcount > 100)
				{
					gotoxy(ptthisMypos);
					gotoxy(ptthisMypos2);
					printf("%s", Aboom[bp]);
					bp++;
					if (bp > 7)
						break;
					bcount = thisTickCount;
				}
			}
			else
				break;
		}

		gotoxy(ptend);
		printf("당신의 비행기는 파괴되었습니다.");
		ptend.y += 1;
		gotoxy(ptend);
		printf("다시 할까요? (y/n)\n");

		if (_getch() == 'y')
		{
			ClearScreen();
			bp = 0;
			killnum = 0;
			timeflag = 0;
			ptend.y = 12;
			loop = 1;
		}
		else
			loop = 0;
	}
}

void  play()
{
	static UPOINT ptMyoldpos;
	static UPOINT ptMyoldpos2;
	DWORD         gthisTickCount = GetTickCount();
	DWORD         gCount = gthisTickCount;
	DWORD         Count = gthisTickCount;
	DWORD         bulletcount = gthisTickCount;
	UPOINT        ptscore, pthi;
	int           juckspeed = 500;

	InitConsole();
	InitMyship();
	Initenemyship();

	ptthisMypos.x = ptMyoldpos.x = MYSHIP_BASE_POSX;
	ptthisMypos.y = ptMyoldpos.y = MYSHIP_BASE_POSY;

	ptthisMypos2.x = ptMyoldpos2.x = MYSHIP_BASE_POSX - 30;
	ptthisMypos2.y = ptMyoldpos2.y = MYSHIP_BASE_POSY;

	ptscore.x = 68;
	ptscore.y = 1;

	pthi.x = 2;
	pthi.y = 1;


	while (TRUE)
	{
		gthisTickCount = GetTickCount();

		if (_kbhit())
		{
			switch (_getch())
			{
			case 'a':
				if (gthisTickCount - bulletcount > 500)
				{
					MyBulletshot(ptthisMypos);
					MyBulletshot(ptthisMypos2);
					bulletcount = gthisTickCount;
				}
				break;
			case 'j':
				ptMyoldpos.x = ptthisMypos.x;
				ptMyoldpos2.x = ptthisMypos2.x;
				if (--ptthisMypos.x < 1)ptthisMypos.x = 1;
				if (--ptthisMypos2.x < 1)ptthisMypos2.x = 1;
				DrawMyship(&ptthisMypos, &ptMyoldpos);
				DrawMyship(&ptthisMypos2, &ptMyoldpos2);
				break;
			case 'l':
				ptMyoldpos.x = ptthisMypos.x;
				ptMyoldpos2.x = ptthisMypos2.x;
				if (++ptthisMypos.x > 75)ptthisMypos.x = 75;
				if (++ptthisMypos2.x > 75)ptthisMypos2.x = 75;
				DrawMyship(&ptthisMypos, &ptMyoldpos);
				DrawMyship(&ptthisMypos2, &ptMyoldpos2);
				break;
			}
		}

		if (gthisTickCount - Count > 150)
		{

			if (CheckMybullet(ptthisMypos) == 0)
			{
				if (score > 2000)
					hiscore = score;
				break;
			}
			if (CheckMybullet(ptthisMypos2) == 0)
			{
				if (score > 2000)
					hiscore = score;
				break;
			}

			CheckenemyBullet(enemyship);
			DrawMyBullet();
			DrawMyship(&ptthisMypos, &ptMyoldpos);
			DrawMyship(&ptthisMypos2, &ptMyoldpos2);
			gotoxy(ptscore);

			if (killnum < 40)
				printf("점수 : %d", score);
			else
			{
				timeflag = TRUE;
				break;
			}

			if (killnum > 20)
				juckspeed = 150;

			gotoxy(pthi);

			Count = gthisTickCount;
		}

		if (gthisTickCount - gCount > juckspeed)
		{
			Bulletshot();
			DrawBullet();
			CalenemyshipPos();
			Drawenemyship();
			if (Checkenemypos() == 1)
				break;
			gCount = gthisTickCount;
		}
	}

}

static void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

unsigned WINAPI SendMsg(void* arg)   // send thread main
{
	//SOCKET hSock = *((SOCKET*)arg);
	//char nameMsg[NAME_SIZE + BUF_SIZE];
	char message[100];
	while (1)
	{
#if 0
		printf(">> ");
		fgets(msg, BUF_SIZE, stdin);
		if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
		{
			closesocket(hSock);
			exit(0);
		}
		sprintf(nameMsg, "%s %s", name, msg);
		//send(hSock, nameMsg, strlen(nameMsg), 0);
#endif
		Sleep(500);
		fputs("Insert message(q to quit, s to start): ", stdout);
		//fputs("input(r or s) : ", stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			exit(1);
			//break;
		send(sock, message, strlen(message), 0);
	}
	return 0;
}

unsigned WINAPI RecvMsg(void* arg)   // read thread main
{
	char message[100];
	int recv_cnt;
	//message[recv_cnt] = 0;
	//printf("message from server : %s\n", message);
	//int hSock = *((SOCKET*)arg);
	//char nameMsg[NAME_SIZE + BUF_SIZE];
	//int strLen;
	UPOINT pos = { 60, 40 };
	while (1)
	{
#if 0
		strLen = recv(hSock, nameMsg, NAME_SIZE + BUF_SIZE - 1, 0);
		if (strLen == -1)
			return -1;
		nameMsg[strLen] = 0;
		fputs(nameMsg, stdout);
#endif
		recv_cnt = recv(sock, message, sizeof(message) - 1, 0);
		message[recv_cnt] = 0;
		//gotoxy(pos);
		printf("\n\nmessage from server : \n\n%s\n\n", message);
	}
	return 0;
}
