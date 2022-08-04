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
//unsigned WINAPI Play(void* arg);

//char name[NAME_SIZE] = "[DEFAULT]";
//char msg[BUF_SIZE];

static SOCKET sock;
static HANDLE hSndThread, hRcvThread;//, hPlayThread;
static int gs_flag = 0;
static int gstart_sig = 0;
static int gloop = 1;
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

	//int	loop = 1;

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

	temp_pos.x = 100;
	temp_pos.y = 200;

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

	hSndThread =
		(HANDLE)_beginthreadex(NULL, 0, SendMsg, (void*)&sock, 0, NULL);
	hRcvThread =
		(HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)&sock, 0, NULL);

	//WaitForSingleObject(hSndThread, INFINITE);
	//WaitForSingleObject(hRcvThread, INFINITE);

	while (gloop)
	{
		if (gs_flag)
		{
			DWORD         thisTickCount = GetTickCount();
			DWORD         bcount = thisTickCount;
			int           bp = 0;

			system("cls");
			play();
			//hPlayThread = (HANDLE)_beginthreadex(NULL, 0, Play, (void*)&sock, 0, NULL);

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
				gloop = 1;
			}
			else
				gloop = 0;
		}
	}
	strcpy(message, "quitandclose");
	send(sock, message, strlen(message), 0);
	closesocket(sock);
	WSACleanup();
	return 0;
}

void  play()
{
#if 1
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
#endif
}

static void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

unsigned WINAPI SendMsg(void* arg)   // send thread main
{
	char message[100];
	int print_flag = 1;

	while (1)
	{
		if (!gs_flag)
		{
			if (print_flag)
			{
				print_flag = 0;
				fputs("Insert message(q to quit, s to start): ", stdout);
			}
			else if (gstart_sig)
			{
				gs_flag = 1;
			}
			else
			{
				if (kbhit())
				{
					print_flag = 1;
					//Sleep(500);
					//fputs("Insert message(q to quit, s to start): ", stdout);
					fgets(message, sizeof(message), stdin);

					if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
					{
						gloop = 0;
						break;
					}
					if (!strcmp(message, "s\n") || !strcmp(message, "S\n"))
					{
						strcpy(message, "startandclear");
					}
					send(sock, message, strlen(message), 0);
				}
			}
		}
		else
		{
			//Sleep(1000);
			//printf("hi\n");
		}
	}
	return 0;
}

unsigned WINAPI RecvMsg(void* arg)   // read thread main
{
	char message[100];
	int recv_cnt;

	while (1)
	{
		if (!gs_flag)
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

			if (strstr(message, "startandclear"))
			{
				//s_flag = 1;
				gstart_sig = 1;
			}
			else
			{
				printf("\nmessage from server : %s\n", message);
			}
			//gotoxy(pos);
			//printf("\nmessage from server : %s\n", message);
		}
		else
		{

		}
	}
	return 0;
}

#if 0
unsigned WINAPI Play(void* arg)
{
	play();
}
#endif
