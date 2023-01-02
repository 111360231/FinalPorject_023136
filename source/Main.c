#define   WIDTH        80
#define   HIGH         10
#define   SNAKE_MAX_LENGHT   100 
#define   SPEED        500
#define   MAX_PLAYTIMES       100  //���g�Y�D���̤j����
#define   clrscr()       system("cls"); //�M�ŵe�����\��

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h> 

int food_flag = 0; //food_flag��1�ɭ����s�b�A�Ϥ��h�_
int key = 72; //��q���ӻ��Gkey=72�N��W�A80�N��U�A75�N���A77�N��k
int conti; //�O�_���s���C��
int playtimes = 0; //���C��������
int history[MAX_PLAYTIMES];

struct //�غcfood���Ҧ��ݩ�
{
	int x;
	int y;
}food;

struct //�غcsnake���Ҧ��ݩ�
{
	int len;
	int x_buf[SNAKE_MAX_LENGHT];
	int y_buf[SNAKE_MAX_LENGHT];
	int score;
}snake;

void gotoxy(int x, int y)
{
	COORD coord; //�Rcoord���y�Ю榡
	coord.X = x; //�Rcoord.X��x�y��
	coord.Y = y; //�Rcoord.Y��y�y��
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); //�T�O�a�Ϥj�p����Q�ù��Үe��
}

void DrawMap(void)
{
	int a, b;
	for (a = 0; a < WIDTH + 4; a += 2) //�e�W�U���䪺��
	{
		b = 0;
		gotoxy(a, b); //���w���I����Ӯy��
		printf("��");
		b = HIGH + 2;
		gotoxy(a, b); //���w���I����s�y��
		printf("��");
	}
	for (b = 1; b < HIGH + 2; b++) //�e���k���䪺��(�����O����ΡA�䬰2*1)
	{
		a = 0;
		gotoxy(a, b);
		printf("��");
		a = WIDTH + 2;
		gotoxy(a, b);
		printf("��");
	}
	gotoxy(0, HIGH + 5); //�N�w���I���X�C���ϰ�A���\��]�i���s�b
}

void CreateSnake(void) //�гy�X�D�M���l��m�A���ɳD�٤��|����
{
	int orgin_x, orgin_y;
	orgin_x = (WIDTH / 2) + 2;
	orgin_y = (HIGH / 2) + 1;

	snake.len = 3;//�D������ѤT�Ӥ���զ�
	snake.x_buf[0] = orgin_x; //�w�q�Ĥ@�Ӥ����x�y��
	snake.y_buf[0] = orgin_y; //�w�q�Ĥ@�Ӥ����y�y��
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
	gotoxy(0, HIGH + 5); //�N�w���I���X�C���ϰ�A���\��]�i���s�b
}

void CreateFood(void)
{
	if (food_flag == 0)//�������s�b�ɡA�ЫإX����
	{
		int a = 0, i;
		do
		{
			srand((unsigned int)time(NULL)); //�C�����H���@��,�ýT�O���|�����ƪ����p
			food.x = ((rand(0) % (WIDTH / 2)) * 2) + 2; //�T�Ox�y�Ъ��ƭȥi�b���{�����B�@
			food.y = ((rand(0) % (HIGH / 2)) * 2) + 2;

			for (i = 0; i < snake.len; i++)
			{
				if (snake.x_buf[i] == food.x && snake.y_buf[i] == food.y) //�P�_�����O�_�M�D�����X
				{
					a = 1;
					break; //���Xfor�j��
				}
			}

		} while (a);

		gotoxy(food.x, food.y);
		printf("��");

		snake.score++; //�Y�쭹���A�h���ƥ[1

		food_flag = 1; //�Ыا����A�����s�b
	}
	gotoxy(0, HIGH + 5);
}

void SnakeMove(int x, int y)
{
	//�P�_�O�_�Y�쭹���A�Y����ץ[1
	if (!food_flag) //�������q�s�b�ܦ����s�b�A�άO�ۤϫ�A�A�i��if
		snake.len++;

	//�S�Y��h�~�򲾰�(�z�L�٥h�̫�@�`���骺�覡)
	else
	{
		gotoxy(snake.x_buf[snake.len - 1], snake.y_buf[snake.len - 1]); //�쨭��̫�@�`���y��
		printf("  "); //�٥h
	}

	//���쥻���˼ƲĤG�`�ܦ��̫�@�`�A�H������
	for (int i = snake.len - 1; i > 0; i--)
	{
		snake.x_buf[i] = snake.x_buf[i - 1];
		snake.y_buf[i] = snake.y_buf[i - 1];
	}

	snake.x_buf[0] = x;
	snake.y_buf[0] = y;
	gotoxy(snake.x_buf[0], snake.y_buf[0]); //�b�A����m�ЫإX�s���Y
	printf("��");
	gotoxy(0, HIGH + 5);
}

void move() //�T�w�U�@�Ӥ�V����
{
	int pre_key = key, x, y;

	if (_kbhit())//�p�G�ϥΪ̫��U�F��L�����Y����
	{
		fflush(stdin);//�M�Žw�İϪ��r��

		//getch()�t�dŪ����V��A�Ө�|�Ǧ^�⦸���P���ȡA�Ĥ@���Ǧ^0��224�A�ĤG���Ǧ^���~�O��ڭ�
		key = _getch();
		key = _getch(); //�ĤG���Ǧ^���Ȥ~�N��ϥΪ̭n�e�i����V
	}

	//�W�w���ʤ�V����M�W�@������V�ۤ�
	if (pre_key == 72 && key == 80)//�p�G(�W�@����V���W&�U�@����V���U)
		key = 72; //��V����¤W
	if (pre_key == 80 && key == 72)
		key = 80;
	if (pre_key == 75 && key == 77)
		key = 75;
	if (pre_key == 77 && key == 75)
		key = 77;

	//�]�����g�k�O���ĤT�`��������A�M��b�A����m��m�Ĥ@�`����A�ҥH���B�u�ݭn���s�w�q�Ĥ@�`���骺��m�Y�i
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

	if (x == food.x&&y == food.y) //�p�G�D�M�������X
		food_flag = 0; //�N�������s�b
	SnakeMove(x, y); //�������D���ʪ��Ƶ{��
}


int check(void)
{
	int i;
	int stop = 0;  //�C������Astop=1

	// ���ѱ���1�G�D������
	if (snake.x_buf[0] == 0 | snake.x_buf[0] == WIDTH + 4 | snake.y_buf[0] == 0 | snake.y_buf[0] == HIGH + 2)
	{
		printf("Game Over!\n");
		stop = 1;
	}
	else
	{
		// ���ѱ���2�G�D����ۤv
		for (i = 1; i < snake.len; i++)  //�ΰj���ˬd�D���Y�O�_�P���魫�X
		{
			if (snake.x_buf[0] == snake.x_buf[i] && snake.y_buf[0] == snake.y_buf[i])
			{
				printf("You lose!\n");
				stop = 1;
			}
		}

		// �ӧQ����
		if (snake.len == SNAKE_MAX_LENGHT)  //�D������100
		{
			printf("You win!\n");
			stop = 1;
		}

		// �C�L�o��
		else
		{
			gotoxy(0, HIGH + 6);  //�w���I����(0,16)�C�L�o�����a��
			printf("Your score: %d", snake.score);
		}
	}
	if (stop)  //stop=1�A�C������
	{
		history[playtimes] = snake.score;  //�x�s�C�����v����
		gotoxy(0, HIGH + 7);  //�w���I���m(0,17)
		printf("��J1�H���s���C���G");
		scanf_s("%d", &conti);
		if (conti == 1)  //�n�~��C��
		{
			clrscr();  //�h�����e��ܪ���r�A�M�ſù�(�M���奻���)
			gotoxy(0, 0);  //�w���I���m(0,0)
			food_flag = 0;  //�������s�b
		}
		else  //���n�~��C��
		{
			exit(0);  //���`�h�X�C��
		}
		return 0;
	}
	else  //stop=0�A�C���٨S����
	{
		return 1;
	}
}

void welcome()  //�w�虜��
{
	gotoxy(0, 0);  //����ù����̤W��y�ЦA�L�X�W�h
	printf("�w��Ө�[�g�Y�D Snake] !\n\n");
	printf("�W�h�p�U:\n");
	printf("1.�ΤW�U���k��ޱ��g�Y�D\n");
	printf("2.�D�Y�I��ۤv�]��������\n");
	printf("3.�H�ۤ��ƼW�[�A�D�]�|�V�]�V�ֳ� !");
}

void HistoryRecord() //��ܾ��v����
{
	gotoxy(0, 9);//����y��(0,9)���
	if (playtimes)//�Ϊ��o���ƨӷ�P�_����
	{
		printf("�����v�����G\n");
		for (int i = 0; i < playtimes; i++)//�쥻�]�����Ƭ�0�A�G�Φ��j��Ӷi��P�_�M�֥[
		{
			gotoxy(0, 10 + i);
			printf("[�� %d ���C�����ƬO %d]", (i + 1), history[i]);//��ܹC�������ƩM����
		}
	}
	else
	{
		printf("�����v�����G�L\n");
	}
}



void GameLevel()
{
	int out, level;
	gotoxy(0, 7);
	printf("��J�C������1~5(1����²��A5���̧x��)�G");
	scanf_s("%d", &level);
	clrscr();
	fflush(stdin);

	//�C������
	gotoxy(0, 0);
	DrawMap();
	CreateSnake();
	do
	{
		CreateFood();
		move();
		Sleep(SPEED - (20 * snake.len * level)); //����D����ɶ��ӱ����t��
		out = check(); //�ˬd�O�ӧQ�٬O���ѡA�����O���ܴN�~��while�j��
	} while (out == 1);

}

int main(void)
{
	while (1)
	{
		welcome(); //����w��e��
		HistoryRecord(); //��ܾ��v����
		GameLevel(); //��ܹC������
		playtimes += 1; //�C�����ƥ[�@
	}
}
