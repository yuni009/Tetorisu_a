#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<conio.h>

#define FIELD_WIDTH (10)
#define FIELD_HEIGHT (20)

#define SHAPE_WIDTH_MAX (4)
#define SHAPE_HEIGHT_MAX (4)

#define fps (1)
#define INTERVAL (1000/fps)
enum {
	SHAPE_I,
	SHAPE_O,
	SHAPE_S,
	SHAPE_Z,
	SHAPE_J,
	SHAPE_L,
	SHAPE_T,
	SHAPE_MAX
};
typedef struct {
	int width;
	int	height;
	int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
}SHAPE;

typedef struct {
	int x, y;
	SHAPE shape;
}MINO;

SHAPE shapes[SHAPE_MAX] = {
	//SHAPE_I,
	{4,4,//int width,height
		{//int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX]
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0},
			{0,0,0,0}
		}
	},
	//SHAPE_O,
	{2,2,
		{
			{1,1},
			{1,1}
		}
	},
	//SHAPE_S,
	{3,3,
		{
			{0,1,1},
			{1,1,0},
			{0,0,0}
		}
	},
	//SHAPE_Z,
	{3,3,
		{
			{1,1,0},
			{0,1,1},
			{0,0,0}
		}
	},
	//SHAPE_J,
	{3,3,
		{
			{1,0,0},
			{1,1,1},
			{0,0,0}
		}
	},
	//SHAPE_L,
	{3,3,
		{
			{0,0,1},
			{1,1,1},
			{0,0,0}
		}
	},
	//SHAPE_T,
	{3,3,
		{
			{0,1,0},
			{1,1,1},
			{0,0,0}
		}
	},
};

//グローバル関数
int field[FIELD_HEIGHT][FIELD_WIDTH];
int screen[FIELD_HEIGHT][FIELD_WIDTH];
MINO mino;

bool MinoIntersectField() {
	for (int y = 0; y < mino.shape.height; y++) {
		for (int x = 0; x < mino.shape.width; x++) {
			if (mino.shape.pattern[y][x]) {
				if ((mino.y + y < 0) || (mino.y + y >= FIELD_HEIGHT) || (mino.x + x < 0) || (mino.x+x >= FIELD_WIDTH)) {
					return true;
				}

				if (field[mino.y + y][mino.x + x]) {
					return true;
				}
			}
		}
	}
	return false;
}

void DrawScreen() {
	memcpy(screen, field, sizeof(field));

	for (int y = 0; y < mino.shape.height; y++) {
		for (int x = 0; x < mino.shape.width; x++) {
			if (mino.shape.pattern[y][x]) {
				screen[mino.y + y][mino.x + x] |= 1;
			}
		}
	}

	system("cls"); 
	//フィールド作成
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		printf("□");
		for (int x = 0; x < FIELD_WIDTH; x++) {
			printf("%s",screen[y][x]?"▦":" ");
		}
		printf("□");
		printf("\n");
	}
	for (int x = 0; x < FIELD_WIDTH+2; x++) {
		printf("□");
	}
}

void InitMino() {
	mino.shape = shapes[rand() % +SHAPE_MAX];
	mino.x = (FIELD_WIDTH - mino.shape.width) / 2;
	mino.y = 0;
}

void Init(){
	memset(field, 0, sizeof(field));

	InitMino();

	DrawScreen();
}

int main() {
	srand((unsigned int)time(NULL));
	Init();

	clock_t lastClock = clock();
	while (1) {
		clock_t nowClock = clock();
		if (nowClock >= lastClock+INTERVAL) {
			lastClock = nowClock;
			
			MINO lastMino_auto = mino;
			mino.y++;

			if (MinoIntersectField()) {
				mino = lastMino_auto;

				for (int y = 0; y < mino.shape.height; y++) {
					for (int x = 0; x < mino.shape.width; x++) {
						if (mino.shape.pattern[y][x]) {
							field[mino.y + y][mino.x + x] = 1;
						}
					}
				}
				for (int y = 0; y < FIELD_HEIGHT; y++) {
					bool completed = true;
					for (int x = 0; x < FIELD_WIDTH; x++) {
						if (!field[y][x]) {
							completed = false;
							break;
						}
					}
					if (completed) {
						for (int x = 0; x < FIELD_WIDTH; x++) {
							field[y][x] = 0;
						}

						for (int y2 = y; y2 >= 1; y2--) {
							for (int x = 0; x < FIELD_WIDTH; x++) {
								field[y2][x] = field[y2 - 1][x];
								field[y2-1][x] = 0;
								}
						}
					}
				}
				InitMino();
			}
			DrawScreen();
		}
		if (_kbhit()) {
			MINO lastMino = mino;
			switch(_getch()){
			case 'w':
				break;
			case 's':
				mino.y++;
				break;
			case 'a':
				mino.x--;
				break;
			case 'd':
				mino.x++;
				break;
			default:
				MINO newMino=mino;
				
				for (int y = 0; y < mino.shape.height; y++) {
					for (int x = 0; x < mino.shape.width; x++) {
						newMino.shape.pattern[mino.shape.width - 1 - x][y] = mino.shape.pattern[y][x];
						
					}
				}
				mino = newMino;
				break;
			}

			if(MinoIntersectField()) {
				mino = lastMino;
			}
			DrawScreen();
		}
	}
}