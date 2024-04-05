#define _CRT_NONSTDC_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <stdbool.h>
#include <Windows.h>
#include <time.h>
#define ENTER 10
#define BACKSPACE 8
#define ESC 27
#define SPACE 32
#define LEFT 75 // 방향키
#define RIGHT 77
#define UP 72
#define DOWN 80
typedef struct pos {
	int x;
	int y;
	int dir;
	int shape;
} P;
int map[60][60], size;
P snake[10000], portal[60][60];
void gotoxy(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
} //좌표 함수
void GotoMap(int x, int y) { //(게임판 기준) 상대 좌표
	gotoxy((1 + x) * 2, 4 + y); //절대 좌표
}
void textcolor(int colorNum) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
} //글씨 색상 함수
int IsEmpty() {
	return snake[0].x == -1 && snake[0].y == -1;
}
void EnQueue(int x, int y, int dir) {
	if (!IsEmpty()) {
		GotoMap(snake[0].x, snake[0].y);
		printf("■");
	}
	for (int i = size - 1; i >= 0; i--)
		snake[i + 1] = snake[i];
	snake[0].x = x;
	snake[0].y = y;
	snake[0].dir = dir;
	GotoMap(x, y);
	printf("▣");
	size++;
}
void DeQueue() {
	GotoMap(snake[size - 1].x, snake[size - 1].y);
	printf("  ");
	map[snake[size - 1].y][snake[size - 1].x] = 0;
	snake[size - 1].x = snake[size - 1].y = -1;
	size--;
}
void SwitchingConsoleCursor(bool flag) {
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 10;
	cursorInfo.bVisible = flag;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
} //커서 보이기/숨기기 함수
void ConsoleSize(int w, int h) {
	char command[60];
	sprintf(command, "mode con: cols=%d lines=%d", w, h);
	system(command);
} //콘솔창 크기 조절 함수
void main_s() { //메인 화면
	puts("┌──────────────────────────────────────┐");
	puts("│                                      │");
	puts("│   ■■■■■■■■■▣  ◎           │");
	puts("│   ■                  □     □□    │");
	puts("│   ■■■ □□   □□  □ □ □  □   │");
	puts("│       ■ □ □ □ □  □□  □□     │");
	puts("│   ■■■ □ □ □□□ □ □  □□□  │");
	puts("│                                      │");
	puts("│      ■■■■■■■■▣   ◎         │");
	puts("│      ■                      □□    │");
	puts("│      ■  ■  □□   □□□  □  □   │");
	puts("│      ■  ■ □ □  □ □ □ □□     │");
	puts("│      ■■■ □□□ □ □ □  □□□  │");
	puts("│                                      │");
	puts("│  ●─────  스 네 이 크 게 임 ─────●  │");
	puts("│                                      │");
	puts("│          ┌────────────────┐          │");
	puts("│          │ 1   게임 시작  │          │");
	puts("│          └────────────────┘          │");
	puts("│          ┌────────────────┐          │");
	puts("│          │ 2   게임 설정  │          │");
	puts("│          └────────────────┘          │");
	printf("└──────────────────────────────────────┘");
} //Main_Screen
void setting_s() { //선택 화면
	puts("┌───────────────────────────────┐");
	puts("│               ┌──────────┐    │");
	puts("│   판 크기     │ 보통     │  < │");
	puts("│               └──────────┘    │");
	puts("│               ┌──────────┐    │");
	puts("│   아이템      │ 적음     │    │");
	puts("│               └──────────┘    │");
	puts("│               ┌──────────┐    │");
	puts("│   게임 모드   │ 1        │    │");
	puts("│               └──────────┘    │");
	puts("│                               │");
	puts("│        [space] 설정 전환      │");
	puts("│         [→] 설정값 변경      │");
	puts("│          [ESC] 나가기         │");
	printf("└───────────────────────────────┘");
} //Select_Screen
void game_s(int w, int h) { //게임 화면
	printf("┌"); for (int i = 0; i <= w * 2; i++) { printf("─"); } printf("┐\n");
	printf("│"); for (int i = 0; i <= w * 2; i++) { printf(" "); } printf("│\n");
	printf("│"); for (int i = 0; i <= w * 2; i++) { printf(" "); } printf("│\n");
	printf("├"); for (int i = 0; i <= w * 2; i++) { printf("─"); } printf("┤\n");
	for (int i = 0; i < h; i++) {
		printf("│"); for (int i = 0; i <= w * 2; i++) { printf(" "); } printf("│\n");
	}
	printf("├"); for (int i = 0; i <= w * 2; i++) { printf("─"); } printf("┤\n");
	printf("│"); for (int i = 0; i <= w * 2; i++) { printf(" "); } printf("│\n");
	printf("│"); for (int i = 0; i <= w * 2; i++) { printf(" "); } printf("│\n");
	printf("└"); for (int i = 0; i <= w * 2; i++) { printf("─"); } printf("┘");
	gotoxy(2, 1); printf("◎ SCORE : 0");
	gotoxy(2, 2); printf("방향키로 방향 전환");
} //Game_Screen
void GameOver(int h) {
	gotoxy(2, h + 5); textcolor(12); //RED
	printf("《 GAME OVER 》"); textcolor(7); // WHITEGRAY
	gotoxy(2, h + 6); printf("ESC 키로 메인화면");
	int key;
	while (1) {
		key = getch();
		if (key == ESC) return;
	}
} //Game_Screen
void RandApple(int w, int h, int item) {
	srand(time(NULL));
	while (item) {
		int x = rand() % w;
		int y = rand() % h;
		if (!map[y][x]) {
			map[y][x] = 2;
			GotoMap(x, y);
			printf("◎");
			item--;
		}
	}
} //아이템 랜덤 배치
void RandWall(int w, int h) {
	srand(time(NULL));
	while (1) {
		int x = rand() % w;
		int y = rand() % h;
		if (!map[y][x]) {
			map[y][x] = 1;
			GotoMap(x, y);
			textcolor(15); //WHITE
			printf("■");
			textcolor(7); //WHITEGRAY
			break;
		}
	}
} //벽 랜덤 생성 (MODE 2)
void RandPortal(int w, int h, int shape) {
	int x1, y1, x2, y2;
	srand(time(NULL));
	while (1) {
		x1 = rand() % w;
		y1 = rand() % h;
		if (!map[y1][x1]) {
			map[y1][x1] = 2;
			GotoMap(x1, y1);
			if (shape == 1) printf("◎");
			else if (shape == 2) printf("▨");
			else if (shape == 3) printf("♤");
			else if (shape == 4) printf("☆");
			else if (shape == 5) printf("㉿");
			break;
		}
	}
	while (1) {
		x2 = rand() % w;
		y2 = rand() % h;
		if (!map[y2][x2]) {
			map[y2][x2] = 2;
			portal[y1][x1].x = x2; portal[y1][x1].y = y2; portal[y1][x1].shape = shape;
			portal[y2][x2].x = x1; portal[y2][x2].y = y1; portal[y2][x2].shape = shape;
			GotoMap(x2, y2);
			if (shape == 1) printf("◎");
			else if (shape == 2) printf("▨");
			else if (shape == 3) printf("♤");
			else if (shape == 4) printf("☆");
			else if (shape == 5) printf("㉿");
			break;
		}
	}
} //포탈 랜덤 생성 (MODE 3)
void RandLocker(int w, int h, int shape) {
	int x1, y1, x2, y2;
	srand(time(NULL));
	while (1) {
		x1 = rand() % w;
		y1 = rand() % h;
		if (!map[y1][x1]) {
			map[y1][x1] = 3;
			GotoMap(x1, y1);
			if (shape == 1) printf("◇");
			else if (shape == 2) printf("▨");
			else if (shape == 3) printf("♤");
			else if (shape == 4) printf("☆");
			else if (shape == 5) printf("㉿");
			break;
		}
	}
	while (1) {
		x2 = rand() % w;
		y2 = rand() % h;
		if (!map[y2][x2]) {
			map[y2][x2] = 1;
			portal[y1][x1].x = x2; portal[y1][x1].y = y2; portal[y1][x1].shape = shape;
			portal[y2][x2].x = x1; portal[y2][x2].y = y1; portal[y2][x2].shape = shape;
			GotoMap(x2, y2);
			textcolor(9); //BLUE
			if (shape == 1) printf("◇");
			else if (shape == 2) printf("▨");
			else if (shape == 3) printf("♤");
			else if (shape == 4) printf("☆");
			else if (shape == 5) printf("㉿");
			textcolor(7); //WHITEGRAY
			break;
		}
	}
} //자물쇠 랜덤 생성 (MODE 5)
void Snake_Game(int w, int h, int item, int mode) {
	system("cls");
	ConsoleSize(2 * w + 4, h + 8);
	game_s(w, h);
	for (int i = 0; i < 60; i++) {
		for (int j = 0; j < 60; j++) {
			map[i][j] = 0;
			portal[i][j].x = portal[i][j].y = portal[i][j].shape = -1;
		}
	}
	for (int i = 0; i < 10000; i++)
		snake[i].x = snake[i].y = -1;
	size = 0;
	for (int i = 1; i <= 3; i++) {
		EnQueue(i, h / 2, RIGHT);
		map[h / 2][i] = 1;
	}
	if (mode == 3) {
		for (int i = 1; i <= item; i++)
			RandPortal(w, h, i);
	}
	else if (mode == 5) {
		for (int i = 1; i <= item; i++)
			RandLocker(w, h, i);
	}
	else RandApple(w, h, item);
	int key = 0, dir = RIGHT, score = 0;
	while (1) {
		if(kbhit())
			do { key = getch(); } while (key == 224);
		Sleep(100); //뱀 이동 속도
		if (key == ESC) break;
		if (key == LEFT || key == RIGHT || key == UP || key == DOWN) {
			if (((dir == LEFT || dir == RIGHT) && key != RIGHT && key != LEFT) || ((dir == UP || dir == DOWN) && key != UP && key != DOWN)) {
				dir = key;
				snake[0].dir = key;
			}
		} //180도 회전 방지
		key = 0;
		if (dir == LEFT) {
			int px = snake[0].x - 1, py = snake[0].y;
			if (px < 0 || px >= w || py < 0 || py >= h || map[py][px] == 1) {
				GameOver(h);
				return;
			}
			if (mode == 3 && portal[py][px].x != -1 && portal[py][px].y != -1) {
				GotoMap(px, py);
				printf("  ");
				map[py][px] = 0;
				int px1 = portal[py][px].x;
				int py1 = portal[py][px].y;
				portal[py][px].x = portal[py][px].y = portal[py][px].shape = -1;
				px = px1, py = py1;
			}
			EnQueue(px, py, dir);
			if (map[py][px] != 2) DeQueue();
			else { //아이템을 먹었을 때
				if (mode == 3) { //MODE 3 (포탈 모드) 일 때
					RandPortal(w, h, portal[py][px].shape); //닿은 포탈과 같은 모양의 포탈 무작위 생성
					portal[py][px].x = portal[py][px].y = portal[py][px].shape = -1;
				}
				else if (mode == 5) { //MODE 5 (자물쇠 모드) 일 떄
					RandLocker(w, h, portal[py][px].shape); //잠겨 있을 떄의 자물쇠와 같은 모양의 열쇠, 자물쇠 생성
					portal[py][px].x = portal[py][px].y = portal[py][px].shape = -1;
				}
				else {
					if (mode == 4) { //방향 역전 (MODE 4)
						GotoMap(snake[0].x, snake[0].y);
						printf("■"); //머리 꼬리로 변경
						for (int i = 0; i < size; i++) { //뱀 몸통 진행 방향 뒤집기
							if (snake[i].dir == LEFT) snake[i].dir = RIGHT;
							else if (snake[i].dir == RIGHT) snake[i].dir = LEFT;
							else if (snake[i].dir == UP) snake[i].dir = DOWN;
							else if (snake[i].dir == DOWN) snake[i].dir = UP;
						}
						for (int i = 0; i < size / 2; i++) { //큐 뒤집기
							P tmp = snake[i];
							snake[i] = snake[size - i - 1];
							snake[size - i - 1] = tmp;
						}
						GotoMap(snake[0].x, snake[0].y);
						printf("▣"); //꼬리 머리로 변경
						dir = snake[0].dir; //꼬리가 향하고 있는 방향으로 변경
					}
					RandApple(w, h, 1); //아이템 무작위 생성
				}
				score++; //점수 증가
				if (mode == 2 && score % 2) RandWall(w, h); // MODE 2 (벽 생성) 일 때 벽 생성
				gotoxy(13, 1);
				printf("%d", score); //점수 출력
			}
			if (mode == 5 && map[py][px] == 3) { // MODE 5 (자물쇠 모드) 일 떄 열쇠 먹었을 때
				map[portal[py][px].y][portal[py][px].x] = 2; //자물쇠 잠금해제
				GotoMap(portal[py][px].x, portal[py][px].y);
				printf("◎");
				portal[py][px].x = portal[py][px].y = portal[py][px].shape = -1;
			}
			map[py][px] = 1; //이동 완료
		}
		else if (dir == RIGHT) {
			int px = snake[0].x + 1, py = snake[0].y;
			if (px < 0 || px >= w || py < 0 || py >= h || map[py][px] == 1) {
				GameOver(h);
				return;
			}
			if (mode == 3 && portal[py][px].x != -1 && portal[py][px].y != -1) {
				GotoMap(px, py);
				printf("  ");
				map[py][px] = 0;
				int px1 = portal[py][px].x;
				int py1 = portal[py][px].y;
				portal[py][px].x = portal[py][px].y = portal[py][px].shape = -1;
				px = px1, py = py1;
			}
			EnQueue(px, py, dir);
			if (map[py][px] != 2) DeQueue();
			else {
				if (mode == 3) {
					RandPortal(w, h, portal[py][px].shape);
					portal[py][px].x = portal[py][px].y = portal[py][px].shape = -1;
				}
				else if (mode == 5) {
					RandLocker(w, h, portal[py][px].shape);
					portal[py][px].x = portal[py][px].y = portal[py][px].shape = -1;
				}
				else {
					if (mode == 4) { 
						GotoMap(snake[0].x, snake[0].y);
						printf("■");
						for (int i = 0; i < size; i++) {
							if (snake[i].dir == LEFT) snake[i].dir = RIGHT;
							else if (snake[i].dir == RIGHT) snake[i].dir = LEFT;
							else if (snake[i].dir == UP) snake[i].dir = DOWN;
							else if (snake[i].dir == DOWN) snake[i].dir = UP;
						}
						for (int i = 0; i < size / 2; i++) {
							P tmp = snake[i];
							snake[i] = snake[size - i - 1];
							snake[size - i - 1] = tmp;
						}
						GotoMap(snake[0].x, snake[0].y);
						printf("▣");
						dir = snake[0].dir;
					}
					RandApple(w, h, 1);
				}
				score++;
				if (mode == 2 && score % 2) RandWall(w, h);
				gotoxy(13, 1);
				printf("%d", score);
			}
			if (mode == 5 && map[py][px] == 3) {
				map[portal[py][px].y][portal[py][px].x] = 2;
				GotoMap(portal[py][px].x, portal[py][px].y);
				printf("◎");
				portal[py][px].x = portal[py][px].y = portal[py][px].shape = -1;
			}
			map[py][px] = 1;
		}
		else if (dir == UP) {
			int px = snake[0].x, py = snake[0].y - 1;
			if (px < 0 || px >= w || py < 0 || py >= h || map[py][px] == 1) {
				GameOver(h);
				return;
			}
			if (mode == 3 && portal[py][px].x != -1 && portal[py][px].y != -1) {
				GotoMap(px, py);
				printf("  ");
				map[py][px] = 0;
				int px1 = portal[py][px].x;
				int py1 = portal[py][px].y;
				portal[py][px].x = portal[py][px].y = portal[py][px].shape = -1;
				px = px1, py = py1;
			}
			EnQueue(px, py, dir);
			if (map[py][px] != 2) DeQueue();
			else {
				if (mode == 3) {
					RandPortal(w, h, portal[py][px].shape);
					portal[py][px].x = portal[py][px].y = portal[py][px].shape = -1;
				}
				else if (mode == 5) {
					RandLocker(w, h, portal[py][px].shape);
					portal[py][px].x = portal[py][px].y = portal[py][px].shape = -1;
				}
				else {
					if (mode == 4) {
						GotoMap(snake[0].x, snake[0].y);
						printf("■");
						for (int i = 0; i < size; i++) {
							if (snake[i].dir == LEFT) snake[i].dir = RIGHT;
							else if (snake[i].dir == RIGHT) snake[i].dir = LEFT;
							else if (snake[i].dir == UP) snake[i].dir = DOWN;
							else if (snake[i].dir == DOWN) snake[i].dir = UP;
						}
						for (int i = 0; i < size / 2; i++) {
							P tmp = snake[i];
							snake[i] = snake[size - i - 1];
							snake[size - i - 1] = tmp;
						}
						GotoMap(snake[0].x, snake[0].y);
						printf("▣");
						dir = snake[0].dir;
					}
					RandApple(w, h, 1);
				}
				score++;
				if (mode == 2 && score % 2) RandWall(w, h);
				gotoxy(13, 1);
				printf("%d", score);
			}
			if (mode == 5 && map[py][px] == 3) {
				map[portal[py][px].y][portal[py][px].x] = 2;
				GotoMap(portal[py][px].x, portal[py][px].y);
				printf("◎");
				portal[py][px].x = portal[py][px].y = portal[py][px].shape = -1;
			}
			map[py][px] = 1;
		}
		else if (dir == DOWN) {
			int px = snake[0].x, py = snake[0].y + 1;
			if (px < 0 || px >= w || py < 0 || py >= h || map[py][px] == 1) {
				GameOver(h);
				return;
			}
			if (mode == 3 && portal[py][px].x != -1 && portal[py][px].y != -1) {
				GotoMap(px, py);
				printf("  ");
				map[py][px] = 0;
				int px1 = portal[py][px].x;
				int py1 = portal[py][px].y;
				portal[py][px].x = portal[py][px].y = portal[py][px].shape = -1;
				px = px1, py = py1;
			}
			EnQueue(px, py, dir);
			if (map[py][px] != 2) DeQueue();
			else {
				if (mode == 3) {
					RandPortal(w, h, portal[py][px].shape);
					portal[py][px].x = portal[py][px].y = portal[py][px].shape = -1;
				}
				else if (mode == 5) {
					RandLocker(w, h, portal[py][px].shape);
					portal[py][px].x = portal[py][px].y = portal[py][px].shape = -1;
				}
				else {
					if (mode == 4) {
						GotoMap(snake[0].x, snake[0].y);
						printf("■");
						for (int i = 0; i < size; i++) {
							if (snake[i].dir == LEFT) snake[i].dir = RIGHT;
							else if (snake[i].dir == RIGHT) snake[i].dir = LEFT;
							else if (snake[i].dir == UP) snake[i].dir = DOWN;
							else if (snake[i].dir == DOWN) snake[i].dir = UP;
						}
						for (int i = 0; i < size / 2; i++) {
							P tmp = snake[i];
							snake[i] = snake[size - i - 1];
							snake[size - i - 1] = tmp;
						}
						GotoMap(snake[0].x, snake[0].y);
						printf("▣");
						dir = snake[0].dir;
					}
					RandApple(w, h, 1);
				}
				score++;
				if (mode == 2 && score % 2) RandWall(w, h);
				gotoxy(13, 1);
				printf("%d", score);
			}
			if (mode == 5 && map[py][px] == 3) {
				map[portal[py][px].y][portal[py][px].x] = 2;
				GotoMap(portal[py][px].x, portal[py][px].y);
				printf("◎");
				portal[py][px].x = portal[py][px].y = portal[py][px].shape = -1;
			}
			map[py][px] = 1;
		}
	}
}
int main() {
	SwitchingConsoleCursor(false);
	main_s();
	int key, w = 24, h = 21, item = 1, mode = 1;
	while (1) {
		system("cls");
		ConsoleSize(41, 23);
		main_s();
		fflush(stdin);
		key = getch();
		if (key == 49) Snake_Game(w, h, item, mode);
		else if (key == 50) {
			system("cls");
			ConsoleSize(34, 15);
			setting_s();
			gotoxy(18, 2);
			if (w == 10) printf("매우 작음");
			else if (w == 17) printf("작음     ");
			else if (w == 24) printf("보통     ");
			else if (w == 31) printf("큼       ");
			else if (w == 45) printf("거대     ");
			else if (w == 52) printf("초거대   ");
			gotoxy(18, 5);
			if (item == 1) printf("적음");
			else if (item == 3) printf("보통");
			else if (item == 5) printf("많음");
			gotoxy(18, 8);
			printf("%d", mode);
			int key1, start = 0, num = 2, sel = 1;
			int w1 = w, h1 = h, item1 = item, mode1 = mode - 1;
			while (1) {
				key1 = getch();
				if (key1 == ESC) break;
				else if (key1 == SPACE) {
					if (sel == 1) {
						num = 0;
						gotoxy(30, 2);
						printf(" ");
						gotoxy(30, 5);
						printf("<");
					}
					else if (sel == 2) {
						num = 0;
						gotoxy(30, 5);
						printf(" ");
						gotoxy(30, 8);
						printf("<");
						gotoxy(22, 11);
						printf("완료");
					}
					else if (sel == 3) {
						w = w1; h = h1; item = item1; mode = mode1 + 1;
						break;
					}
					sel++;
				}
				else if (key1 == 224) { 
					key1 = getch();
					if (key1 == RIGHT) {
						if (sel == 1) { //맵 크기
							num = (num + 1) % 6;
							if (num == 0) {
								gotoxy(18, 2);
								printf("매우 작음");
								w1 = 10; h1 = 9;
							}
							if (num == 1) {
								gotoxy(18, 2);
								printf("작음     ");
								w1 = 17; h1 = 15;
							}
							else if (num == 2) {
								gotoxy(18, 2);
								printf("보통     ");
								w1 = 24; h1 = 21;
							}
							else if (num == 3) {
								gotoxy(18, 2);
								printf("큼       ");
								w1 = 31; h1 = 27;
							}
							else if (num == 4) {
								gotoxy(18, 2);
								printf("거대     ");
								w1 = 45; h1 = 39;
							}
							else if (num == 5) {
								gotoxy(18, 2);
								printf("초거대   ");
								w1 = 52; h1 = 45;
							}
						}
						else if(sel == 2) { //아이템 개수
							num = (num + 1) % 3;
							if (num == 0) {
								gotoxy(18, 5);
								printf("적음");
								item1 = 1;
							}
							else if (num == 1) {
								gotoxy(18, 5);
								printf("보통");
								item1 = 3;
							}
							else if (num == 2) {
								gotoxy(18, 5);
								printf("많음");
								item1 = 5;
							}
						}
						else if (sel == 3) { //모드
							mode1 = (mode1 + 1) % 5; //모드 번호
							gotoxy(18, 8);
							printf("%d", mode1 + 1);
						}
					}
				}
			}
		}
	}
	return 0;
}