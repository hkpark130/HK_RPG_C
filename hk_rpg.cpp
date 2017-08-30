
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
	char gold[7]; // NULL���� �ʸ��� ����
	int hp_it;		// hp����
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


character menu(int ch, character user, int*); //�ϼ�
void fight(character, character*, int); //�ϼ�
void skill(int*, int*, int*, int*, int*, char*, int*, character, int);	//�ϼ�
void item(int* c_h2, int* c_mp2, int* c_hpit2, int* c_mpit2, int*, int*);	// �ϼ� 
void levelup(character*);	//�ϼ�
void store(character*);			//�ϼ�
void dragon(character*, int*); //�ϼ�
void get_items(int*, character*);
void dunmap(int, int*, char*);
void quest(character*);

void gotoxy(int x, int y)//x�� ����, y�� ����
{
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void main()
{
	CursorView(HIDDEN);
	character user = { "�ε������� ����","10","3","1","0","100","100","50","50","0",3,3 };
	user.y = 1;
	int c_a = atoi(user.atk);
	int c_l = atoi(user.lev);
	int hk2 = true; //�޴������� �� �ٽ� ����
	int hk3 = true; //ó�� ���� ������ ����
	int hk4 = true; //���������� �� �ٽ� ���
	int hk5 = true; //�ο�� ���� �� �׸����
	int rx[5] = { 0,1 }, ry[5] = { 1,2 };//�� ������ ��
	int ch = 0;
	int complete = false;
	int dungeon = 49;
	char dunge[20] = "��ȯ���� ����";
	bool hk = false;	//���ο�� �� �� �ٽ� ����
	int choose_1 = 0;	//�ο� ����
	bool load = true;
	int choose_2 = 0;
	clock_t a = 0, b = 0;

	system("title ��HK  RPG��");
	gotoxy(28, 11);
	printf("ó������ �Ͻðڽ��ϱ�?");
	gotoxy(28, 12);
	printf("1. ��\t2. �ƴϿ�\n");
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
			printf("�̸��� �Է����ּ���.");
			gotoxy(20, 11);
			printf("(�ѱ� 5��,���� 10�� �̳��� ���ּ���.)\n");
			gotoxy(30, 12);
			gets_s(user.name);
			system("cls");
		} while (strlen(user.name) >= 11);
		gotoxy(15, 10);
		printf("����Ʈ�� �����Ͽ� �Ʒÿ� �������� ��������.");
		gotoxy(8, 12);
		printf("�̵�Ű�� ����Ű�� �����ϸ� �ٸ� ����� �����Ͻø� ���� �ֽ��ϴ�.");
		gotoxy(18, 14);
		color(14)
			printf("����Ϸ��� �ƹ� Ű�� �����ʽÿ� . . .");
		getch();
		system("cls");
		color(7)
	}

	while (kbhit()) {
		getch(); 	//���� �о ���� �����
	}

	srand((unsigned)time(NULL));
	system("cls");

	for (;;) {
		a = clock();
		if (atoi(user.exp) >= 100) {	//������
			levelup(&user);
		}

		if (ch == 49 || ch == 50 || ch == 51 || ch == 52 || ch == 53 || ch == 54) {	//menu �Լ�
			if (ch == 50 && hk3) {
				do {
					system("cls");
					printf("LOAD �Ͻðڽ��ϱ�?\n1. ��\t2. �ƴϿ�");
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
				getch(); 	//���� �о ���� �����
			}
		}

		if (ch == 113) {	//����
			system("cls");

			dunmap(atoi(user.lev), &dungeon, dunge);
			hk4 = false;
			system("cls");
			user.dun = dungeon;
			strcpy(user.duz, dunge);
		}

		if (!hk || !hk4) {				//�� ����
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

		if ((!hk2 && hk3&& hk5) || (!hk4&& hk3&& hk5)) {			//menu �Լ��� ����� �� �ٽ� ����
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
		printf("1. �����ϱ� 2. �ε��ϱ� 3. �޽��ϱ� 4. �������� 5. �������� 6. ����");





		if ((((user.x == rx[0]) && (user.y == ry[0])) && (hk)) || (((user.x == rx[1]) && (user.y == ry[1])) && (hk)) || (((user.x == rx[2]) && (user.y == ry[2])) && (hk)) || (((user.x == rx[3]) && (user.y == ry[3])) && (hk)) || (((user.x == rx[4]) && (user.y == ry[4])) && (hk))) {
			hk = false;
			system("cls");
			gotoxy(22, 11);
			printf("���� �������ϴ�. ��� �Ͻðڽ��ϱ�?\n");
			gotoxy(28, 12);
			printf("1. �ο��\t2. ����ģ��");
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
				getch(); 	//���� �о ���� �����
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
		printf(" : ��\tMAP.%s", user.duz);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		printf("\tq. ��������\tw. ����Ʈ");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		gotoxy(user.x, user.y);		//��ȯ �� ��ǥ�� ���
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		printf("+");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

		ch = getch();											//����Ű,�޴� �� �Է� ��

		if (ch == 119) {
			system("cls");
			quest(&user);
			system("cls");
			hk4 = false;
		}

		b = clock();

		if (ch == 224)			//�̻��� �� ��ȯ��
			ch = getch();		//�ٽ� ��ȯ�� �ް�

		if (ch == 72 || ch == 75 || ch == 77 || ch == 80) {
			gotoxy(user.x, user.y);		//������ ���ڴ� �������� �����
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
		//�Է¹޴� �ð��� ª���� ĳ���� �̵��ӵ� �׼�
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


	}//for��

}//main ��
 //main ��

character menu(int ch, character user, int* ptr) {

	*ptr = true;
	FILE* fs = fopen("save2.txt", "r+b");
	if (fs == NULL) { //save2 ������ ���ٸ�
		FILE* fs = fopen("save2.txt", "w+b");//save�ؽ�Ʈ ���� ����
	}


	if (ch == 49) { //save
		FILE* fs = fopen("save2.txt", "r+b");
		fwrite(&user, sizeof(user), 1, fs); //����ü �ѹ��� ����

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
		fread(&user, sizeof(user), 1, fs); //����ü �ѹ��� �б�

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
		printf("�� ");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		printf("mp");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("�� ȸ���ϱ� ���� 10�ʰ� �޽����Դϴ�...");

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
		//�̸�����
		printf("NAME : %s\n", user.name);
		//��������
		printf("ATK : %s + %d\n", &user.atk[0], user.atk_its);
		//�������
		printf("DEF : %s + %d\n", user.def, user.def_its);
		//��������
		printf("LEV : %s\n", user.lev);
		//��������
		printf("EXP : %s\n", user.exp);
		//ü������
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		printf("HP : %s/%s\n", user.hp, user.hp_max);
		//��������
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		printf("MP : %s/%s\n", user.mp, user.mp_max);
		//�� ����
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		printf("GOLD : %s\n", user.gold);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		//��������
		printf("HP���� : %d MP���� : %d\n", user.hp_it, user.mp_it);
		printf("\n");
		printf("���� ���� ������\n���� : %s\t���� : %s \n", user.atk_it, user.def_it);
		fclose(fs);
		gotoxy(0, 14);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		printf("��3��° �������� ���������� �ֽ��ϴ�.\n��8���� ���� BOSS���� �����մϴ�.\n��5���� ���� ���ο� ��ų�� ���� �˴ϴ�.\n�ھ������� �ѹ� �����ϸ� ���� ������ �������� �ٽ� �ǻ츱 �� �����ϴ�.\n������ ������ ���ٸ� ����Ű �Է��� ������ �Է����ּ���. \n\t(2ĭ�� �̵��ϱ� �����Դϴ�.)");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("\n\n");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		printf("\t\t������ : 1301127 ���� 1-J ������\n");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("����Ϸ��� �ƹ� Ű�� �����ʽÿ�	. . .");
		*ptr = false;

		getch();
		return user;
	}
	else if (ch == 53) {
		system("cls");
		printf("���� ���� �����ϱ� �� �纸��~\n");
		Sleep(1000);
		system("cls");

		store(&user);

		system("cls");
		*ptr = false;
		return user;
	}
	else if (ch == 54) {
		system("cls");
		printf("�����մϴ�.\n");
		exit(1);
	}



};
//menu ��

void store(character* user) {
	int ch = 0;
	srand(time((unsigned int)NULL));
	char def[][20] = { "õ����","ö����","���ð���","������ ����","������ ����","������� ����" };
	char atk[][20] = { "�ռҵ�","������","��������","������ ���� ��","���ָ� �����","�ǹٶ��" };
	int r = rand() % 6;
	int sr = rand() % 6;
	int s = 0;	//���� ����

	printf("1. %s\t(%d GOLD) DEF+%d\n", def[r], r * 1100 + 1100, (r + 1) * 25);
	printf("2. %s\t(%d GOLD) ATK+%d\n", atk[sr], sr * 1200 + 1200, (sr + 1) * 36);
	printf("3. HP����\t(100 GOLD)\n");
	printf("4. MP����\t(80 GOLD)\n");
	printf("5. ATK +1\t(500 GOLD)\n");
	printf("6. DEF +1\t(500 GOLD)\n");
	printf("7. HP_MAX +10\t(500 GOLD)\n");
	printf("8. MP_MAX +5 \t(500 GOLD)\n");
	printf("9. ������\n\n");
	printf("���� GOLD : %s", user->gold);
	do {
		ch = getch();
	} while (ch != 49 && ch != 50 && ch != 51 && ch != 52 && ch != 53 && ch != 54 && ch != 55 && ch != 56 && ch != 57);
	if (ch == 49) {
		system("cls");
		if (atoi(user->gold)<r * 1100 + 1100) {
			printf("GOLD�� �����մϴ�");
			Sleep(1000);
			return;
		}
		printf("������ %s(��)�� �����߽��ϴ�.\n", def[r]);
		Sleep(1000);
		system("cls");
		printf("�� ���ʼ�~");
		Sleep(100);

		strcpy(user->def_it, def[r]);
		strcpy(user->gold, itoa(atoi(user->gold) - (r * 1100 + 1100), user->gold, 10));
		user->def_its = (r + 1) * 25;
		return;
	}
	if (ch == 50) {
		system("cls");
		if (atoi(user->gold)<sr * 1200 + 1200) {
			printf("GOLD�� �����մϴ�");
			Sleep(1000);
			return;
		}
		printf("������ %s(��)�� �����߽��ϴ�.\n", atk[sr]);
		Sleep(1000);
		system("cls");
		printf("�� ���ʼ�~");
		Sleep(100);
		strcpy(user->atk_it, atk[sr]);
		strcpy(user->gold, itoa(atoi(user->gold) - (sr * 1200 + 1200), user->gold, 10));
		user->atk_its = (sr + 1) * 36;
		return;
	}
	if (ch == 51) {
		system("cls");
		printf("�� �� ��� �ǰ���?");
		scanf("%d", &s);
		fflush(stdin);
		if (atoi(user->gold)<100 * s) {
			printf("GOLD�� �����մϴ�");
			Sleep(1000);
			return;
		}
		printf("HP ������ �����Ͽ����ϴ�.\n");
		Sleep(1000);
		system("cls");
		printf("�� ���ʼ�~");
		Sleep(100);
		user->hp_it = user->hp_it + s;
		strcpy(user->gold, itoa(atoi(user->gold) - 100 * s, user->gold, 10));
		return;
	}
	if (ch == 52) {
		system("cls");
		printf("�� �� ��� �ǰ���?");
		scanf("%d", &s);
		fflush(stdin);
		if (atoi(user->gold)<80 * s) {
			printf("GOLD�� �����մϴ�");
			Sleep(1000);
			return;
		}
		printf("MP ������ �����Ͽ����ϴ�.\n");
		Sleep(1000);
		system("cls");
		printf("�� ���ʼ�~");
		Sleep(100);
		user->mp_it = user->mp_it + s;
		strcpy(user->gold, itoa(atoi(user->gold) - 80 * s, user->gold, 10));
		return;
	}
	if (ch == 53) {
		system("cls");
		printf("�� �� ��� �ǰ���?");
		scanf("%d", &s);
		fflush(stdin);
		if (atoi(user->gold)<500 * s) {
			printf("GOLD�� �����մϴ�");
			Sleep(1000);
			return;
		}
		printf("ATK ���� +%d \n", s);
		Sleep(1000);
		system("cls");
		printf("�� ���ʼ�~");
		Sleep(100);
		strcpy(user->atk, itoa(atoi(user->atk) + s, user->atk, 10));
		strcpy(user->gold, itoa(atoi(user->gold) - 500 * s, user->gold, 10));
		return;
	}
	if (ch == 54) {
		system("cls");
		printf("�� �� ��� �ǰ���?");
		scanf("%d", &s);
		fflush(stdin);
		if (atoi(user->gold)<500 * s) {
			printf("GOLD�� �����մϴ�");
			Sleep(1000);
			return;
		}
		printf("DEF ���� +%d \n", s);
		Sleep(1000);
		system("cls");
		printf("�� ���ʼ�~");
		Sleep(100);
		strcpy(user->def, itoa(atoi(user->def) + s, user->def, 10));
		strcpy(user->gold, itoa(atoi(user->gold) - 500 * s, user->gold, 10));
		return;
	}
	if (ch == 55) {
		system("cls");
		printf("�� �� ��� �ǰ���?");
		scanf("%d", &s);
		fflush(stdin);
		if (atoi(user->gold)<500 * s) {
			printf("GOLD�� �����մϴ�");
			Sleep(1000);
			return;
		}
		printf("HP_MAX ���� +%d \n", s * 10);
		Sleep(1000);
		system("cls");
		printf("�� ���ʼ�~");
		Sleep(100);
		strcpy(user->hp_max, itoa(atoi(user->hp_max) + 10 * s, user->hp_max, 10));
		strcpy(user->gold, itoa(atoi(user->gold) - 500 * s, user->gold, 10));
		return;
	}
	if (ch == 56) {
		system("cls");
		printf("�� �� ��� �ǰ���?");
		scanf("%d", &s);
		fflush(stdin);
		if (atoi(user->gold)<500 * s) {
			printf("GOLD�� �����մϴ�");
			Sleep(1000);
			return;
		}
		printf("MP_MAX ���� +%d \n", s * 5);
		Sleep(1000);
		system("cls");
		printf("�� ���ʼ�~");
		Sleep(100);
		strcpy(user->mp_max, itoa(atoi(user->mp_max) + 5 * s, user->mp_max, 10));
		strcpy(user->gold, itoa(atoi(user->gold) - 500 * s, user->gold, 10));
		return;
	}
	if (ch == 57) {
		system("cls");
		printf("�� ���ʼ�~");
		Sleep(100);
		return;
	}

}
//store ��

void fight(character user, character* uptr, int d) {
	srand(time(NULL));
	int survival = 0;
	int c_a = atoi(user.atk) + user.atk_its;//���� �ɷ�
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
	int cho = 0;				//����
	int b_l = 0;	//�� ����
	int b_h = 0;	//�� ü��
	int b_hp_max = 0;
	int b_a = 0;		//������ ���� ����
	int c_p = 0;		//������ ���� ����
	char bot[10][5][20] = { { "������", "���", "��ũ", "Ʈ��", "�׸� �巡��" },{ "���� Ʈ��","�һ�����","��ġ","�Ͽ��","���� �巡��" },{ "��", "����", "ȣ�ӵ�", "���̽���", "ȭ��Ʈ �巡��" },{ "������","��� ����","����","�ư�","�� �巡��" },{ "���̷���","����","����Ŭ�ӽ�","����","�� �巡��" },{ "������","���ֹ��� ����","����ī��","�߷�","��� �巡��" },{ "���Ϲ�Ʈ","��Ű��","���ڵ�","�����","���� �巡��" },{ "����Ʈ","���� ����","����ĭ","źŻ�ν�","�� �巡��" },{ "�� ���̳�","���ĭ","���� ũ����Ʈ","�һ���","õ��� �巡��" },{ "�̽ý�","�α� ���򸮿�","����Ŀ","�ٷ�","����� ȯ��" } };
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
		printf("BOSS���� 8�����ĺ��� ���ɴϴ�.");
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
			printf("��BOSS ���ӡ�");
		}

		gotoxy(1, 5);
		printf("1. ����");
		gotoxy(1, 6);
		printf("2. ��ų");
		gotoxy(1, 7);
		printf("3. ����");
		gotoxy(1, 8);
		printf("4. ����");

		cho = getch();

		if (cho == 49) {

			if (d<1) {
				b_a = rand() % (b_l)* 2 + (bot_r)* 2 - (c_d);//����
				c_p = rand() % (c_a)+(c_a) / 10;		//�ǰ�
				bonus = 30;
				gold_bonus = 40;
			}
			else if (d == 1) {
				b_a = rand() % (b_l)* 2 + 4 * d + (bot_r)* 4 - (c_d);
				c_p = rand() % (c_a + (c_a) / 10);		//�ǰ�
				bonus = 20;
				gold_bonus = 40;
			}
			else if (d <= 3) {
				b_a = rand() % (b_l)* 4 + 4 * d + (bot_r)* 5 - (c_d);
				c_p = rand() % (c_a)+(c_a) / 10;		//�ǰ�
				bonus = 20;
				gold_bonus = 40;
			}
			else if (d <= 5) {
				b_a = rand() % (b_l)* 6 + 4 * d + (bot_r)* 6 - (c_d);
				c_p = rand() % (c_a)+(c_a) / 10;		//�ǰ�
				bonus = 10;
				gold_bonus = 55;

			}
			else if (d <= 7) {
				b_a = rand() % (b_l)* 9 + 6 * d + (bot_r)* 8 - (c_d);
				c_p = rand() % (c_a)+(c_a) / 10;		//�ǰ�
				gold_bonus = 105;

			}
			else if (d == 8) {
				b_a = rand() % (b_l)* 10 + 7 * d + (bot_r)* 13 - (c_d);
				c_p = rand() % (c_a)+(c_a) / 10;		//�ǰ�
				gold_bonus = 500;

			}
			else if (d == 9) {
				b_a = rand() % (b_l)* 11 + 12 * d + (bot_r)* 20 - (c_d);
				c_p = rand() % (c_a)+(c_a) / 10;		//�ǰ�
				gold_bonus = 1000;
			}
			gotoxy(0, 9);
			dragon_skill = rand() % 51;
			printf("%s�� %s���� %d�� ���ظ� �־���\n", user.name, bot[d][bot_r], c_p);
			if (bot_r == 4 && get_item % 5 == 0 && (dragon_skill % 5 == 0) && d <= 8) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				printf("%s�� breath�� �����Ͽ���.\n", bot[d][bot_r]);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				if (b_a <= 0) {
					printf("%s�� breath�� ȸ��!! \t\t\t\t\t\n", bot[d][bot_r]);
					b_a = 0;
				}
				else
					printf("%s�� %s���� %d�� ���ظ� ������.\n", bot[d][bot_r], user.name, b_a * 3);

				b_a = b_a * 3;
				Sleep(200);
			}
			else {
				if (b_a <= 0) {
					printf("%s�� ������ ȸ��!! \t\t\t\t\t\n", bot[d][bot_r]);
					b_a = 0;
				}
				else
					printf("%s�� %s���� %d�� ���ظ� ������.\n", bot[d][bot_r], user.name, b_a);
			}
			Sleep(1000);

			c_h = c_h - b_a;
			b_h = b_h - c_p;

			if (c_h <= 0) {
				system("cls");
				c_e = c_e - 10;
				printf("�й��ϼ̽��ϴ�.\n");
				printf("����ġ�� 10 �ս��Ͽ� ���� ����ġ : %d\n", c_e);
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
						printf("Quest�� �Ϸ��߽��ϴ�.\n������ ��������.");
						Sleep(1000);
						uptr->qhk = true;
					}
					system("cls");
				}
				else if (bot_r != 4 && (get_item % 11 == 1)) {
					system("cls");
					while (kbhit()) {
						getch(); 	//���� �о ���� �����
					}
					get_items(&b_l, uptr);
					system("cls");
				}

				printf("�¸��ϼ̽��ϴ�.\n");
				printf("���� ����ġ %d\n", c_e);
				printf("���� GOLD %d\n", c_gold);
				Sleep(1200);
				strcpy(uptr->gold, itoa(atoi(user.gold) + c_gold, user.gold, 10));
				strcpy(uptr->exp, itoa(atoi(user.exp) + c_e, user.exp, 10));
				strcpy(uptr->hp, itoa(c_h, user.hp, 10));
				return;
			}
		}
		else if (cho == 50) {			//��ų���

			skill(&c_h, &c_mp, &b_h, &c_d, &c_l, bot[d][bot_r], &b_l, user, bot_r);

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			strcpy(uptr->mp, itoa(c_mp, user.mp, 10));

			if (c_h <= 0) {
				system("cls");

				c_e = c_e - 10;
				printf("�й��ϼ̽��ϴ�.\n");
				printf("����ġ�� 10 �ս��Ͽ� ���� ����ġ : %d\n", c_e);
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
						printf("Quest�� �Ϸ��߽��ϴ�.\n������ ��������.");
						Sleep(1000);
						uptr->qhk = true;
					}
					system("cls");
				}
				else if (bot_r != 4 && (get_item % 11 == 1)) {
					system("cls");
					while (kbhit()) {
						getch(); 	//���� �о ���� �����
					}
					get_items(&b_l, uptr);
					system("cls");
				}
				if (c_e >= 30) {
					c_e = c_e - 15;
				}
				c_gold = gold_bonus + baron + (rand() % b_l)*(4) + 20;
				printf("�¸��ϼ̽��ϴ�.\n");
				printf("���� ����ġ %d\n", c_e);
				printf("���� GOLD %d\n", c_gold);
				Sleep(1200);
				strcpy(uptr->gold, itoa(atoi(user.gold) + c_gold, user.gold, 10));
				strcpy(uptr->exp, itoa(atoi(user.exp) + c_e, user.exp, 10));
				strcpy(uptr->hp, itoa(c_h, user.hp, 10));
				return;
			}


		}
		else if (cho == 51) {			//�������

			item(&c_h, &c_mp, &c_hp_it, &c_mp_it, &c_hp_max, &c_mp_max); //������ �Լ��� ����ü ���� �ּҰ� �ѱ�!!!
			uptr->hp_it = c_hp_it;
			uptr->mp_it = c_mp_it;

		}
		else if (cho == 52) {			//����
			if (survival == 1) {
				system("cls");
				printf("�������� ���߽��ϴ�.\n");
				b_a = rand() % (b_l)* 2 + d + (bot_r)* 2 - (c_d);
				if (b_a>0) {
					printf("%s�� %s���� %d�� ���ظ� ������.\n", bot[d][bot_r], user.name, b_a);
				}
				else {
					printf("%s�� ������ ȸ��!! \t\t\t\t\t\n", bot[d][bot_r]);
				}
				c_h = c_h - b_a;
				Sleep(1100);
				if (c_h <= 0) {
					system("cls");
					c_e = c_e - 10;
					printf("�й��ϼ̽��ϴ�.\n");
					printf("����ġ�� 10 �ս��Ͽ� ���� ����ġ : %d\n", c_e);
					Sleep(1000);
					strcpy(uptr->exp, itoa(atoi(user.exp) - 10, user.exp, 10));
					strcpy(uptr->hp, "1");
					return;
				}

			}
			else {
				system("cls");
				printf("��������~");
				Sleep(1000);
				strcpy(uptr->hp, itoa(c_h, user.hp, 10));
				return;
			}
		}

	} while (cho != 1);	//���ѷ���


};//fight ��
  //fight ��

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
		printf("���ο� ��ų�� ������ϴ�.\n");
		Sleep(1000);
		system("cls");
	}
	for (i = 3; i != 0; i--) {
		while (kbhit()) {
			getch(); 	//���� �о ���� �����
		}
		printf("������ �ø�����.\n1. ATK + 2\n2. DEF\n3. HP_MAX + 10\n4. MP_MAX + 5\n���� ����Ʈ : %d\n", i);
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
			printf("���� ���� �Է��ϼ���.");
			Sleep(500);
			i++;
			system("cls");
		}
	}
	system("cls");
	strcpy(ulptr->hp, ulptr->hp_max);
	strcpy(ulptr->mp, ulptr->mp_max);

}//levelup ��
 //������ ��

void item(int* c_h2, int* c_mp2, int* c_hpit2, int* c_mpit2, int* c_hp_max2, int* c_mp_max2) {
	int ch = 0;
	system("cls");
	printf("1. hp ���� %d��\n", *c_hpit2);
	printf("2. mp ���� %d��\n", *c_mpit2);
	printf("3. ������\n");
	ch = getch();
	if (ch == 49 && *c_hpit2>0) {
		*c_h2 = *c_h2 + 100;
		if (*c_h2 > * c_hp_max2) {
			printf("�ִ� hp�� %d �Դϴ�.\n", *c_hp_max2);
			*c_h2 = *c_hp_max2;
		}
		gotoxy(0, 9);
		printf("���� 100�� ü���� ȸ���ߴ�\n");
		*c_hpit2 = *c_hpit2 - 1;
		Sleep(1000);
		return;
	}
	else if (ch == 50 && *c_mpit2>0) {
		*c_mp2 = *c_mp2 + 50;
		if (*c_mp2 > * c_mp_max2) {
			printf("�ִ� mp�� %d �Դϴ�.\n", *c_mp_max2);
			*c_mp2 = *c_mp_max2;
		}
		gotoxy(0, 9);
		printf("���� 50�� ������ ȸ���ߴ�\n");
		*c_mpit2 = *c_mpit2 - 1;
		Sleep(1000);
		return;
	}
	else if (ch == 51) {
		return;
	}
	else {
		gotoxy(0, 9);
		printf("���� �߸� �Է��ϼ̰ų� ������ �����մϴ�.\n");
		Sleep(1000);
		return;
	}
}
//item ��

void skill(int* c_h2, int* c_mp2, int* b_h2, int* c_d2, int* c_l2, char* mob, int* b_l2, character user, int bot_r) {
	// c_h2 �� ü�� | c_mp2 �� ���� | b_h2 �� ü�� | c_d2 �� ��� | c_l2 �� ����
	int ch = 0;
	int i = 10;
	int j = 0;
	srand(time(NULL));
	int dmg = rand() % (*b_l2) * 6 - (*c_d2); //�Ϻη� ��ų ������ �������� �� ������ ����
	system("cls");
	int dragon_skill = rand() % 51;
	if (*c_l2 <= 4) {
		printf("������ ���� ����Ͻ� �� �ִ� ��ų�� �����ϴ�.\n������ 5�̻��� �Ǿ�� �մϴ�.");
		Sleep(1000);
		return;
	}

	if (*c_l2 >= 5) {
		printf("1. ������ ���� (Lv ��� ������) (MP 20 �Ҹ�)\n");
	}
	if (*c_l2 >= 10) {
		printf("2. ���ַ� �˼� (Lv ��� ������) (MP 50 �Ҹ�)\n");
	}
	if (*c_l2 >= 15) {
		printf("3. ���� (Lv ��� ������) (MP 70 �Ҹ�)\n");
	}
	if (*c_l2 >= 20) {
		printf("4. ����!! (ATK ��� ������) (MP 90 �Ҹ�)\n");
	}
	if (*c_l2 >= 25) {
		printf("5. ��ȥ�� (Lv+ATK ��� ������) (MP 110 �Ҹ�)\n");
	}
	if (*c_l2 >= 30) {
		printf("6. �����þ� (Lv+ATK ��� ������) (MP 200 �Ҹ�)\n");
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
				printf("������ ����");
				Sleep(80);
			}
			*c_mp2 = *(c_mp2)-20;
			gotoxy(0, 9);
			printf("%s�� %s���� %d�� ���ظ� �־���\n", user.name, mob, 10 * (*c_l2));
			*b_h2 = *b_h2 - 10 * (*c_l2);
			if (bot_r == 4 && dragon_skill % 5 == 0 && *c_l2<80) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				printf("%s�� breath�� �����Ͽ���.\n", mob);
				if (dmg <= 0) {
					printf("%s�� breath�� ȸ��!! \t\t\t\t\t\n", mob);
					dmg = 0;
				}
				else
					printf("%s�� %s���� %d�� ���ظ� ������.\n", mob, user.name, dmg * 3);

				dmg = dmg * 3;
				*c_h2 = *c_h2 - dmg;
				Sleep(1000);
			}
			else {
				if (dmg <= 0) {
					printf("%s�� ������ ȸ��!! \t\t\t\t\t\n", mob);
					Sleep(1000);
				}
				else {
					printf("%s�� %s���� %d�� ���ظ� ������.\n", mob, user.name, dmg);
					*c_h2 = *c_h2 - dmg;
					Sleep(1000);
				}
			}
			return;
		}
		else {
			system("cls");
			printf("������ �����ϰų� ������ �� �˴ϴ�.");
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
				printf("���ַ� �˼�");
				Sleep(80);
			}
			*c_mp2 = *c_mp2 - 50;
			gotoxy(0, 9);
			printf("%s�� %s���� %d�� ���ظ� �־���\n", user.name, mob, 15 * (*c_l2));
			*b_h2 = *b_h2 - 15 * (*c_l2);
			if (bot_r == 4 && dragon_skill % 5 == 0 && *c_l2<80) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				printf("%s�� breath�� �����Ͽ���.\n", mob);
				if (dmg <= 0) {
					printf("%s�� breath�� ȸ��!! \t\t\t\t\t\n", mob);
					dmg = 0;
				}
				else
					printf("%s�� %s���� %d�� ���ظ� ������.\n", mob, user.name, dmg * 3);
				dmg = dmg * 3;
				*c_h2 = *c_h2 - dmg;
				Sleep(1000);
			}
			else {
				if (dmg <= 0) {
					printf("%s�� ������ ȸ��!! \t\t\t\t\t\n", mob);
					Sleep(1000);
				}
				else {
					printf("%s�� %s���� %d�� ���ظ� ������.\n", mob, user.name, dmg);
					*c_h2 = *c_h2 - dmg;
					Sleep(1000);
				}
			}
			return;
		}
		else {
			system("cls");
			printf("������ �����ϰų� ������ �� �˴ϴ�.");
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
				printf("����");
				Sleep(80);
			}
			*c_mp2 = *c_mp2 - 70;
			gotoxy(0, 9);
			printf("%s�� %s���� %d�� ���ظ� �־���\n", user.name, mob, 20 * (*c_l2));
			*b_h2 = *b_h2 - 20 * (*c_l2);
			if (bot_r == 4 && dragon_skill % 5 == 0 && *c_l2<80) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				printf("%s�� breath�� �����Ͽ���.\n", mob);
				if (dmg <= 0) {
					printf("%s�� breath�� ȸ��!! \t\t\t\t\t\n", mob);
					dmg = 0;
				}
				else
					printf("%s�� %s���� %d�� ���ظ� ������.\n", mob, user.name, dmg * 4);
				dmg = dmg * 4;
				*c_h2 = *c_h2 - dmg;
				Sleep(1000);
			}
			else {
				if (dmg <= 0) {
					printf("%s�� ������ ȸ��!! \t\t\t\t\t\n", mob);
					Sleep(1000);
				}
				else {
					printf("%s�� %s���� %d�� ���ظ� ������.\n", mob, user.name, dmg);
					*c_h2 = *c_h2 - dmg;
					Sleep(1000);
				}
			}
			return;
		}
		else {
			system("cls");
			printf("������ �����ϰų� ������ �� �˴ϴ�.");
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
				printf("����!! (���� �ù� ���ϴ�.) ");
				Sleep(80);
			}
			*c_mp2 = *c_mp2 - 90;
			gotoxy(0, 9);
			printf("%s�� %s���� %d�� ���ظ� �־���\n", user.name, mob, 30 * (atoi(user.atk)));
			*b_h2 = *b_h2 - 30 * (atoi(user.atk));
			if (bot_r == 4 && dragon_skill % 5 == 0 && *c_l2<80) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				printf("%s�� breath�� �����Ͽ���.\n", mob);
				if (dmg <= 0) {
					printf("%s�� breath�� ȸ��!! \t\t\t\t\t\n", mob);
					dmg = 0;
				}
				else
					printf("%s�� %s���� %d�� ���ظ� ������.\n", mob, user.name, dmg * 5);
				dmg = dmg * 5;
				*c_h2 = *c_h2 - dmg;
				Sleep(1000);
			}
			else {
				if (dmg <= 0) {
					printf("%s�� ������ ȸ��!! \t\t\t\t\t\n", mob);
					Sleep(1000);
				}
				else {
					printf("%s�� %s���� %d�� ���ظ� ������.\n", mob, user.name, dmg);
					*c_h2 = *c_h2 - dmg;
					Sleep(1000);
				}
			}
			return;
		}
		else {
			system("cls");
			printf("������ �����ϰų� ������ �� �˴ϴ�.");
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
				printf("��ȥ�� (���� �ξ�� �� ���״�..) ");
				Sleep(80);
			}
			*c_mp2 = *c_mp2 - 110;
			gotoxy(0, 9);
			printf("%s�� %s���� %d�� ���ظ� �־���\n", user.name, mob, 40 * (*c_l2) + atoi(user.atk));
			*b_h2 = *b_h2 - (40 * (*c_l2) + atoi(user.atk));
			if (bot_r == 4 && dragon_skill % 5 == 0 && *c_l2<80) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				printf("%s�� breath�� �����Ͽ���.\n", mob);
				if (dmg <= 0) {
					printf("%s�� breath�� ȸ��!! \t\t\t\t\t\n", mob);
					dmg = 0;
				}
				else
					printf("%s�� %s���� %d�� ���ظ� ������.\n", mob, user.name, dmg * 6);
				dmg = dmg * 6;
				*c_h2 = *c_h2 - dmg;
				Sleep(1000);
			}
			else {
				if (dmg <= 0) {
					printf("%s�� ������ ȸ��!! \t\t\t\t\t\n", mob);
					Sleep(1000);
				}
				else {
					printf("%s�� %s���� %d�� ���ظ� ������.\n", mob, user.name, dmg);
					*c_h2 = *c_h2 - dmg;
					Sleep(1000);
				}
			}
			return;
		}
		else {
			system("cls");
			printf("������ �����ϰų� ������ �� �˴ϴ�.");
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
				printf("�����þ�!!");
				Sleep(80);
			}
			*c_mp2 = *c_mp2 - 200;
			gotoxy(0, 9);
			printf("%s�� %s���� %d�� ���ظ� �־���\n", user.name, mob, 70 * (*c_l2) + 2 * atoi(user.atk));
			*b_h2 = *b_h2 - (70 * (*c_l2) + 2 * atoi(user.atk));
			if (bot_r == 4 && dragon_skill % 5 == 0 && *c_l2<80) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				printf("%s�� breath�� �����Ͽ���.\n", mob);
				if (dmg <= 0) {
					printf("%s�� breath�� ȸ��!! \t\t\t\t\t\n", mob);
					dmg = 0;
				}
				else
					printf("%s�� %s���� %d�� ���ظ� ������.\n", mob, user.name, dmg * 8);
				dmg = dmg * 8;
				*c_h2 = *c_h2 - dmg;
				Sleep(1000);
			}
			else {
				if (dmg <= 0) {
					printf("%s�� ������ ȸ��!! \t\t\t\t\t\n", mob);
					Sleep(1000);
				}
				else {
					printf("%s�� %s���� %d�� ���ظ� ������.\n", mob, user.name, dmg);
					*c_h2 = *c_h2 - dmg;
					Sleep(1000);
				}
			}
			return;
		}
		else {
			system("cls");
			printf("������ �����ϰų� ������ �� �˴ϴ�.");
			Sleep(1000);
			return;
		}
	}
	else {
		system("cls");
		printf("���� ���� �Է����� ���մϱ�?!!");
		Sleep(1000);
		return;
	}

}
//skill ��

void dragon(character* user, int* d_l) {
	int ch = 0;
	while (kbhit()) {
		getch(); 	//���� �о ���� �����
	}
	int n = (*d_l / 10) - 1; //�巡�� ����
	char dr[][20] = { "�巡����","�巡���̻�","�巡����Ʈ","�巡�� �ҿ�" };
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	printf(" '%s' �̶� �������� ȹ�� �ϼ˽��ϴ�. \n�����Ͻðٽ��ϱ�?\n\n1. �� 2. �ƴϿ�\n\n", dr[n]);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	if (n == 0) {
		printf("DEF : %d \t ���� �������� �� DEF : %d\n", (n + 1) * 35, user->def_its);
	}
	else if (n == 1 || n == 2) {
		printf("ATK : %d \t ���� �������� ���� ATK : %d \n", (n + 1) * 45, user->atk_its);
	}
	else {
		printf("(���� �ߺ����� ����)\n(�� ����� �ߺ� ���� �Ұ�)");
		printf("ATK : %d \n", (n + 1) * 40 + 5 * (*d_l));
		printf("DEF : %d \n", (n + 1) * 10 + 5 * (*d_l));
		printf("HP_MAX : %d \n", (n + 1) * 50 + 5 * (*d_l));
		printf("MP_MAX : %d \n", (n + 1) * 51 + 5 * (*d_l));
	}

	Sleep(1500);
	while (kbhit()) {
		getch(); 	//���� �о ���� �����
	}

	ch = getch();
	if (ch == 49) {
		system("cls");
		printf("�������� �����߽��ϴ�.");
		Sleep(1000);
		if (n == 0) {
			strcpy(user->def_it, "�巡����");
			user->def_its = (n + 1) * 35;
		}
		else if (n == 1) {
			strcpy(user->atk_it, "�巡���̻�");
			user->atk_its = (n + 1) * 45;
		}
		else if (n == 2) {
			strcpy(user->atk_it, "�巡����Ʈ");
			user->atk_its = (n + 1) * 45;
		}
		else if (n >= 3) {
			strcpy(user->atk_it, "�巡�� �ҿ�");
			user->atk_its = (n + 1) * 40 + 5 * (*d_l);
			user->def_its = user->def_its + (n + 1) * 10 + 5 * (*d_l);
			strcpy(user->hp_max, itoa(atoi(user->hp_max) + (n + 1) * 50, user->hp_max, 10) + 5 * (*d_l));
			strcpy(user->mp_max, itoa(atoi(user->mp_max) + (n + 1) * 51, user->mp_max, 10) + 5 * (*d_l));
		}

		return;
	}
	else if (ch == 50) {
		system("cls");
		printf("�������� ���Ƚ��ϴ�.");
		Sleep(1000);
		return;
	}
	else {
		system("cls");
		printf("�������� ���Ƚ��ϴ�.");
		Sleep(1000);
		return;
	}
}
//dragon ��

void get_items(int* d_l, character* user) {
	srand(time(NULL));
	int ch = 0;
	while (kbhit()) {
		getch(); 	//���� �о ���� �����
	}
	int n = (*d_l / 10); //�巡�� ����
	if (n>9) {
		n = 9;
	}
	char dr[2][10][20] = { { "õ ��","���� ����","���� ����","�Ǳ� ����","�Ǳ� ����","���̾� ����","����","�ƴٸ�Ƽ��","Armor","Storm Armor" },{ "������","������ ��","Sword","Gun","����","���","���̿��Ͼ�","����","�����","���� �긵��" } };
	int a = rand() % 2;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	printf(" '%s' (��)�� �������� ȹ�� �ϼ˽��ϴ�. \n�����Ͻðٽ��ϱ�?\n\n1. �� 2. �ƴϿ�\n\n", dr[a][n]);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	if (a == 0) {
		printf("DEF : %d  \t ���� �������� �� DEF : %d\n", (n + 1) * 6 + *d_l, user->def_its);
	}
	else if (a == 1) {
		printf("ATK : %d  \t ���� �������� ���� ATK : %d\n", (n + 1) * 7 + *d_l, user->atk_its);
	}
	Sleep(1500);
	while (kbhit()) {
		getch(); 	//���� �о ���� �����
	}

	do {
		ch = getch();
	} while (ch != 49 && ch != 50);
	if (ch == 49) {
		system("cls");
		printf("�������� �����߽��ϴ�.");
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
		printf("�������� ���Ƚ��ϴ�.");
		Sleep(1000);
		return;
	}

}
//get_items ��

void dunmap(int c_l, int *dungeon, char* dunge) {
	int mor = *dungeon;
	printf("1. ��ȯ���� ���� ��õ����(1~15)\n2. ��Ʋ�� �� \t ��õ����(10~25)\n3. Desert \t ��õ����(20~35) �ּҷ��� 20\n4. Lost \t ��õ����(30~45) �ּҷ��� 30\n5. Noria \t ��õ����(40~55) �ּҷ��� 40\n6. Tarkan \t ��õ����(50~65) �ּҷ��� 50\n7. Atlance \t ��õ����(60~75) �ּҷ��� 60\n8. Icarus \t ��õ����(70~85) �ּҷ��� 70\n9. Calima \t ��õ����(80~95) �ּҷ��� 80\n0. Epic Dungeon  ��õ����(95�̻�)�ּҷ��� 90\nq. ���ư���");
	do {
		*dungeon = getch();
	} while (*dungeon != 113 && *dungeon != 48 && *dungeon != 49 && *dungeon != 50 && *dungeon != 51 && *dungeon != 52 && *dungeon != 53 && *dungeon != 54 && *dungeon != 55 && *dungeon != 56 && *dungeon != 57);
	if (*dungeon == 113) {
		*dungeon = mor;
		return;
	}
	if (*dungeon == 49) {
		strcpy(dunge, "��ȯ���� ����");
		gotoxy(28, 12);
		system("cls");
		printf("MAP.1 ��ȯ���� ����");
		Sleep(700);
	}
	else if (*dungeon == 50) {
		strcpy(dunge, "��Ʋ�� ��");
		gotoxy(28, 12);
		system("cls");
		printf("MAP.2 ��Ʋ�� ��");
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
		printf("\n������ �����մϴ�.");
		*dungeon = mor;
		Sleep(1000);
	}
}
//���� ��

void quest(character* user) {
	int ch = 0;
	int i = 0;
	if (user->que == 0) {
		gotoxy(10, 10);
		printf("����Ʈ�� ���� BOSS���� �ִ� �������� ���� �� �Ϸ��� �� �ֽ��ϴ�.");
		Sleep(500);
		gotoxy(10, 11);
		printf("������ ������ �����ϰ� ������ �Ұ����Ͽ� ���� ���� �������� �ٽ� �ǻ츱 �� �����ϴ�.");
		Sleep(500);
		gotoxy(20, 12);
		color(14)
			printf("����Ϸ��� �ƹ� Ű�� �����ʽÿ�	. . .");
		color(7)
			getch();
		system("cls");
		char def[20] = { "�Ʒÿ� ����" };
		gotoxy(15, 2);
		printf(" '%s' (��)�� �������� ȹ�� �ϼ˽��ϴ�.", def);
		gotoxy(28, 4);
		printf("�����Ͻðٽ��ϱ�?");
		gotoxy(29, 6);
		printf("1. �� 2. �ƴϿ�");
		gotoxy(32, 8);
		printf("DEF : + %d ", 3);
		Sleep(1500);
		while (kbhit()) {
			getch(); 	//���� �о ���� �����
		}
		do {
			ch = getch();
		} while (ch != 49 && ch != 50);
		if (ch == 49) {
			system("cls");
			gotoxy(22, 4);
			printf("�������� �����߽��ϴ�.");
			Sleep(1000);
			strcpy(user->def_it, def);
			user->def_its = 3;
			user->que++;
			return;
		}
		else if (ch == 50) {
			system("cls");
			gotoxy(22, 3);
			printf("�������� ���Ƚ��ϴ�.");
			Sleep(1000);
			user->que++;
			return;
		}
	}
	else {
		printf("%d��° Quest\n%d.MAP BOSS���� ����ǰ�� ��������.\n", user->que, user->que);
		for (i = 1; i<10; i++) {
			if (user->que == i) {
				if (user->qhk) {
					system("cls");
					gotoxy(22, 3);
					printf("����Ʈ�� �Ϸ��߽��ϴ�.");
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
//quest ��