#define   WIDTH        100
#define   HIGH         20	
#define   MAX_LENGHT   100	// �w�q�D���̤j����
#define   SPEED        500	// �w�q�C����l�t��

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>

// �n�Ψ쪺������ܼ�
int food_flag = 0;
int key = 72;

struct
{
	int x;
	int y;
}food;

struct
{
	int len;
	int x_buf[MAX_LENGHT];
	int y_buf[MAX_LENGHT];
	int score;
}snake;

void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void DrawMap(void)
{
	int x, y;
	for (x = 0; x < WIDTH + 4; x += 2)
	{
		y = 0;
		gotoxy(x, y);
		printf("��");
		y = HIGH + 2;
		gotoxy(x, y);
		printf("��");
	}

	for (y = 1; y < HIGH + 2; y++)
	{
		x = 0;
		gotoxy(x, y);
		printf("��");
		x = WIDTH + 2;
		gotoxy(x, y);
		printf("��");
	}

	// �N��в��X�C���ϰ�
	gotoxy(0, HIGH + 5);
}

void CreateSnake(void)
{
	int orgin_x, orgin_y;
	orgin_x = WIDTH / 2 + 2;
	orgin_y = HIGH / 2 + 1;

	snake.len = 3;
	snake.x_buf[0] = orgin_x;
	snake.y_buf[0] = orgin_y;
	snake.x_buf[1] = orgin_x;
	snake.y_buf[1] = ++orgin_y;
	snake.x_buf[2] = orgin_x;
	snake.y_buf[2] = ++orgin_y;

	snake.score = -1;

	int i;
	for (i = 0; i < snake.len; i++)
	{
		gotoxy(snake.x_buf[i], snake.y_buf[i]);
		printf("��");
	}
	gotoxy(0, HIGH + 5);
}

void CreateFood(void)
{
	if (food_flag == 0)
	{
		int flag = 0, i;
		do
		{
			srand((unsigned int)time(NULL));
			food.x = (rand() % (WIDTH / 2)) * 2 + 2;
			food.y = rand() % HIGH + 1;

			// �P�_�ͦ��������O�_�M�D�����X
			for (i = 0; i < snake.len; i++)
			{
				if (snake.x_buf[i] == food.x && snake.y_buf[i] == food.y)
				{
					flag = 1;
					break;
				}
			}

		} while (flag);

		gotoxy(food.x, food.y);
		printf("��");

		// �Y�쭹���A�h���ƥ[1
		snake.score++;

		food_flag = 1;
	}
	gotoxy(0, HIGH + 5);
}

void SnakeMove(int x, int y)
{
	// �P�_�O�_�Y�쭹���A�Y����ץ[1
	if (!food_flag)
		snake.len++;
	// �S�Y��h�٥h�̫�@�`
	else
	{
		gotoxy(snake.x_buf[snake.len - 1], snake.y_buf[snake.len - 1]);
		printf("  ");
	}
	int i;
	for (i = snake.len - 1; i > 0; i--)
	{
		snake.x_buf[i] = snake.x_buf[i - 1];
		snake.y_buf[i] = snake.y_buf[i - 1];
	}
	snake.x_buf[0] = x;
	snake.y_buf[0] = y;
	gotoxy(snake.x_buf[0], snake.y_buf[0]);
	printf("��");
	gotoxy(0, HIGH + 5);
}

void move()
{
	int pre_key = key, x, y;

	if (_kbhit())//�p�G�ϥΪ̫��U�F��L�����Y����
	{
		fflush(stdin);//�M�Žw�İϪ��r��

		//getch()Ū����V�䪺�ɭԡA�|��^�⦸�A�Ĥ@���I�s��^0�Ϊ�224�A�ĤG���I�s��^���~�O��ڭ�
		key = _getch();//�Ĥ@���I�s��^�����O��ڭ�
		key = _getch();//�ĤG���I�s��^��ڭ�
	}

	// �p�D���ʤ�V����M�W�@������V�ۤ�
	if (pre_key == 72 && key == 80)
		key = 72;
	if (pre_key == 80 && key == 72)
		key = 80;
	if (pre_key == 75 && key == 77)
		key = 75;
	if (pre_key == 77 && key == 75)
		key = 77;

	switch (key)
	{
	case 75:
		x = snake.x_buf[0] - 2;//����
		y = snake.y_buf[0];
		break;
	case 77:
		x = snake.x_buf[0] + 2;//���k
		y = snake.y_buf[0];
		break;
	case 72:
		x = snake.x_buf[0];
		y = snake.y_buf[0] - 1;//���W
		break;
	case 80:
		x = snake.x_buf[0];
		y = snake.y_buf[0] + 1;//���U
		break;
	}

	if (x == food.x&&y == food.y)
		food_flag = 0;
	SnakeMove(x, y);
}

void check(void)
{
	int i;

	// ���ѱ���
	if (snake.x_buf[0] == 0 | snake.x_buf[0] == WIDTH + 4 | snake.y_buf[0] == 0 | snake.y_buf[0] == HIGH + 2)
	{
		printf("Game Over!\n");
		exit(0);
	}
	for (i = 1; i < snake.len; i++)
	{
		if (snake.x_buf[0] == snake.x_buf[i] && snake.y_buf[0] == snake.y_buf[i])
		{
			printf("Game Over!\n");
			exit(0);
		}
	}

	// �ӧQ����
	if (snake.len == MAX_LENGHT)
	{
		printf("Your are win!\n");
		exit(0);
	}

	// �C�L�o��
	gotoxy(0, HIGH + 6);
	printf("Your score: %d", snake.score);
}

int main(void)
{
	DrawMap();
	CreateSnake();
	//printf("");
	while (1)
	{
		CreateFood();
		move();
		// �Ω󱱨�C�����t��
		Sleep(SPEED - 2 * snake.len);
		check();
	}
	system("pause");
}