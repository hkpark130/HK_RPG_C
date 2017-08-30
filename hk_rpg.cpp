
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#define false 0
#define true 1
#define color(i) SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), i);
#pragma warning(disable:4996)

typedef struct {
	char name[20];
	char atk[10];
	char def[10];
	char lev[10];
	char exp[4];
	char hp[10];
	char hp_max[10];
	char mp[10];
	char mp_max[10];
	char gold[7]; // NULL빼고 십만원 단위
	int hp_it;		// hp물약
	int mp_it;
	char def_it[20];
	char atk_it[20];
	int def_its;
	int atk_its;
	int x;
	int y;
	int dun;
	char duz[20];
	int que;
	bool qhk;
}character;

enum { HIDDEN, SHOW };

void CursorView(char show)
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}


character menu(int ch, character user, int*); //완성
void fight(character, character*, int); //완성
void skill(int*, int*, int*, int*, int*, char*, int*, character, int);	//완성
void item(int* c_h2, int* c_mp2, int* c_hpit2, int* c_mpit2, int*, int*);	// 완성 
void levelup(character*);	//완성
void store(character*);			//완성
void dragon(character*, int*); //완성
void get_items(int*, character*);
void dunmap(int, int*, char*);
void quest(character*);

void gotoxy(int x, int y)//x는 가로, y는 세로
{
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void main()
{
	CursorView(HIDDEN);
	character user = { "로드파일이 없음","10","3","1","0","100","100","50","50","0",3,3 };
	user.y = 1;
	int c_a = atoi(user.atk);
	int c_l = atoi(user.lev);
	int hk2 = true; //메뉴나오고 적 다시 출현
	int hk3 = true; //처음 적이 나오는 오류
	int hk4 = true; //던전선택후 적 다시 출력
	int hk5 = true; //싸우고 나서 적 그만출력
	int rx[5] = { 0,1 }, ry[5] = { 1,2 };//적 나오는 곳
	int ch = 0;
	int complete = false;
	int dungeon = 49;
	char dunge[20] = "소환사의 협곡";
	bool hk = false;	//적싸우고 난 후 다시 출현
	int choose_1 = 0;	//싸움 여부
	bool load = true;
	int choose_2 = 0;
	clock_t a = 0, b = 0;

	system("title ★HK  RPG★");
	gotoxy(28, 11);
	printf("처음부터 하시겠습니까?");
	gotoxy(28, 12);
	printf("1. 예\t2. 아니오\n");
	do {
		choose_1 = getch();
	} while (choose_1 != 49 && choose_1 != 50);
	if (choose_1 == 50) {
		ch = 50;
		hk3 = false;
	}
	else {
		user.que = 0;
		user.qhk = false;
		user.dun = dungeon;
		strcpy(user.duz, dunge);
		do {
			system("cls");
			gotoxy(28, 10);
			printf("이름을 입력해주세요.");
			gotoxy(20, 11);
			printf("(한글 5자,영문 10자 이내로 써주세요.)\n");
			gotoxy(30, 12);
			gets_s(user.name);
			system("cls");
		} while (strlen(user.name) >= 11);
		gotoxy(15, 10);
		printf("퀘스트를 시작하여 훈련용 아이템을 받으세요.");
		gotoxy(8, 12);
		printf("이동키는 방향키로 조작하며 다른 기능은 시작하시면 나와 있습니다.");
		gotoxy(18, 14);
		color(14)
			printf("계속하려면 아무 키나 누르십시오 . . .");
		getch();
		system("cls");
		color(7)
	}

	while (kbhit()) {
		getch(); 	//값을 읽어서 버퍼 지우기
	}

	srand((unsigned)time(NULL));
	system("cls");

	for (;;) {
		a = clock();
		if (atoi(user.exp) >= 100) {	//레벨업
			levelup(&user);
		}

		if (ch == 49 || ch == 50 || ch == 51 || ch == 52 || ch == 53 || ch == 54) {	//menu 함수
			if (ch == 50 && hk3) {
				do {
					system("cls");
					printf("LOAD 하시겠습니까?\n1. 예\t2. 아니오");
					choose_2 = getch();
				} while (choose_2 != 49 && choose_2 != 50);
				if (choose_2 == 50) {
					load = false;
				}
			}
			if (load) {
				user = menu(ch, user, &hk2);
			}
			load = true;
			system("cls");
			while (kbhit()) {
				getch(); 	//값을 읽어서 버퍼 지우기
			}
		}

		if (ch == 113) {	//던전
			system("cls");

			dunmap(atoi(user.lev), &dungeon, dunge);
			hk4 = false;
			system("cls");
			user.dun = dungeon;
			strcpy(user.duz, dunge);
		}

		if (!hk || !hk4) {				//몹 등장
			for (int i = 0; i<5; i++) {
				rx[i] = rand() % 75 + 1;
				ry[i] = rand() % 20 + 1;
				for (int j = 0; j<5; j++) {
					if (((rx[i] == rx[j]) || (ry[i] == ry[j])) && (i != j) && ((rx[i] == user.x) || (ry[i] == user.y))) {
						i--;
						break;
					}
				}
				gotoxy(rx[i], ry[i]);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				printf("x");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			}
			hk4 = true;
			hk = true;
		}

		if ((!hk2 && hk3&& hk5) || (!hk4&& hk3&& hk5)) {			//menu 함수로 사라진 적 다시 출현
			for (int i = 0; i<5; i++) {
				gotoxy(rx[i], ry[i]);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				printf("x");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			}
		}
		hk3 = true;
		hk5 = true;
		choose_1 = 0;
		gotoxy(5, 23);
		printf("1. 저장하기 2. 로드하기 3. 휴식하기 4. 정보보기 5. 상점가기 6. 종료");





		if ((((user.x == rx[0]) && (user.y == ry[0])) && (hk)) || (((user.x == rx[1]) && (user.y == ry[1])) && (hk)) || (((user.x == rx[2]) && (user.y == ry[2])) && (hk)) || (((user.x == rx[3]) && (user.y == ry[3])) && (hk)) || (((user.x == rx[4]) && (user.y == ry[4])) && (hk))) {
			hk = false;
			system("cls");
			gotoxy(22, 11);
			printf("적을 만났습니다. 어떻게 하시겠습니까?\n");
			gotoxy(28, 12);
			printf("1. 싸운다\t2. 도망친다");
			do {

				choose_1 = getch();

				if (choose_1 == 49) {
					fight(user, &user, user.dun);
					break;
				}
				else if (choose_1 == 50)
					break;

			} while (choose_1 != 2 && choose_1 != 1);


			while (kbhit()) {
				getch(); 	//값을 읽어서 버퍼 지우기
			}
			system("cls");
			hk5 = false;
			continue;
		}

		gotoxy(0, 0);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		printf("+");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf(" : %s", user.name);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		printf("\tx");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf(" : 적\tMAP.%s", user.duz);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		printf("\tq. 던전가기\tw. 퀘스트");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		gotoxy(user.x, user.y);		//변환 된 좌표에 출력
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		printf("+");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

		ch = getch();											//방향키,메뉴 등 입력 곳

		if (ch == 119) {
			system("cls");
			quest(&user);
			system("cls");
			hk4 = false;
		}

		b = clock();

		if (ch == 224)			//이상한 값 반환시
			ch = getch();		//다시 반환값 받고

		if (ch == 72 || ch == 75 || ch == 77 || ch == 80) {
			gotoxy(user.x, user.y);		//이전에 문자는 공백으로 지우고
			printf(" ");
		}

		if (ch == 72 && user.y>1) {//UP
			user.y = user.y - 1;

		}
		else if (ch == 80 && user.y<22) {//DOWN
			user.y = user.y + 1;

		}
		else if (ch == 75 && user.x>0) {//LEFT
			user.x = user.x - 1;

		}
		else if (ch == 77 && user.x<79) {//RIGHT
			user.x = user.x + 1;

		}
		//입력받는 시간이 짧으면 캐릭터 이동속도 항샹
		if ((b - a)<(50) && (ch == 72 || ch == 75 || ch == 77 || ch == 80)) {

			if (ch == 72 && user.y>1) {//UP
				user.y = user.y - 1;
			}
			else if (ch == 80 && user.y<22) {//DOWN
				user.y = user.y + 1;
			}
			else if (ch == 75 && user.x>0) {//LEFT
				user.x = user.x - 1;
			}
			else if (ch == 77 && user.x<79) {//RIGHT
				user.x = user.x + 1;
			}

		}


	}//for문

}//main 문
 //main 문

character menu(int ch, character user, int* ptr) {

	*ptr = true;
	FILE* fs = fopen("save2.txt", "r+b");
	if (fs == NULL) { //save2 파일이 없다면
		FILE* fs = fopen("save2.txt", "w+b");//save텍스트 파일 생성
	}


	if (ch == 49) { //save
		FILE* fs = fopen("save2.txt", "r+b");
		fwrite(&user, sizeof(user), 1, fs); //구조체 한번에 저장

		system("cls");
		printf("SAVE...");
		Sleep(1000);
		system("cls");
		fclose(fs);
		*ptr = false;
		return user;
	}
	else if (ch == 50) { //load	
		FILE* fs = fopen("save2.txt", "r+b");
		fread(&user, sizeof(user), 1, fs); //구조체 한번에 읽기

		system("cls");
		printf("LOAD...");
		Sleep(1000);
		system("cls");
		fclose(fs);
		*ptr = false;
		return user;
	}
	else if (ch == 51) { //rest
		FILE* fs = fopen("save2.txt", "r+b");
		system("cls");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		printf("hp");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("와 ");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		printf("mp");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("를 회복하기 위해 10초간 휴식중입니다...");

		strcpy(user.hp, user.hp_max);
		strcpy(user.mp, user.mp_max);
		Sleep(10000);
		system("cls");
		*ptr = false;
		return user;
	}
	else if (ch == 52) { //info	
		FILE* fs = fopen("save2.txt", "r+b");
		system("cls");
		//이름정보
		printf("NAME : %s\n", user.name);
		//공격정보
		printf("ATK : %s + %d\n", &user.atk[0], user.atk_its);
		//방어정보
		printf("DEF : %s + %d\n", user.def, user.def_its);
		//레벨정보
		printf("LEV : %s\n", user.lev);
		//경험정보
		printf("EXP : %s\n", user.exp);
		//체력정보
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		printf("HP : %s/%s\n", user.hp, user.hp_max);
		//마력정보
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		printf("MP : %s/%s\n", user.mp, user.mp_max);
		//돈 정보
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		printf("GOLD : %s\n", user.gold);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		//도구정보
		printf("HP물약 : %d MP물약 : %d\n", user.hp_it, user.mp_it);
		printf("\n");
		printf("장착 중인 아이템\n무기 : %s\t갑옷 : %s \n", user.atk_it, user.def_it);
		fclose(fs);
		gotoxy(0, 14);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		printf("★3번째 던전부터 레벨제한이 있습니다.\n★8레벨 부터 BOSS몹이 등장합니다.\n★5레벨 부터 새로운 스킬을 배우게 됩니다.\n★아이템은 한번 장착하면 전에 장착된 아이템은 다시 되살릴 수 없습니다.\n★적을 지나쳐 간다면 방향키 입력을 느리게 입력해주세요. \n\t(2칸씩 이동하기 때문입니다.)");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("\n\n");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		printf("\t\t제작자 : 1301127 컴정 1-J 박현경\n");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("계속하려면 아무 키나 누르십시오	. . .");
		*ptr = false;

		getch();
		return user;
	}
	else if (ch == 53) {
		system("cls");
		printf("좋은 물건 있으니깐 얼른 사보라구~\n");
		Sleep(1000);
		system("cls");

		store(&user);

		system("cls");
		*ptr = false;
		return user;
	}
	else if (ch == 54) {
		system("cls");
		printf("종료합니다.\n");
		exit(1);
	}



};
//menu 문

void store(character* user) {
	int ch = 0;
	srand(time((unsigned int)NULL));
	char def[][20] = { "천갑옷","철갑옷","가시갑옷","정령의 형상","군단의 방패","워모그의 갑옷" };
	char atk[][20] = { "롱소드","도란검","빌지워터","몰락한 왕의 검","굶주린 히드라","피바라기" };
	int r = rand() % 6;
	int sr = rand() % 6;
	int s = 0;	//물약 개수

	printf("1. %s\t(%d GOLD) DEF+%d\n", def[r], r * 1100 + 1100, (r + 1) * 25);
	printf("2. %s\t(%d GOLD) ATK+%d\n", atk[sr], sr * 1200 + 1200, (sr + 1) * 36);
	printf("3. HP포션\t(100 GOLD)\n");
	printf("4. MP포션\t(80 GOLD)\n");
	printf("5. ATK +1\t(500 GOLD)\n");
	printf("6. DEF +1\t(500 GOLD)\n");
	printf("7. HP_MAX +10\t(500 GOLD)\n");
	printf("8. MP_MAX +5 \t(500 GOLD)\n");
	printf("9. 나가기\n\n");
	printf("보유 GOLD : %s", user->gold);
	do {
		ch = getch();
	} while (ch != 49 && ch != 50 && ch != 51 && ch != 52 && ch != 53 && ch != 54 && ch != 55 && ch != 56 && ch != 57);
	if (ch == 49) {
		system("cls");
		if (atoi(user->gold)<r * 1100 + 1100) {
			printf("GOLD가 부족합니다");
			Sleep(1000);
			return;
		}
		printf("선택한 %s(을)를 장착했습니다.\n", def[r]);
		Sleep(1000);
		system("cls");
		printf("또 오십쇼~");
		Sleep(100);

		strcpy(user->def_it, def[r]);
		strcpy(user->gold, itoa(atoi(user->gold) - (r * 1100 + 1100), user->gold, 10));
		user->def_its = (r + 1) * 25;
		return;
	}
	if (ch == 50) {
		system("cls");
		if (atoi(user->gold)<sr * 1200 + 1200) {
			printf("GOLD가 부족합니다");
			Sleep(1000);
			return;
		}
		printf("선택한 %s(을)를 장착했습니다.\n", atk[sr]);
		Sleep(1000);
		system("cls");
		printf("또 오십쇼~");
		Sleep(100);
		strcpy(user->atk_it, atk[sr]);
		strcpy(user->gold, itoa(atoi(user->gold) - (sr * 1200 + 1200), user->gold, 10));
		user->atk_its = (sr + 1) * 36;
		return;
	}
	if (ch == 51) {
		system("cls");
		printf("몇 개 사실 건가요?");
		scanf("%d", &s);
		fflush(stdin);
		if (atoi(user->gold)<100 * s) {
			printf("GOLD가 부족합니다");
			Sleep(1000);
			return;
		}
		printf("HP 물약을 구입하였습니다.\n");
		Sleep(1000);
		system("cls");
		printf("또 오십쇼~");
		Sleep(100);
		user->hp_it = user->hp_it + s;
		strcpy(user->gold, itoa(atoi(user->gold) - 100 * s, user->gold, 10));
		return;
	}
	if (ch == 52) {
		system("cls");
		printf("몇 개 사실 건가요?");
		scanf("%d", &s);
		fflush(stdin);
		if (atoi(user->gold)<80 * s) {
			printf("GOLD가 부족합니다");
			Sleep(1000);
			return;
		}
		printf("MP 물약을 구입하였습니다.\n");
		Sleep(1000);
		system("cls");
		printf("또 오십쇼~");
		Sleep(100);
		user->mp_it = user->mp_it + s;
		strcpy(user->gold, itoa(atoi(user->gold) - 80 * s, user->gold, 10));
		return;
	}
	if (ch == 53) {
		system("cls");
		printf("몇 개 사실 건가요?");
		scanf("%d", &s);
		fflush(stdin);
		if (atoi(user->gold)<500 * s) {
			printf("GOLD가 부족합니다");
			Sleep(1000);
			return;
		}
		printf("ATK 스텟 +%d \n", s);
		Sleep(1000);
		system("cls");
		printf("또 오십쇼~");
		Sleep(100);
		strcpy(user->atk, itoa(atoi(user->atk) + s, user->atk, 10));
		strcpy(user->gold, itoa(atoi(user->gold) - 500 * s, user->gold, 10));
		return;
	}
	if (ch == 54) {
		system("cls");
		printf("몇 개 사실 건가요?");
		scanf("%d", &s);
		fflush(stdin);
		if (atoi(user->gold)<500 * s) {
			printf("GOLD가 부족합니다");
			Sleep(1000);
			return;
		}
		printf("DEF 스텟 +%d \n", s);
		Sleep(1000);
		system("cls");
		printf("또 오십쇼~");
		Sleep(100);
		strcpy(user->def, itoa(atoi(user->def) + s, user->def, 10));
		strcpy(user->gold, itoa(atoi(user->gold) - 500 * s, user->gold, 10));
		return;
	}
	if (ch == 55) {
		system("cls");
		printf("몇 개 사실 건가요?");
		scanf("%d", &s);
		fflush(stdin);
		if (atoi(user->gold)<500 * s) {
			printf("GOLD가 부족합니다");
			Sleep(1000);
			return;
		}
		printf("HP_MAX 스텟 +%d \n", s * 10);
		Sleep(1000);
		system("cls");
		printf("또 오십쇼~");
		Sleep(100);
		strcpy(user->hp_max, itoa(atoi(user->hp_max) + 10 * s, user->hp_max, 10));
		strcpy(user->gold, itoa(atoi(user->gold) - 500 * s, user->gold, 10));
		return;
	}
	if (ch == 56) {
		system("cls");
		printf("몇 개 사실 건가요?");
		scanf("%d", &s);
		fflush(stdin);
		if (atoi(user->gold)<500 * s) {
			printf("GOLD가 부족합니다");
			Sleep(1000);
			return;
		}
		printf("MP_MAX 스텟 +%d \n", s * 5);
		Sleep(1000);
		system("cls");
		printf("또 오십쇼~");
		Sleep(100);
		strcpy(user->mp_max, itoa(atoi(user->mp_max) + 5 * s, user->mp_max, 10));
		strcpy(user->gold, itoa(atoi(user->gold) - 500 * s, user->gold, 10));
		return;
	}
	if (ch == 57) {
		system("cls");
		printf("또 오십쇼~");
		Sleep(100);
		return;
	}

}
//store 문

void fight(character user, character* uptr, int d) {
	srand(time(NULL));
	int survival = 0;
	int c_a = atoi(user.atk) + user.atk_its;//유저 능력
	int c_d = atoi(user.def) + user.def_its;
	int c_l = atoi(user.lev);
	int c_e = atoi(user.exp);
	int c_h = atoi(user.hp);
	int c_hp_max = atoi(user.hp_max);
	int c_mp = atoi(user.mp);
	int c_mp_max = atoi(user.mp_max);
	int c_gold = 0;
	int c_hp_it = user.hp_it;
	int c_mp_it = user.mp_it;
	int get_item = rand() % 51;
	int dragon_skill = 0;
	int cho = 0;				//선택
	int b_l = 0;	//몹 레벨
	int b_h = 0;	//몹 체력
	int b_hp_max = 0;
	int b_a = 0;		//적에게 입은 피해
	int c_p = 0;		//적에게 가한 피해
	char bot[10][5][20] = { { "슬라임", "고블린", "오크", "트롤", "그린 드래곤" },{ "유성 트롤","소뿔전사","리치","하운드","레드 드래곤" },{ "웜", "설인", "호머드", "아이스퀸", "화이트 드래곤" },{ "고리전갈","고블린 대장","헌터","아곤","블랙 드래곤" },{ "스켈레톤","유충","싸이클롭스","고르곤","헬 드래곤" },{ "쉐도우","저주받은 법사","데쓰카우","발록","골드 드래곤" },{ "바하무트","발키리","리자드","히드라","스톰 드래곤" },{ "뮤턴트","블러디 울프","자이칸","탄탈로스","본 드래곤" },{ "퀸 레이너","드라칸","알파 크러스트","불사조","천상계 드래곤" },{ "이시스","로그 센츄리온","슈리커","바론","쿤둔의 환영" } };
	int bot_r = rand() % 5;
	int bonus = 0;
	int gold_bonus = 0;
	int baron = 0;
	int i = 1;

	for (i = 1; i<10; i++) {
		if (d == 48) {
			d = 9;
			break;
		}
		if (d == (48 + i)) {
			d = i - 1;
			break;
		}
	}
	b_l = rand() % (5) + d * 10 + bot_r + 1;

	b_h = rand() % ((d + 1) * 6) * 5 + 10 * (b_l)+d * 10;

	if (bot_r == 4 && c_l >= 8) {
		b_l = rand() % (10) + d * 10 + 11;
		b_h = rand() % ((d + 1) * 6) * 4 * d + 100 + 10 * (b_l);
	}
	else if (bot_r == 4) {
		system("cls");
		printf("BOSS몹은 8렙이후부터 나옵니다.");
		Sleep(500);
		bot_r = 0;
	}

	if (bot_r == 4 && d >= 9) {
		b_h = rand() % ((d + 1) * 6) * 5 + 10000 + 10 * (b_l);
		baron = rand() % b_l * 10 + 5000;
	}

	b_hp_max = b_h;

	do {
		survival = rand() % 2;
		system("cls");
		printf("%s\n", user.name);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		printf("lev : %d\n", c_l);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		printf(" hp : %d\n", c_h);

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		printf(" mp : %d", c_mp);

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);


		gotoxy(24, 0);
		printf("%s ", bot[d][bot_r]);
		gotoxy(24, 1);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		printf("lev : %d", b_l);
		gotoxy(24, 2);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		printf(" hp : %d", b_h);

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		if (bot_r == 4 && c_l >= 8) {
			gotoxy(24, 3);
			printf("★BOSS 몹임★");
		}

		gotoxy(1, 5);
		printf("1. 공격");
		gotoxy(1, 6);
		printf("2. 스킬");
		gotoxy(1, 7);
		printf("3. 도구");
		gotoxy(1, 8);
		printf("4. 도망");

		cho = getch();

		if (cho == 49) {

			if (d<1) {
				b_a = rand() % (b_l)* 2 + (bot_r)* 2 - (c_d);//피해
				c_p = rand() % (c_a)+(c_a) / 10;		//피격
				bonus = 30;
				gold_bonus = 40;
			}
			else if (d == 1) {
				b_a = rand() % (b_l)* 2 + 4 * d + (bot_r)* 4 - (c_d);
				c_p = rand() % (c_a + (c_a) / 10);		//피격
				bonus = 20;
				gold_bonus = 40;
			}
			else if (d <= 3) {
				b_a = rand() % (b_l)* 4 + 4 * d + (bot_r)* 5 - (c_d);
				c_p = rand() % (c_a)+(c_a) / 10;		//피격
				bonus = 20;
				gold_bonus = 40;
			}
			else if (d <= 5) {
				b_a = rand() % (b_l)* 6 + 4 * d + (bot_r)* 6 - (c_d);
				c_p = rand() % (c_a)+(c_a) / 10;		//피격
				bonus = 10;
				gold_bonus = 55;

			}
			else if (d <= 7) {
				b_a = rand() % (b_l)* 9 + 6 * d + (bot_r)* 8 - (c_d);
				c_p = rand() % (c_a)+(c_a) / 10;		//피격
				gold_bonus = 105;

			}
			else if (d == 8) {
				b_a = rand() % (b_l)* 10 + 7 * d + (bot_r)* 13 - (c_d);
				c_p = rand() % (c_a)+(c_a) / 10;		//피격
				gold_bonus = 500;

			}
			else if (d == 9) {
				b_a = rand() % (b_l)* 11 + 12 * d + (bot_r)* 20 - (c_d);
				c_p = rand() % (c_a)+(c_a) / 10;		//피격
				gold_bonus = 1000;
			}
			gotoxy(0, 9);
			dragon_skill = rand() % 51;
			printf("%s는 %s에게 %d의 피해를 주었다\n", user.name, bot[d][bot_r], c_p);
			if (bot_r == 4 && get_item % 5 == 0 && (dragon_skill % 5 == 0) && d <= 8) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				printf("%s은 breath를 시전하였다.\n", bot[d][bot_r]);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				if (b_a <= 0) {
					printf("%s의 breath를 회피!! \t\t\t\t\t\n", bot[d][bot_r]);
					b_a = 0;
				}
				else
					printf("%s은 %s에게 %d의 피해를 입혔다.\n", bot[d][bot_r], user.name, b_a * 3);

				b_a = b_a * 3;
				Sleep(200);
			}
			else {
				if (b_a <= 0) {
					printf("%s의 공격을 회피!! \t\t\t\t\t\n", bot[d][bot_r]);
					b_a = 0;
				}
				else
					printf("%s은 %s에게 %d의 피해를 입혔다.\n", bot[d][bot_r], user.name, b_a);
			}
			Sleep(1000);

			c_h = c_h - b_a;
			b_h = b_h - c_p;

			if (c_h <= 0) {
				system("cls");
				c_e = c_e - 10;
				printf("패배하셨습니다.\n");
				printf("경험치를 10 손실하여 현재 경험치 : %d\n", c_e);
				Sleep(1000);
				strcpy(uptr->exp, itoa(atoi(user.exp) - 10, user.exp, 10));
				strcpy(uptr->hp, "1");
				return;
			}
			else if (b_h <= 0) {
				system("cls");
				c_e = bonus + rand() % b_l + 2 - (c_l - b_l) * 5;
				if (c_e <= 0) {
					c_e = 0;
				}
				if (c_e >= 30) {
					c_e = c_e - 15;
				}
				c_gold = gold_bonus + baron + (rand() % b_l)*(4) + 20;
				if (bot_r == 4 && (get_item % 10 == 0)) {
					system("cls");
					dragon(uptr, &b_l);
					if (!uptr->qhk) {
						system("cls");
						printf("Quest를 완료했습니다.\n보상을 받으세요.");
						Sleep(1000);
						uptr->qhk = true;
					}
					system("cls");
				}
				else if (bot_r != 4 && (get_item % 11 == 1)) {
					system("cls");
					while (kbhit()) {
						getch(); 	//값을 읽어서 버퍼 지우기
					}
					get_items(&b_l, uptr);
					system("cls");
				}

				printf("승리하셨습니다.\n");
				printf("얻은 경험치 %d\n", c_e);
				printf("얻은 GOLD %d\n", c_gold);
				Sleep(1200);
				strcpy(uptr->gold, itoa(atoi(user.gold) + c_gold, user.gold, 10));
				strcpy(uptr->exp, itoa(atoi(user.exp) + c_e, user.exp, 10));
				strcpy(uptr->hp, itoa(c_h, user.hp, 10));
				return;
			}
		}
		else if (cho == 50) {			//스킬사용

			skill(&c_h, &c_mp, &b_h, &c_d, &c_l, bot[d][bot_r], &b_l, user, bot_r);

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			strcpy(uptr->mp, itoa(c_mp, user.mp, 10));

			if (c_h <= 0) {
				system("cls");

				c_e = c_e - 10;
				printf("패배하셨습니다.\n");
				printf("경험치를 10 손실하여 현재 경험치 : %d\n", c_e);
				Sleep(1000);
				strcpy(uptr->exp, itoa(atoi(user.exp) - 10, user.exp, 10));
				strcpy(uptr->hp, "1");
				return;
			}
			else if (b_h <= 0) {
				system("cls");
				c_e = bonus + rand() % b_l + 2 - (c_l - b_l) * 5;
				if (c_e <= 0) {
					c_e = 0;
				}
				if (bot_r == 4 && (get_item % 10 == 0)) {
					system("cls");
					dragon(uptr, &b_l);
					if (!uptr->qhk) {
						system("cls");
						printf("Quest를 완료했습니다.\n보상을 받으세요.");
						Sleep(1000);
						uptr->qhk = true;
					}
					system("cls");
				}
				else if (bot_r != 4 && (get_item % 11 == 1)) {
					system("cls");
					while (kbhit()) {
						getch(); 	//값을 읽어서 버퍼 지우기
					}
					get_items(&b_l, uptr);
					system("cls");
				}
				if (c_e >= 30) {
					c_e = c_e - 15;
				}
				c_gold = gold_bonus + baron + (rand() % b_l)*(4) + 20;
				printf("승리하셨습니다.\n");
				printf("얻은 경험치 %d\n", c_e);
				printf("얻은 GOLD %d\n", c_gold);
				Sleep(1200);
				strcpy(uptr->gold, itoa(atoi(user.gold) + c_gold, user.gold, 10));
				strcpy(uptr->exp, itoa(atoi(user.exp) + c_e, user.exp, 10));
				strcpy(uptr->hp, itoa(c_h, user.hp, 10));
				return;
			}


		}
		else if (cho == 51) {			//도구사용

			item(&c_h, &c_mp, &c_hp_it, &c_mp_it, &c_hp_max, &c_mp_max); //파이터 함수의 구조체 변수 주소값 넘김!!!
			uptr->hp_it = c_hp_it;
			uptr->mp_it = c_mp_it;

		}
		else if (cho == 52) {			//도망
			if (survival == 1) {
				system("cls");
				printf("도망가지 못했습니다.\n");
				b_a = rand() % (b_l)* 2 + d + (bot_r)* 2 - (c_d);
				if (b_a>0) {
					printf("%s은 %s에게 %d의 피해를 입혔다.\n", bot[d][bot_r], user.name, b_a);
				}
				else {
					printf("%s의 공격을 회피!! \t\t\t\t\t\n", bot[d][bot_r]);
				}
				c_h = c_h - b_a;
				Sleep(1100);
				if (c_h <= 0) {
					system("cls");
					c_e = c_e - 10;
					printf("패배하셨습니다.\n");
					printf("경험치를 10 손실하여 현재 경험치 : %d\n", c_e);
					Sleep(1000);
					strcpy(uptr->exp, itoa(atoi(user.exp) - 10, user.exp, 10));
					strcpy(uptr->hp, "1");
					return;
				}

			}
			else {
				system("cls");
				printf("도망가기~");
				Sleep(1000);
				strcpy(uptr->hp, itoa(c_h, user.hp, 10));
				return;
			}
		}

	} while (cho != 1);	//무한루프


};//fight 문
  //fight 문

void levelup(character* ulptr) {
	int ch = 0;
	int i = 0;
	system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	gotoxy(28, 12);
	printf("Level Up!!\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	Sleep(1000);
	system("cls");
	strcpy(ulptr->exp, "0");
	strcpy(ulptr->lev, itoa(atoi(ulptr->lev) + 1, ulptr->lev, 10));
	if (atoi(ulptr->lev) % 5 == 0 && atoi(ulptr->lev) < 31) {
		gotoxy(28, 12);
		printf("새로운 스킬이 생겼습니다.\n");
		Sleep(1000);
		system("cls");
	}
	for (i = 3; i != 0; i--) {
		while (kbhit()) {
			getch(); 	//값을 읽어서 버퍼 지우기
		}
		printf("스텟을 올리세요.\n1. ATK + 2\n2. DEF\n3. HP_MAX + 10\n4. MP_MAX + 5\n남은 포인트 : %d\n", i);
		ch = getch();
		system("cls");
		if (ch == 49) {
			strcpy(ulptr->atk, itoa(atoi(ulptr->atk) + 2, ulptr->atk, 10));
			printf("ATK + 2");
			Sleep(300);
			system("cls");
		}
		else if (ch == 50) {
			strcpy(ulptr->def, itoa(atoi(ulptr->def) + 1, ulptr->def, 10));
			printf("DEF + 1");
			Sleep(300);
			system("cls");
		}
		else if (ch == 51) {
			strcpy(ulptr->hp_max, itoa(atoi(ulptr->hp_max) + 10, ulptr->hp_max, 10));
			printf("HP_MAX + 10");
			Sleep(300);
			system("cls");
		}
		else if (ch == 52) {
			strcpy(ulptr->mp_max, itoa(atoi(ulptr->mp_max) + 5, ulptr->mp_max, 10));
			printf("MP_MAX + 5");
			Sleep(300);
			system("cls");
		}
		else {
			printf("값을 재대로 입력하세요.");
			Sleep(500);
			i++;
			system("cls");
		}
	}
	system("cls");
	strcpy(ulptr->hp, ulptr->hp_max);
	strcpy(ulptr->mp, ulptr->mp_max);

}//levelup 문
 //레벨업 문

void item(int* c_h2, int* c_mp2, int* c_hpit2, int* c_mpit2, int* c_hp_max2, int* c_mp_max2) {
	int ch = 0;
	system("cls");
	printf("1. hp 물약 %d개\n", *c_hpit2);
	printf("2. mp 물약 %d개\n", *c_mpit2);
	printf("3. 나가기\n");
	ch = getch();
	if (ch == 49 && *c_hpit2>0) {
		*c_h2 = *c_h2 + 100;
		if (*c_h2 > * c_hp_max2) {
			printf("최대 hp는 %d 입니다.\n", *c_hp_max2);
			*c_h2 = *c_hp_max2;
		}
		gotoxy(0, 9);
		printf("나는 100의 체력을 회복했다\n");
		*c_hpit2 = *c_hpit2 - 1;
		Sleep(1000);
		return;
	}
	else if (ch == 50 && *c_mpit2>0) {
		*c_mp2 = *c_mp2 + 50;
		if (*c_mp2 > * c_mp_max2) {
			printf("최대 mp는 %d 입니다.\n", *c_mp_max2);
			*c_mp2 = *c_mp_max2;
		}
		gotoxy(0, 9);
		printf("나는 50의 마력을 회복했다\n");
		*c_mpit2 = *c_mpit2 - 1;
		Sleep(1000);
		return;
	}
	else if (ch == 51) {
		return;
	}
	else {
		gotoxy(0, 9);
		printf("값을 잘못 입력하셨거나 물약이 부족합니다.\n");
		Sleep(1000);
		return;
	}
}
//item 문

void skill(int* c_h2, int* c_mp2, int* b_h2, int* c_d2, int* c_l2, char* mob, int* b_l2, character user, int bot_r) {
	// c_h2 내 체력 | c_mp2 내 마나 | b_h2 적 체력 | c_d2 내 방어 | c_l2 내 레벨
	int ch = 0;
	int i = 10;
	int j = 0;
	srand(time(NULL));
	int dmg = rand() % (*b_l2) * 6 - (*c_d2); //일부러 스킬 쓸때는 데미지가 더 따르게 만듬
	system("cls");
	int dragon_skill = rand() % 51;
	if (*c_l2 <= 4) {
		printf("레벨이 낮아 사용하실 수 있는 스킬이 없습니다.\n레벨이 5이상은 되어야 합니다.");
		Sleep(1000);
		return;
	}

	if (*c_l2 >= 5) {
		printf("1. 공허의 가시 (Lv 계수 데미지) (MP 20 소모)\n");
	}
	if (*c_l2 >= 10) {
		printf("2. 우주류 검술 (Lv 계수 데미지) (MP 50 소모)\n");
	}
	if (*c_l2 >= 15) {
		printf("3. 선고 (Lv 계수 데미지) (MP 70 소모)\n");
	}
	if (*c_l2 >= 20) {
		printf("4. 이쿠!! (ATK 계수 데미지) (MP 90 소모)\n");
	}
	if (*c_l2 >= 25) {
		printf("5. 진혼곡 (Lv+ATK 계수 데미지) (MP 110 소모)\n");
	}
	if (*c_l2 >= 30) {
		printf("6. 데마시아 (Lv+ATK 계수 데미지) (MP 200 소모)\n");
	}
	ch = getch();

	if (ch == 49) {
		if ((*c_mp2) >= 20 && (*c_l2) >= 5) {
			system("cls");
			for (j = 0; j<4; i++) {
				if (i == 16) {
					j++;
					i = 10;
				}
				gotoxy(24, 12);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
				printf("공허의 가시");
				Sleep(80);
			}
			*c_mp2 = *(c_mp2)-20;
			gotoxy(0, 9);
			printf("%s는 %s에게 %d의 피해를 주었다\n", user.name, mob, 10 * (*c_l2));
			*b_h2 = *b_h2 - 10 * (*c_l2);
			if (bot_r == 4 && dragon_skill % 5 == 0 && *c_l2<80) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				printf("%s은 breath를 시전하였다.\n", mob);
				if (dmg <= 0) {
					printf("%s의 breath를 회피!! \t\t\t\t\t\n", mob);
					dmg = 0;
				}
				else
					printf("%s은 %s에게 %d의 피해를 입혔다.\n", mob, user.name, dmg * 3);

				dmg = dmg * 3;
				*c_h2 = *c_h2 - dmg;
				Sleep(1000);
			}
			else {
				if (dmg <= 0) {
					printf("%s의 공격을 회피!! \t\t\t\t\t\n", mob);
					Sleep(1000);
				}
				else {
					printf("%s은 %s에게 %d의 피해를 입혔다.\n", mob, user.name, dmg);
					*c_h2 = *c_h2 - dmg;
					Sleep(1000);
				}
			}
			return;
		}
		else {
			system("cls");
			printf("마나가 부족하거나 레벨이 안 됩니다.");
			Sleep(1000);
			return;
		}
	}
	else if (ch == 50) {
		if (ch == 50 && *c_mp2 >= 50 && *c_l2 >= 10) {
			system("cls");
			for (j = 0; j<4; i++) {
				if (i == 16) {
					j++;
					i = 10;
				}
				gotoxy(24, 12);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
				printf("우주류 검술");
				Sleep(80);
			}
			*c_mp2 = *c_mp2 - 50;
			gotoxy(0, 9);
			printf("%s는 %s에게 %d의 피해를 주었다\n", user.name, mob, 15 * (*c_l2));
			*b_h2 = *b_h2 - 15 * (*c_l2);
			if (bot_r == 4 && dragon_skill % 5 == 0 && *c_l2<80) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				printf("%s은 breath를 시전하였다.\n", mob);
				if (dmg <= 0) {
					printf("%s의 breath를 회피!! \t\t\t\t\t\n", mob);
					dmg = 0;
				}
				else
					printf("%s은 %s에게 %d의 피해를 입혔다.\n", mob, user.name, dmg * 3);
				dmg = dmg * 3;
				*c_h2 = *c_h2 - dmg;
				Sleep(1000);
			}
			else {
				if (dmg <= 0) {
					printf("%s의 공격을 회피!! \t\t\t\t\t\n", mob);
					Sleep(1000);
				}
				else {
					printf("%s은 %s에게 %d의 피해를 입혔다.\n", mob, user.name, dmg);
					*c_h2 = *c_h2 - dmg;
					Sleep(1000);
				}
			}
			return;
		}
		else {
			system("cls");
			printf("마나가 부족하거나 레벨이 안 됩니다.");
			Sleep(1000);
			return;
		}
	}
	else if (ch == 51) {
		if (ch == 51 && (*c_mp2) >= 70 && (*c_l2) >= 15) {
			system("cls");
			for (j = 0; j<4; i++) {
				if (i == 16) {
					j++;
					i = 10;
				}
				gotoxy(24, 12);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
				printf("선고");
				Sleep(80);
			}
			*c_mp2 = *c_mp2 - 70;
			gotoxy(0, 9);
			printf("%s는 %s에게 %d의 피해를 주었다\n", user.name, mob, 20 * (*c_l2));
			*b_h2 = *b_h2 - 20 * (*c_l2);
			if (bot_r == 4 && dragon_skill % 5 == 0 && *c_l2<80) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				printf("%s은 breath를 시전하였다.\n", mob);
				if (dmg <= 0) {
					printf("%s의 breath를 회피!! \t\t\t\t\t\n", mob);
					dmg = 0;
				}
				else
					printf("%s은 %s에게 %d의 피해를 입혔다.\n", mob, user.name, dmg * 4);
				dmg = dmg * 4;
				*c_h2 = *c_h2 - dmg;
				Sleep(1000);
			}
			else {
				if (dmg <= 0) {
					printf("%s의 공격을 회피!! \t\t\t\t\t\n", mob);
					Sleep(1000);
				}
				else {
					printf("%s은 %s에게 %d의 피해를 입혔다.\n", mob, user.name, dmg);
					*c_h2 = *c_h2 - dmg;
					Sleep(1000);
				}
			}
			return;
		}
		else {
			system("cls");
			printf("마나가 부족하거나 레벨이 안 됩니다.");
			Sleep(1000);
			return;
		}
	}
	else if (ch == 52) {
		if ((*c_mp2) >= 90 && (*c_l2) >= 20) {
			system("cls");
			for (j = 0; j<4; i++) {
				if (i == 16) {
					j++;
					i = 10;
				}
				gotoxy(24, 12);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
				printf("이쿠!! (딜러 택배 갑니다.) ");
				Sleep(80);
			}
			*c_mp2 = *c_mp2 - 90;
			gotoxy(0, 9);
			printf("%s는 %s에게 %d의 피해를 주었다\n", user.name, mob, 30 * (atoi(user.atk)));
			*b_h2 = *b_h2 - 30 * (atoi(user.atk));
			if (bot_r == 4 && dragon_skill % 5 == 0 && *c_l2<80) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				printf("%s은 breath를 시전하였다.\n", mob);
				if (dmg <= 0) {
					printf("%s의 breath를 회피!! \t\t\t\t\t\n", mob);
					dmg = 0;
				}
				else
					printf("%s은 %s에게 %d의 피해를 입혔다.\n", mob, user.name, dmg * 5);
				dmg = dmg * 5;
				*c_h2 = *c_h2 - dmg;
				Sleep(1000);
			}
			else {
				if (dmg <= 0) {
					printf("%s의 공격을 회피!! \t\t\t\t\t\n", mob);
					Sleep(1000);
				}
				else {
					printf("%s은 %s에게 %d의 피해를 입혔다.\n", mob, user.name, dmg);
					*c_h2 = *c_h2 - dmg;
					Sleep(1000);
				}
			}
			return;
		}
		else {
			system("cls");
			printf("마나가 부족하거나 레벨이 안 됩니다.");
			Sleep(1000);
			return;
		}
	}
	else if (ch == 53) {
		if (ch == 53 && (*c_mp2) >= 110 && (*c_l2) >= 25) {
			system("cls");
			for (j = 0; j<4; i++) {
				if (i == 16) {
					j++;
					i = 10;
				}
				gotoxy(24, 12);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
				printf("진혼곡 (가게 두어라 궁 쓸테니..) ");
				Sleep(80);
			}
			*c_mp2 = *c_mp2 - 110;
			gotoxy(0, 9);
			printf("%s는 %s에게 %d의 피해를 주었다\n", user.name, mob, 40 * (*c_l2) + atoi(user.atk));
			*b_h2 = *b_h2 - (40 * (*c_l2) + atoi(user.atk));
			if (bot_r == 4 && dragon_skill % 5 == 0 && *c_l2<80) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				printf("%s은 breath를 시전하였다.\n", mob);
				if (dmg <= 0) {
					printf("%s의 breath를 회피!! \t\t\t\t\t\n", mob);
					dmg = 0;
				}
				else
					printf("%s은 %s에게 %d의 피해를 입혔다.\n", mob, user.name, dmg * 6);
				dmg = dmg * 6;
				*c_h2 = *c_h2 - dmg;
				Sleep(1000);
			}
			else {
				if (dmg <= 0) {
					printf("%s의 공격을 회피!! \t\t\t\t\t\n", mob);
					Sleep(1000);
				}
				else {
					printf("%s은 %s에게 %d의 피해를 입혔다.\n", mob, user.name, dmg);
					*c_h2 = *c_h2 - dmg;
					Sleep(1000);
				}
			}
			return;
		}
		else {
			system("cls");
			printf("마나가 부족하거나 레벨이 안 됩니다.");
			Sleep(1000);
			return;
		}
	}
	else if (ch == 54) {
		if (ch == 54 && (*c_mp2) >= 200 && (*c_l2) >= 30) {
			system("cls");
			for (j = 0; j<4; i++) {
				if (i == 16) {
					j++;
					i = 10;
				}
				gotoxy(24, 12);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
				printf("데마시아!!");
				Sleep(80);
			}
			*c_mp2 = *c_mp2 - 200;
			gotoxy(0, 9);
			printf("%s는 %s에게 %d의 피해를 주었다\n", user.name, mob, 70 * (*c_l2) + 2 * atoi(user.atk));
			*b_h2 = *b_h2 - (70 * (*c_l2) + 2 * atoi(user.atk));
			if (bot_r == 4 && dragon_skill % 5 == 0 && *c_l2<80) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				printf("%s은 breath를 시전하였다.\n", mob);
				if (dmg <= 0) {
					printf("%s의 breath를 회피!! \t\t\t\t\t\n", mob);
					dmg = 0;
				}
				else
					printf("%s은 %s에게 %d의 피해를 입혔다.\n", mob, user.name, dmg * 8);
				dmg = dmg * 8;
				*c_h2 = *c_h2 - dmg;
				Sleep(1000);
			}
			else {
				if (dmg <= 0) {
					printf("%s의 공격을 회피!! \t\t\t\t\t\n", mob);
					Sleep(1000);
				}
				else {
					printf("%s은 %s에게 %d의 피해를 입혔다.\n", mob, user.name, dmg);
					*c_h2 = *c_h2 - dmg;
					Sleep(1000);
				}
			}
			return;
		}
		else {
			system("cls");
			printf("마나가 부족하거나 레벨이 안 됩니다.");
			Sleep(1000);
			return;
		}
	}
	else {
		system("cls");
		printf("값을 재대로 입력하지 못합니까?!!");
		Sleep(1000);
		return;
	}

}
//skill 문

void dragon(character* user, int* d_l) {
	int ch = 0;
	while (kbhit()) {
		getch(); 	//값을 읽어서 버퍼 지우기
	}
	int n = (*d_l / 10) - 1; //드래곤 레벨
	char dr[][20] = { "드래곤비늘","드래곤이빨","드래곤하트","드래곤 소울" };
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	printf(" '%s' 이란 아이템을 획득 하셧습니다. \n장착하시겟습니까?\n\n1. 예 2. 아니오\n\n", dr[n]);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	if (n == 0) {
		printf("DEF : %d \t 지금 장착중인 방어구 DEF : %d\n", (n + 1) * 35, user->def_its);
	}
	else if (n == 1 || n == 2) {
		printf("ATK : %d \t 지금 장착중인 무기 ATK : %d \n", (n + 1) * 45, user->atk_its);
	}
	else {
		printf("(방어구와 중복착용 가능)\n(★ 무기와 중복 착용 불가)");
		printf("ATK : %d \n", (n + 1) * 40 + 5 * (*d_l));
		printf("DEF : %d \n", (n + 1) * 10 + 5 * (*d_l));
		printf("HP_MAX : %d \n", (n + 1) * 50 + 5 * (*d_l));
		printf("MP_MAX : %d \n", (n + 1) * 51 + 5 * (*d_l));
	}

	Sleep(1500);
	while (kbhit()) {
		getch(); 	//값을 읽어서 버퍼 지우기
	}

	ch = getch();
	if (ch == 49) {
		system("cls");
		printf("아이템을 장착했습니다.");
		Sleep(1000);
		if (n == 0) {
			strcpy(user->def_it, "드래곤비늘");
			user->def_its = (n + 1) * 35;
		}
		else if (n == 1) {
			strcpy(user->atk_it, "드래곤이빨");
			user->atk_its = (n + 1) * 45;
		}
		else if (n == 2) {
			strcpy(user->atk_it, "드래곤하트");
			user->atk_its = (n + 1) * 45;
		}
		else if (n >= 3) {
			strcpy(user->atk_it, "드래곤 소울");
			user->atk_its = (n + 1) * 40 + 5 * (*d_l);
			user->def_its = user->def_its + (n + 1) * 10 + 5 * (*d_l);
			strcpy(user->hp_max, itoa(atoi(user->hp_max) + (n + 1) * 50, user->hp_max, 10) + 5 * (*d_l));
			strcpy(user->mp_max, itoa(atoi(user->mp_max) + (n + 1) * 51, user->mp_max, 10) + 5 * (*d_l));
		}

		return;
	}
	else if (ch == 50) {
		system("cls");
		printf("아이템을 버렸습니다.");
		Sleep(1000);
		return;
	}
	else {
		system("cls");
		printf("아이템을 버렸습니다.");
		Sleep(1000);
		return;
	}
}
//dragon 문

void get_items(int* d_l, character* user) {
	srand(time(NULL));
	int ch = 0;
	while (kbhit()) {
		getch(); 	//값을 읽어서 버퍼 지우기
	}
	int n = (*d_l / 10); //드래곤 레벨
	if (n>9) {
		n = 9;
	}
	char dr[2][10][20] = { { "천 옷","가죽 갑옷","구리 갑옷","판금 갑옷","판금 방패","다이아 방패","비브라늄","아다만티움","Armor","Storm Armor" },{ "몽둥이","흡혈의 낫","Sword","Gun","니코","어벤져","파이오니어","스톰","히드라","스톰 브링거" } };
	int a = rand() % 2;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	printf(" '%s' (이)란 아이템을 획득 하셧습니다. \n장착하시겟습니까?\n\n1. 예 2. 아니오\n\n", dr[a][n]);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	if (a == 0) {
		printf("DEF : %d  \t 지금 장착중인 방어구 DEF : %d\n", (n + 1) * 6 + *d_l, user->def_its);
	}
	else if (a == 1) {
		printf("ATK : %d  \t 지금 장착중인 무기 ATK : %d\n", (n + 1) * 7 + *d_l, user->atk_its);
	}
	Sleep(1500);
	while (kbhit()) {
		getch(); 	//값을 읽어서 버퍼 지우기
	}

	do {
		ch = getch();
	} while (ch != 49 && ch != 50);
	if (ch == 49) {
		system("cls");
		printf("아이템을 장착했습니다.");
		Sleep(1000);
		if (a == 0) {
			strcpy(user->def_it, dr[a][n]);
			user->def_its = (n + 1) * 6 + *d_l;
		}
		else if (a == 1) {
			strcpy(user->atk_it, dr[a][n]);
			user->atk_its = (n + 1) * 7 + *d_l;
		}



		return;
	}
	else if (ch == 50) {
		system("cls");
		printf("아이템을 버렸습니다.");
		Sleep(1000);
		return;
	}

}
//get_items 문

void dunmap(int c_l, int *dungeon, char* dunge) {
	int mor = *dungeon;
	printf("1. 소환사의 협곡 추천레벨(1~15)\n2. 뒤틀린 숲 \t 추천레벨(10~25)\n3. Desert \t 추천레벨(20~35) 최소레벨 20\n4. Lost \t 추천레벨(30~45) 최소레벨 30\n5. Noria \t 추천레벨(40~55) 최소레벨 40\n6. Tarkan \t 추천레벨(50~65) 최소레벨 50\n7. Atlance \t 추천레벨(60~75) 최소레벨 60\n8. Icarus \t 추천레벨(70~85) 최소레벨 70\n9. Calima \t 추천레벨(80~95) 최소레벨 80\n0. Epic Dungeon  추천레벨(95이상)최소레벨 90\nq. 돌아가기");
	do {
		*dungeon = getch();
	} while (*dungeon != 113 && *dungeon != 48 && *dungeon != 49 && *dungeon != 50 && *dungeon != 51 && *dungeon != 52 && *dungeon != 53 && *dungeon != 54 && *dungeon != 55 && *dungeon != 56 && *dungeon != 57);
	if (*dungeon == 113) {
		*dungeon = mor;
		return;
	}
	if (*dungeon == 49) {
		strcpy(dunge, "소환사의 협곡");
		gotoxy(28, 12);
		system("cls");
		printf("MAP.1 소환사의 협곡");
		Sleep(700);
	}
	else if (*dungeon == 50) {
		strcpy(dunge, "뒤틀린 숲");
		gotoxy(28, 12);
		system("cls");
		printf("MAP.2 뒤틀린 숲");
		Sleep(700);
	}
	else if (*dungeon == 51 && c_l>20) {
		strcpy(dunge, "Desert");
		gotoxy(28, 12);
		system("cls");
		printf("MAP.3 Desert");
		Sleep(700);
	}
	else if (*dungeon == 52 && c_l>30) {
		strcpy(dunge, "Lost");
		gotoxy(28, 12);
		system("cls");
		printf("MAP.4. Lost");
		Sleep(700);
	}
	else if (*dungeon == 53 && c_l>40) {
		strcpy(dunge, "Noria");
		gotoxy(28, 12);
		system("cls");
		printf("MAP.5 Noria");
		Sleep(700);
	}
	else if (*dungeon == 54 && c_l>50) {
		strcpy(dunge, "Tarkan");
		gotoxy(28, 12);
		system("cls");
		printf("MAP.6 Tarkan");
		Sleep(700);
	}
	else if (*dungeon == 55 && c_l>60) {
		strcpy(dunge, "Atlance");
		gotoxy(28, 12);
		system("cls");
		printf("MAP.7 Atlance");
		Sleep(700);
	}
	else if (*dungeon == 56 && c_l>70) {
		strcpy(dunge, "Icarus");
		gotoxy(28, 12);
		system("cls");
		printf("MAP.8 Icarus");
		Sleep(700);
	}
	else if (*dungeon == 57 && c_l>80) {
		strcpy(dunge, "Calima");
		gotoxy(28, 12);
		system("cls");
		printf("MAP.9 Calima");
		Sleep(700);
	}
	else if (*dungeon == 48 && c_l>90) {
		strcpy(dunge, "Epic Dungeon");
		gotoxy(28, 12);
		system("cls");
		printf("MAP.0 Epic Dungeon");
		Sleep(700);
	}
	else {
		system("cls");
		printf("\n레벨이 부족합니다.");
		*dungeon = mor;
		Sleep(1000);
	}
}
//던전 문

void quest(character* user) {
	int ch = 0;
	int i = 0;
	if (user->que == 0) {
		gotoxy(10, 10);
		printf("퀘스트는 맵의 BOSS몹만 주는 아이템을 얻을 시 완료할 수 있습니다.");
		Sleep(500);
		gotoxy(10, 11);
		printf("아이템 장착은 가능하고 보관은 불가능하여 장착 전의 아이템은 다시 되살릴 수 없습니다.");
		Sleep(500);
		gotoxy(20, 12);
		color(14)
			printf("계속하려면 아무 키나 누르십시오	. . .");
		color(7)
			getch();
		system("cls");
		char def[20] = { "훈련용 갑옷" };
		gotoxy(15, 2);
		printf(" '%s' (이)란 아이템을 획득 하셧습니다.", def);
		gotoxy(28, 4);
		printf("장착하시겟습니까?");
		gotoxy(29, 6);
		printf("1. 예 2. 아니오");
		gotoxy(32, 8);
		printf("DEF : + %d ", 3);
		Sleep(1500);
		while (kbhit()) {
			getch(); 	//값을 읽어서 버퍼 지우기
		}
		do {
			ch = getch();
		} while (ch != 49 && ch != 50);
		if (ch == 49) {
			system("cls");
			gotoxy(22, 4);
			printf("아이템을 장착했습니다.");
			Sleep(1000);
			strcpy(user->def_it, def);
			user->def_its = 3;
			user->que++;
			return;
		}
		else if (ch == 50) {
			system("cls");
			gotoxy(22, 3);
			printf("아이템을 버렸습니다.");
			Sleep(1000);
			user->que++;
			return;
		}
	}
	else {
		printf("%d번째 Quest\n%d.MAP BOSS몹의 전리품을 얻어오세요.\n", user->que, user->que);
		for (i = 1; i<10; i++) {
			if (user->que == i) {
				if (user->qhk) {
					system("cls");
					gotoxy(22, 3);
					printf("퀘스트를 완료했습니다.");
					Sleep(700);
					strcpy(user->exp, "100");
					user->qhk = false;
					user->que++;
					return;
				}
			}

		}
		getch();
		return;
	}


};
//quest 문