#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<Windows.h>
#include<time.h>
#include<conio.h>
#include"block.h"

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define SPACE 32
#define KEY_Q 113
#define KEY_W 119
#define	KEY_E 101
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
#define KEY_Z 122
#define KEY_X 120
#define KEY_C 99
#define KEY_J 106
#define KEY_K 107

#define GBOARD_WIDTH 10
#define GBOARD_HEIGHT 20

#define GBOARD_ORIGIN_X 4
#define GBOARD_ORIGIN_Y 2

#define MAXIMUM_X 110;
#define MAXIMUM_Y 27;

int block_id = 0;
int SPEED = 50;
int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2];

bool DetectColision(int posX, int posY, char blockModel[4][4]);

void setSPEED(int s);
void ScreenSaver();
void BlockControlFromKeyboard();
void PrintMapByte();
void ShowBlockGhost(char blockInfo[4][4]);
void DeleteBlockGhost(char blockInfo[4][4]);
void ShowGhost();
void DeleteGhost();
void ShowNextBlock(char blockInfo[4][4]);
void DeleteNextBlock(char blockInfo[4][4]);
bool isContain(int* arr, int size, int find);

bool GHOST_MODE = false;
bool DETECT_CHECK = false;
bool ROTATE_CORNER = false;
bool SHOW_NEXT_BLOCK = false;
bool ENABLE_CLEAR_BLOCK = false;
bool ENABLE_BOMB_BLOCK = false;



void setSPEED(int s) {

	SPEED = s / 10;

}

void SetCurrentCursorPos(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void SetCurrentCursorPos(COORD current)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), current);
}

COORD GetCurrentCursorPos(void)
{
	COORD curPoint;
	CONSOLE_SCREEN_BUFFER_INFO curInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curPoint.X = curInfo.dwCursorPosition.X;
	curPoint.Y = curInfo.dwCursorPosition.Y;

	return curPoint;
}

void GetCurrentCursorPos(int& x, int & y) {

	CONSOLE_SCREEN_BUFFER_INFO curInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	x = curInfo.dwCursorPosition.X;
	y = curInfo.dwCursorPosition.Y;

}

void RemoveCursor(void)
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void ShowBlock(char blockInfo[4][4])
{
	int x, y;
	COORD curPos = GetCurrentCursorPos();

	ShowGhost();

	for (y = 0; y<4; y++)
	{
		for (x = 0; x<4; x++)
		{

			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y);

			if (blockInfo[y][x] == 1)
				printf("■");
			else if (blockInfo[y][x] == CLEAR_BLOCK_BYTE)
				printf("★");
			else if (blockInfo[y][x] == BOMB_BLOCK_BYTE)
				printf("●");

		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);
}

void DeleteBlock(char blockInfo[4][4])
{
	int y, x;
	COORD curPos = GetCurrentCursorPos();

	DeleteGhost();

	for (y = 0; y<4; y++)
	{
		for (x = 0; x<4; x++)
		{
			SetCurrentCursorPos(curPos.X + x * 2, curPos.Y + y);

			if (blockInfo[y][x])
				printf(" ");
		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);
}

//void ShiftRight()
//{
//	COORD current = GetCurrentCursorPos();
//	int x = current.X;
//	int y = current.Y;
//	x += 2;
//
//	if (DetectColision(x, y, blockModel[block_id])) {
//		DeleteBlock(blockModel[block_id]);
//		SetCurrentCursorPos(x, y);
//		ShowBlock(blockModel[block_id]);
//	}
//}

void ShiftRight()
{
	int x, y;
	GetCurrentCursorPos(x, y);
	x += 2;
	x = x % MAXIMUM_X; // 오른쪽으로 계속 이동할 경우 범위를 넘어서면 왼쪽으로 보내줌

	if (DetectColision(x, y, blockModel[block_id])) {
		DeleteBlock(blockModel[block_id]);
		SetCurrentCursorPos(x, y);
		ShowBlock(blockModel[block_id]);
	}
}

//void ShiftLeft()
//{
//	COORD current = GetCurrentCursorPos();
//	int x = current.X;
//	int y = current.Y;
//	x -= 2;
//
//	if (DetectColision(x, y, blockModel[block_id])) {
//		DeleteBlock(blockModel[block_id]);
//		SetCurrentCursorPos(x, y);
//		ShowBlock(blockModel[block_id]);
//	}
//}

void ShiftLeft()
{
	int x, y;
	GetCurrentCursorPos(x, y);
	x -= 2;
	if (x < 0) { // 왼쪽으로 계속 이동할 경우 범위를 넘어서면 오른쪽으로 보내줌
		x = MAXIMUM_X + x;
	}

	if (DetectColision(x, y, blockModel[block_id])) {
		DeleteBlock(blockModel[block_id]);
		SetCurrentCursorPos(x, y);
		ShowBlock(blockModel[block_id]);
	}
}

bool BlockDown()
{

	int x, y;
	GetCurrentCursorPos(x, y);
	y += 1;
	y = y % MAXIMUM_Y;

	if (DetectColision(x, y, blockModel[block_id])) {
		DeleteBlock(blockModel[block_id]);
		SetCurrentCursorPos(x, y);
		ShowBlock(blockModel[block_id]);
		return true;
	}

	return false;
}

void BlockUp()
{
	int x, y;
	GetCurrentCursorPos(x, y);

	y -= 1;

	if (y < 0) {
		y = MAXIMUM_Y + y;
	}

	if (DetectColision(x, y, blockModel[block_id])) {
		DeleteBlock(blockModel[block_id]);
		SetCurrentCursorPos(x, y);
		ShowBlock(blockModel[block_id]);
	}
}

void SpaceDown() {
	while (BlockDown());
}

void RotateBlock()
{
	int x, y;
	GetCurrentCursorPos(x, y);

	int block_base = block_id - block_id % 4;
	int nextBlockId = block_base + (block_id + 1) % 4;

	if (!DetectColision(x, y, blockModel[nextBlockId])) {

		// 코너에서 회전할 수 없는 경우 이동시킨 후 회전 할 것인지
		if (ROTATE_CORNER) {

			if (!DetectColision(x + 2, y, blockModel[block_id])) {
				ShiftLeft();
				if (block_id >= 12 && block_id < 16) {
					ShiftLeft();
				}
			}
			if (!DetectColision(x - 2, y, blockModel[block_id])) {
				ShiftRight();
				/*if (block_id >= 12 && block_id < 16) {
					ShiftRight();
				}*/
			}

			DeleteBlock(blockModel[block_id]);
			block_id = nextBlockId;
			ShowBlock(blockModel[block_id]);

		}

		return;
	}

	DeleteBlock(blockModel[block_id]);
	/*int temp = (block_id + 1) % 4;
	block_id = index * 4 + temp;*/
	block_id = nextBlockId;
	ShowBlock(blockModel[block_id]);

}

void ReverseRotateBlock()
{
	int x, y;
	GetCurrentCursorPos(x, y);


	/*int index = block_id / 4;
	int temp;

	temp = (block_id + 3) % 4;
	block_id = index * 4 + temp;*/


	int block_base = block_id - block_id % 4;
	int nextBlockId = block_base + (block_id + 3) % 4;

	if (!DetectColision(x, y, blockModel[nextBlockId])) {
		return;
	}

	DeleteBlock(blockModel[block_id]);
	block_id = nextBlockId;
	ShowBlock(blockModel[block_id]);
}

void Rotate180Block()
{
	int x, y;
	GetCurrentCursorPos(x, y);

	int block_base = block_id - block_id % 4;
	int nextBlockId = block_base + (block_id + 2) % 4;

	if (!DetectColision(x, y, blockModel[nextBlockId])) {
		return;
	}

	DeleteBlock(blockModel[block_id]);
	block_id = nextBlockId;
	ShowBlock(blockModel[block_id]);

	/*int index = block_id / 4;
	int temp = (block_id + 2) % 4;
	block_id = index * 4 + temp;*/

}

bool DetectColision(int posX, int posY, char blockModel[4][4]) {

	if (DETECT_CHECK) {

		int x, y;

		int arrX = (posX - GBOARD_ORIGIN_X) / 2;
		int arrY = posY - GBOARD_ORIGIN_Y;

		for (x = 0; x<4; x++) {
			for (y = 0; y<4; y++) {

				int convertX = arrX + x;
				int convertY = arrY + y;

				if (blockModel[y][x] == 1 && gameBoardInfo[convertY][convertX]) {
					return false;
				}
				else if (blockModel[y][x] == CLEAR_BLOCK_BYTE) { // 클리어 블럭

					if(gameBoardInfo[convertY][convertX] == 1){
						gameBoardInfo[convertY][convertX] = 0;
					}
					else if (gameBoardInfo[convertY][convertX] == 9) {
						return false;
					}

				}
				else if (blockModel[y][x] == BOMB_BLOCK_BYTE && gameBoardInfo[convertY][convertX]) { // 폭탄 블럭
					return false;
				}

			}
		}

	}

	return true;
}

void ProcessKeyInput()
{

	int key;

	for (int i = 0; i <SPEED; i++)
	{

		/*COORD current = GetCurrentCursorPos();

		SetCurrentCursorPos(0, 0);

		printf("%d", i);

		SetCurrentCursorPos(current);*/

		if (_kbhit() != 0)
		{
			key = _getch();
			switch (key)
			{
			case LEFT:
				ShiftLeft();
				break;
			case RIGHT:
				ShiftRight();
				break;
			case UP:
				RotateBlock();
				break;
			case DOWN:
				return;
				break;
			case SPACE:
				SpaceDown();
				return;
				break;
			default:
				break;
			}

		}

		Sleep(10);
	}

	return;
}

bool isGameOver() {

	COORD current = GetCurrentCursorPos();

	if (!DetectColision(current.X, current.Y, blockModel[block_id])) {
		return true;
	}

	return false;

}

void DrawGameBoard() {

	// 마지막 전줄 까지만 채움
	for (int y = 0; y<GBOARD_HEIGHT; y++) {
		gameBoardInfo[y][0] = 9;
		gameBoardInfo[y][GBOARD_WIDTH + 1] = 9;
	}

	// 마지막줄을 1로 채움
	for (int x = 0; x<GBOARD_WIDTH + 2; x++) {
		gameBoardInfo[GBOARD_HEIGHT][x] = 9;
	}

	// 맵 라인 출력부
	for (int y = 0; y < GBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);

		printf("┃");
		SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + 2, GBOARD_ORIGIN_Y + y);
		printf("┃");

	}

	SetCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
	for (int x = 0; x < GBOARD_WIDTH + 2; x++) {

		if (x == 0)
		{
			printf("┗");
		}
		else if (x + 1 == GBOARD_WIDTH + 2)
		{
			printf("┛");
		}
		else
		{
			printf("━");
		}

	}



	PrintMapByte();

}

void ReDrawBlocks(void)
{
	int x, y;

	int cursX, cursY;
	for (y = 0; y<GBOARD_HEIGHT; y++) {
		for (x = 1; x<GBOARD_WIDTH + 1; x++) {
			cursX = x * 2 + GBOARD_ORIGIN_X;
			cursY = y + GBOARD_ORIGIN_Y;
			SetCurrentCursorPos(cursX, cursY);
			if (gameBoardInfo[y][x] == 1) {
				printf("■");
			}
			else {
				printf("  ");
			}
		}
	}
}

void RemoveFillUpLine() {

	int line, x, y;
	for (y = GBOARD_HEIGHT - 1; y>0; y--) {
		//완성된 라인이 있는지 검사
		for (x = 1; x<GBOARD_WIDTH + 1; x++) {

			if (isContain(gameBoardInfo[y], GBOARD_WIDTH + 2, CLEAR_BLOCK_BYTE)) {
				x = GBOARD_WIDTH + 1;
				break;
			}
			if (isContain(gameBoardInfo[y], GBOARD_WIDTH + 2, 0)) {
				break;
			}

		}

		if (x == (GBOARD_WIDTH + 1)) {
			for (line = 0; y - line>0; line++) {
				// 완성된 라인 채우기
				memcpy(&gameBoardInfo[y - line][1], &gameBoardInfo[(y - line) - 1][1], GBOARD_WIDTH * sizeof(int));
			}
			//y값 조정
			y += 1;
		}
	}

	ReDrawBlocks();

}

void AddBlockToBoard() {

	int x, y, arrCurX, arrCurY;
	int curPosX, curPosY;
	GetCurrentCursorPos(curPosX, curPosY);

	// Convert X,Y coodinate to array index
	arrCurX = (curPosX - GBOARD_ORIGIN_X) / 2;
	arrCurY = curPosY - GBOARD_ORIGIN_Y;

	if (block_id / 4 == 8) { // 폭탄 블럭일 경우 별도로 처리

		for (int j = arrCurY - 1; j < arrCurY - 1 + 4; j++) {
			for (int i = arrCurX - 1; i < arrCurX - 1 + 4; i++) {

				if (gameBoardInfo[j][i] != 9) {
					gameBoardInfo[j][i] = 0;
				}

			}
		}

		return;

	}

	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {

			if (blockModel[block_id][y][x]) {
				gameBoardInfo[arrCurY + y][arrCurX + x] = blockModel[block_id][y][x];
			}

		}
	}

}

int getBlock() {

	int	BLOCK_SIZE = 9;

	srand((unsigned int)time(NULL));

	int block;
	while (1) {
		block = (rand() % BLOCK_SIZE) * 4;
		if (block / 4 == CLEAR_BLOCK_INDEX) { // CLEAR_BLOCK이 TRUE일 경우만 적용
			if (ENABLE_CLEAR_BLOCK) {
				break;
			}
		}
		else if (block / 4 == BOMB_BLOCK_INDEX) { // BOMB_BLOCK이 TRUE일 경우만 적용
			if (ENABLE_BOMB_BLOCK) {
				break;
			}
		}
		else {
			break;
		}
	}

	return block;
}

void init() {

	// 기본설정
	RemoveCursor();
	DrawGameBoard();
	setSPEED(1000);
	GHOST_MODE = true; // 고스트를 보이고 싶다면 true아니면 false
	DETECT_CHECK = true; // 충돌을 체크할 것인가 체크하고 싶으면 true
	ROTATE_CORNER = true; // 코너에서 돌릴 수 없는 경우 이동시킨 후 회전시킬 것인가
	SHOW_NEXT_BLOCK = true; // 다음 블럭을 보여줄 것인가
	ENABLE_CLEAR_BLOCK = true; // 클리어 블럭을 사용할 것인가
	ENABLE_BOMB_BLOCK = true; // 폭탄 블럭을 사용할 것인가

}

void GameStart() {

	init();

	int next_block_id = getBlock();
	
	// 게임 반복
	while (1) {

		SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH, GBOARD_ORIGIN_Y);
		srand((unsigned int)time(NULL));
		block_id = next_block_id;
		next_block_id = getBlock();

		// 다음 블럭을 보여줌
		if (SHOW_NEXT_BLOCK) {
			bool temp = GHOST_MODE;
			GHOST_MODE = false;
			DeleteNextBlock(blockModel[block_id]);
			ShowNextBlock(blockModel[next_block_id]);
			GHOST_MODE = temp;
		}

		if (isGameOver()) {
			break;
		}

		while (1) {

			if (BlockDown() == false) {
				AddBlockToBoard();
				RemoveFillUpLine();
				PrintMapByte(); // 오른쪽에 바이트 표시
				break;
			}

			ProcessKeyInput();
			

		}

	}

	SetCurrentCursorPos(10, 2);
	puts("Game Over!!");

}

/*=========================================================================*/
/*================================= Main ==================================*/
/*=========================================================================*/

int main() {

	GameStart();

	//RemoveCursor();
	////ScreenSaver();
	//BlockControlFromKeyboard();

	getchar();

}


/*=========================================================================*/
/*=========================== Addition Function ===========================*/
/*=========================================================================*/

bool isContain(int* arr, int size, int find) {

	for (int i = 0; i < size; i++) {
		if (arr[i] == find) {
			return true;
		}
	}

	return false;

}

void ShowNextBlock(char blockInfo[4][4])
{
	int x, y;
	COORD current = GetCurrentCursorPos();

	SetCurrentCursorPos(GBOARD_WIDTH + 20, GBOARD_ORIGIN_Y);
	COORD curPos = GetCurrentCursorPos();

	ShowGhost();

	for (y = 0; y<4; y++)
	{
		for (x = 0; x<4; x++)
		{
			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y);

			if (blockInfo[y][x] == 1)
				printf("■");
			else if (blockInfo[y][x] == CLEAR_BLOCK_BYTE)
				printf("★");
			else if (blockInfo[y][x] == BOMB_BLOCK_BYTE)
				printf("●");

		}
	}
	//SetCurrentCursorPos(curPos.X, curPos.Y);

	SetCurrentCursorPos(current);
}

void DeleteNextBlock(char blockInfo[4][4])
{
	int y, x;
	COORD current = GetCurrentCursorPos();

	SetCurrentCursorPos(GBOARD_WIDTH + 20, GBOARD_ORIGIN_Y);
	COORD curPos = GetCurrentCursorPos();

	DeleteGhost();

	for (y = 0; y<4; y++)
	{
		for (x = 0; x<4; x++)
		{
			SetCurrentCursorPos(curPos.X + x * 2, curPos.Y + y);

			if (blockInfo[y][x])
				printf(" ");
		}
	}
	SetCurrentCursorPos(current);
}

void ShowBlockGhost(char blockInfo[4][4])
{
	int x, y;
	COORD curPos = GetCurrentCursorPos();
	for (y = 0; y<4; y++)
	{
		for (x = 0; x<4; x++)
		{
			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y);

			if (blockInfo[y][x] == 1)
				printf("□");

		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);
}

void DeleteBlockGhost(char blockInfo[4][4])
{
	int y, x;
	COORD curPos = GetCurrentCursorPos();

	for (y = 0; y<4; y++)
	{
		for (x = 0; x<4; x++)
		{
			SetCurrentCursorPos(curPos.X + x * 2, curPos.Y + y);

			if (blockInfo[y][x] == 1)
				printf(" ");
		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);
}

void ShowGhost() {

	if (GHOST_MODE && DETECT_CHECK && (block_id/4 != CLEAR_BLOCK_INDEX) && (block_id/4 != BOMB_BLOCK_INDEX)) {

		COORD current = GetCurrentCursorPos();
		int x = current.X;
		int y = current.Y;

		while (1) {

			if (!DetectColision(x, y, blockModel[block_id])) {
				y -= 1;
				SetCurrentCursorPos(x, y);
				DeleteBlockGhost(blockModel[block_id]);
				ShowBlockGhost(blockModel[block_id]);
				SetCurrentCursorPos(current);
				return;
			}
			y += 1;
		}
	}
}

void DeleteGhost() {

	if (GHOST_MODE && DETECT_CHECK && (block_id / 4 != CLEAR_BLOCK_INDEX) && (block_id / 4 != BOMB_BLOCK_INDEX)) {

		COORD current = GetCurrentCursorPos();
		int x = current.X;
		int y = current.Y;

		while (1) {

			if (!DetectColision(x, y, blockModel[block_id])) {
				y -= 1;
				SetCurrentCursorPos(x, y);
				DeleteBlockGhost(blockModel[block_id]);
				SetCurrentCursorPos(current);
				return;
			}
			y += 1;
		}

	}
}

void PrintMapByte() {
	// 배열의 사이즈는 HEIGHT+1까지 반복이 맞음
	// 맵 출력부
	for (int y = 0; y < GBOARD_HEIGHT + 1; y++) {
		SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH + 50, GBOARD_ORIGIN_Y + y);
		for (int x = 0; x < GBOARD_WIDTH + 2; x++) {

			printf("%d%d", gameBoardInfo[y][x], gameBoardInfo[y][x]);

		}
		printf("\n");
	}
}

void BlockControlFromKeyboard() {

	srand((unsigned int)time(NULL));
	int x = rand() % MAXIMUM_X;
	int y = rand() % MAXIMUM_Y;
	x = x + (x % 2); // 2의 배수로 만듬

	SetCurrentCursorPos(x, y);

	srand((unsigned int)time(NULL));
	block_id = (rand() % 7) * 4;


	int key;
	while (1)
	{
		ShowBlock(blockModel[block_id]);

		while (!_kbhit())
		{

		}
		key = _getch();
		switch (key)
		{
		case KEY_A:
			ShiftLeft();
			break;
		case KEY_W:
			BlockUp();
			break;
		case KEY_D:
			ShiftRight();
			break;
		case KEY_X:
			BlockDown();
			break;
		case KEY_Q:
			ShiftLeft();
			BlockUp();
			break;
		case KEY_E:
			ShiftRight();
			BlockUp();
			break;
		case KEY_Z:
			ShiftLeft();
			BlockDown();
			break;
		case KEY_C:
			ShiftRight();
			BlockDown();
			break;
		case KEY_S:
			RotateBlock();
			break;
		case KEY_J:
			ReverseRotateBlock();
			break;
		case KEY_K:
			Rotate180Block();
			break;
		default:
			break;
		}
	}

}

void ScreenSaver() {

	int x = 0, y = 0;

	int sum1 = 1;
	int sum2 = 0;

	int direction[3] = { -1, 0, 1 };

	while (1) {

		const int maximumX = MAXIMUM_X;
		const int maximumY = MAXIMUM_Y;

		srand((unsigned int)time(NULL));
		x = rand() % maximumX;
		y = rand() % maximumY;
		x = x + (x % 2);

		SetCurrentCursorPos(x, y); // 랜덤한 위치에 출력

		srand((unsigned int)time(NULL));
		int block_id = (rand() % 7) * 4; // 랜덤한 블럭 생성

		while (1) {

			// 랜덤한 방향 결정
			sum1 = direction[rand() % 3] * 2;
			sum2 = direction[rand() % 3];

			if (sum1 != 0 && sum2 != 0)
				break;

		}

		// 20번 반복 출력
		for (int i = 0; i <20; i++)
		{

			ShowBlock(blockModel[block_id + i % 4]);

			Sleep(100);

			DeleteBlock(blockModel[block_id + i % 4]);

			COORD currentPos = GetCurrentCursorPos();

			SetCurrentCursorPos(currentPos.X + 1 * sum1, currentPos.Y + 1 * sum2);

			currentPos = GetCurrentCursorPos();

			if (currentPos.X < 2 || currentPos.X > maximumX) {
				sum1 *= -1;
			}

			if (currentPos.Y < 1 || currentPos.Y > maximumY) {
				sum2 *= -1;
			}

		}

	}


	getchar();

}