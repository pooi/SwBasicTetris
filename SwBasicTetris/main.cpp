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
int SPEED = 1000;
int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2];

bool DetectColision(int posX, int posY, char blockModel[4][4]);

void ScreenSaver();
void BlockControlFromKeyboard();
void PrintMapByte();
void ShowBlockGhost(char blockInfo[4][4]);
void DeleteBlockGhost(char blockInfo[4][4]);
void ShowGhost();
void DeleteGhost();
void ShowNextBlock(char blockInfo[4][4]);
void DeleteNextBlock(char blockInfo[4][4]);

bool GHOST_MODE = true;
bool DETECT_CHECK = true;
bool ROTATE_CORNER = false;
bool SHOW_NEXT_BLOCK = true;


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
				printf("��");
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

			if (blockInfo[y][x] == 1)
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
	x = x % MAXIMUM_X; // ���������� ��� �̵��� ��� ������ �Ѿ�� �������� ������

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
	if (x < 0) { // �������� ��� �̵��� ��� ������ �Ѿ�� ���������� ������
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

		// �ڳʿ��� ȸ���� �� ���� ��� �̵���Ų �� ȸ�� �� ������
		if (ROTATE_CORNER) {

			if (!DetectColision(x + 2, y, blockModel[block_id])) {
				ShiftLeft();
			}
			if (!DetectColision(x - 2, y, blockModel[block_id])) {
				ShiftRight();
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

				if (blockModel[y][x] && gameBoardInfo[arrY + y][arrX + x] == 1) {
					return false;
				}

			}
		}

	}

	return true;
}

bool ProcessKeyInput()
{
	int key;

	for (int i = 0; i <SPEED; i++)
	{

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
				return false;
				break;
			case SPACE:
				SpaceDown();
				return true;
				break;
			default:
				break;
			}

		}

		Sleep(1);
	}

	return false;
}

bool isGameOver() {

	COORD current = GetCurrentCursorPos();

	if (!DetectColision(current.X, current.Y, blockModel[block_id])) {
		return true;
	}

	return false;

}

void DrawGameBoard() {

	// ������ ���� ������ ä��
	for (int y = 0; y<GBOARD_HEIGHT; y++) {
		gameBoardInfo[y][0] = 1;
		gameBoardInfo[y][GBOARD_WIDTH + 1] = 1;
	}

	// ���������� 1�� ä��
	for (int x = 0; x<GBOARD_WIDTH + 2; x++) {
		gameBoardInfo[GBOARD_HEIGHT][x] = 1;
	}

	// �� ���� ��º�
	for (int y = 0; y < GBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);

		printf("��");
		SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + 2, GBOARD_ORIGIN_Y + y);
		printf("��");

	}

	SetCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
	for (int x = 0; x < GBOARD_WIDTH + 2; x++) {

		if (x == 0)
		{
			printf("��");
		}
		else if (x + 1 == GBOARD_WIDTH + 2)
		{
			printf("��");
		}
		else
		{
			printf("��");
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
				printf("��");
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
		//�ϼ��� ������ �ִ��� �˻�
		for (x = 1; x<GBOARD_WIDTH + 1; x++) {
			if (gameBoardInfo[y][x] != 1) {
				break;
			}
		}

		if (x == (GBOARD_WIDTH + 1)) {
			for (line = 0; y - line>0; line++) {
				// �ϼ��� ���� ä���
				memcpy(&gameBoardInfo[y - line][1], &gameBoardInfo[(y - line) - 1][1], GBOARD_WIDTH * sizeof(int));
			}
			//y�� ����
			y += 1;
		}
	}

	ReDrawBlocks();

}

void AddBlockToBoard() {

	int x, y, arrCurX, arrCurY;
	int curPosX, curPosY;
	GetCurrentCursorPos(curPosX, curPosY);

	for (y = 0; y<4; y++) {
		for (x = 0; x<4; x++) {

			// Convert X,Y coodinate to array index
			arrCurX = (curPosX - GBOARD_ORIGIN_X) / 2;
			arrCurY = curPosY - GBOARD_ORIGIN_Y;

			if (blockModel[block_id][y][x] == 1) {
				gameBoardInfo[arrCurY + y][arrCurX + x] = 1;
			}

		}
	}

}

void init() {

	// �⺻����
	RemoveCursor();
	DrawGameBoard();
	GHOST_MODE = true; // ��Ʈ�� ���̰� �ʹٸ� true�ƴϸ� false
	DETECT_CHECK = true; // �浹�� üũ�� ���ΰ� üũ�ϰ� ������ true
	ROTATE_CORNER = true; // �ڳʿ��� ���� �� ���� ��� �̵���Ų �� ȸ����ų ���ΰ�
	SHOW_NEXT_BLOCK = false; // ���� ���� ������ ���ΰ�

}

void GameStart() {

	init();

	srand((unsigned int)time(NULL));
	int next_block_id = (rand() % 7) * 4;
	// ���� �ݺ�
	while (1) {

		SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH, GBOARD_ORIGIN_Y - 2);
		srand((unsigned int)time(NULL));
		block_id = next_block_id;
		next_block_id = (rand() % 7) * 4;

		// ���� ���� ������
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
				PrintMapByte(); // �����ʿ� ����Ʈ ǥ��
				break;
			}
			ProcessKeyInput();

		}

	}

	SetCurrentCursorPos(10, 2);
	puts("Game Over!!");

}

int main() {

	GameStart();

	//RemoveCursor();
	//DETECT_CHECK = false;
	////ScreenSaver();
	//BlockControlFromKeyboard();

	getchar();

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
				printf("��");
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

			if (blockInfo[y][x] == 1)
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
				printf("��");
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

	if (GHOST_MODE && DETECT_CHECK) {

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

	if (GHOST_MODE && DETECT_CHECK) {

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
	// �迭�� ������� HEIGHT+1���� �ݺ��� ����
	// �� ��º�
	for (int y = 0; y < GBOARD_HEIGHT + 1; y++) {
		SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH + 50, GBOARD_ORIGIN_Y + y);
		for (int x = 0; x < GBOARD_WIDTH + 2; x++) {
			if (gameBoardInfo[y][x] == 1) {
				printf("11");
			}
			else {
				printf("00");
			}
		}
		printf("\n");
	}
}

void BlockControlFromKeyboard() {

	srand((unsigned int)time(NULL));
	int x = rand() % MAXIMUM_X;
	int y = rand() % MAXIMUM_Y;
	x = x + (x % 2); // 2�� ����� ����

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

		SetCurrentCursorPos(x, y); // ������ ��ġ�� ���

		srand((unsigned int)time(NULL));
		int block_id = (rand() % 7) * 4; // ������ �� ����

		while (1) {

			// ������ ���� ����
			sum1 = direction[rand() % 3] * 2;
			sum2 = direction[rand() % 3];

			if (sum1 != 0 && sum2 != 0)
				break;

		}

		// 20�� �ݺ� ���
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