#include "Main_client.h"
UPOINT ptthisMypos;
UPOINT ptthisMypos2;
int    timeflag = FALSE;
int    score, hiscore = 2000, killnum;
char* Aboom[8];

static void ErrorHandling(char*);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET servSock;
	int clntAdrSz;
	SOCKADDR_IN servAdr, clntAdr;
	UPOINT        ptend;

	int	loop = 1;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	servSock = socket(PF_INET, SOCK_DGRAM, 0);
	if (servSock == INVALID_SOCKET)
		ErrorHandling("UDP socket creation error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	if (bind(servSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	while (1)
	{
		clntAdrSz = sizeof(clntAdr);
		// strLen = recvfrom(servSock, message, buf_size, 0, (SOCKADDR*)&clntAdr, &clntAdrSz);
		//sendto(servSock, message, strLen, 0, (SOCKADDR*)&clntAdr, sizeof(clntAdr));
	}
	closesocket(servSock);
	WSACleanup();

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
