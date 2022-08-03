#include "Main.h"
#if 0
UPOINT user_pos[USER_MAX_SIZE];
struct USER_ADDR_INFO
{
	SOCKET sock;
	SOCKADDR_IN sock_addr;
	int clntAdrSz;
};
struct USER_ADDR_INFO user_addr_info[USER_MAX_SIZE];
static UPOINT ptthisMypos;
static UPOINT ptthisMypos2;
int    timeflag = FALSE;
int    score, hiscore = 2000, killnum;
char* Aboom[8];

static void ErrorHandling(char*);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET servSock, clientSock;
	int clntAdrSz;
	SOCKADDR_IN servAdr, clntAdr;
	UPOINT        ptend;
	UPOINT temp_pos;
	int temp_pos_len;
	int i;

	int	loop = 1;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

#if 0
	servSock = socket(PF_INET, SOCK_DGRAM, 0);
	if (servSock == INVALID_SOCKET)
		ErrorHandling("UDP socket creation error");
#endif

	servSock = socket(PF_INET, SOCK_STREAM, 0);
	if (servSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	if (bind(servSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	if (listen(servSock, USER_MAX_SIZE) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	clntAdrSz = sizeof(clntAdr);

	for (i = 1; i < USER_MAX_SIZE; i++)
	{
		printf("%d\n", i);
		clientSock = accept(servSock, (SOCKADDR*)&clntAdr, &clntAdrSz);
		if (clientSock == -1)
			ErrorHandling("accept() error");
		else
			printf("Connected client %d\n", i);

		user_addr_info[i].sock_addr = clntAdr;
		user_addr_info[i].sock_addr = clntAdr;
		user_addr_info[i].clntAdrSz = clntAdrSz;
		
		send(clientSock, &user_addr_info[i+1], sizeof(struct USER_ADDR_INFO), 0);
#if 0
		while ((strLen = recv(clientSock, message, buf_size, 0)) != 0)
			send(clientSock, message, strLen, 0);
#endif
	}
	closesocket(clientSock);
	closesocket(servSock);
	WSACleanup();

#if 0
	while (1)
	{
		clntAdrSz = sizeof(clntAdr);
		//strLen = recvfrom(servSock, message, buf_size, 0, (SOCKADDR*)&clntAdr, &clntAdrSz);
		temp_pos_len = recvfrom(servSock, &temp_pos, sizeof(UPOINT), 0, (SOCKADDR*)&clntAdr, &clntAdrSz);
		//sendto(servSock, message, strLen, 0, (SOCKADDR*)&clntAdr, sizeof(clntAdr));
		printf("recvfrom temp_pos : %d %d\n", temp_pos.x, temp_pos.y);
		sendto(servSock, &temp_pos, temp_pos_len, 0, (SOCKADDR*)&clntAdr, sizeof(clntAdr));
	}
	closesocket(servSock);
	WSACleanup();
#endif
	//return 0;

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

	system("cls");

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

#if 0
				clntAdrSz = sizeof(clntAdr);
				strLen = recvfrom(servSock, message, buf_size, 0, (SOCKADDR*)&clntAdr, &clntAdrSz);
				sendto(servSock, message, strLen, 0, (SOCKADDR*)&clntAdr, sizeof(clntAdr));
#endif
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
#endif
