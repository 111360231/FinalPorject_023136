#define   WIDTH        80
#define   HIGH         10
#define   SNAKE_MAX_LENGHT   100 
#define   SPEED        500
#define   MAX_PLAYTIMES       100  //玩貪吃蛇的最大次數
#define   clrscr()       system("cls"); //清空畫面的功能

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h> 

int food_flag = 0; //food_flag為1時食物存在，反之則否
int key = 72; //對電腦來說：key=72代表上，80代表下，75代表左，77代表右
int conti; //是否重新玩遊戲
int playtimes = 0; //玩遊戲的次數
int history[MAX_PLAYTIMES];

struct //建構food的所有屬性
{
	int x;
	int y;
}food;

struct //建構snake的所有屬性
{
	int len;
	int x_buf[SNAKE_MAX_LENGHT];
	int y_buf[SNAKE_MAX_LENGHT];
	int score;
}snake;

void gotoxy(int x, int y)
{
	COORD coord; //命coord為座標格式
	coord.X = x; //命coord.X為x座標
	coord.Y = y; //命coord.Y為y座標
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); //確保地圖大小能夠被螢幕所容納
}

void DrawMap(void)
{
	int a, b;
	for (a = 0; a < WIDTH + 4; a += 2) //畫上下兩邊的牆面
	{
		b = 0;
		gotoxy(a, b); //讓定位點移到該座標
		printf("■");
		b = HIGH + 2;
		gotoxy(a, b); //讓定位點移到新座標
		printf("■");
	}
	for (b = 1; b < HIGH + 2; b++) //畫左右兩邊的牆面(■不是正方形，其為2*1)
	{
		a = 0;
		gotoxy(a, b);
		printf("■");
		a = WIDTH + 2;
		gotoxy(a, b);
		printf("■");
	}
	gotoxy(0, HIGH + 5); //將定位點移出遊戲區域，此功能也可不存在
}

void CreateSnake(void) //創造出蛇和其初始位置，此時蛇還不會移動
{
	int orgin_x, orgin_y;
	orgin_x = (WIDTH / 2) + 2;
	orgin_y = (HIGH / 2) + 1;

	snake.len = 3;//蛇的身體由三個方塊組成
	snake.x_buf[0] = orgin_x; //定義第一個方塊的x座標
	snake.y_buf[0] = orgin_y; //定義第一個方塊的y座標
	snake.x_buf[1] = orgin_x;
	snake.y_buf[1] = ++orgin_y;
	snake.x_buf[2] = orgin_x;
	snake.y_buf[2] = ++orgin_y;

	snake.score = -1;

	int i;
	for (i = 0; i < snake.len; i++)
	{
		gotoxy(snake.x_buf[i], snake.y_buf[i]);
		printf("■");
	}
	gotoxy(0, HIGH + 5); //將定位點移出遊戲區域，此功能也可不存在
}

void CreateFood(void)
{
	if (food_flag == 0)//食物不存在時，創建出食物
	{
		int a = 0, i;
		do
		{
			srand((unsigned int)time(NULL)); //每秒產生隨機一數,並確保不會有重複的情況
			food.x = ((rand(0) % (WIDTH / 2)) * 2) + 2; //確保x座標的數值可在此程式中運作
			food.y = ((rand(0) % (HIGH / 2)) * 2) + 2;

			for (i = 0; i < snake.len; i++)
			{
				if (snake.x_buf[i] == food.x && snake.y_buf[i] == food.y) //判斷食物是否和蛇身重合
				{
					a = 1;
					break; //跳出for迴圈
				}
			}

		} while (a);

		gotoxy(food.x, food.y);
		printf("★");

		snake.score++; //吃到食物，則分數加1

		food_flag = 1; //創建完畢，食物存在
	}
	gotoxy(0, HIGH + 5);
}

void SnakeMove(int x, int y)
{
	//判斷是否吃到食物，吃到長度加1
	if (!food_flag) //讓食物從存在變成不存在，或是相反後，再進行if
		snake.len++;

	//沒吃到則繼續移動(透過抹去最後一節身體的方式)
	else
	{
		gotoxy(snake.x_buf[snake.len - 1], snake.y_buf[snake.len - 1]); //到身體最後一節的座標
		printf("  "); //抹去
	}

	//讓原本的倒數第二節變成最後一節，以此類推
	for (int i = snake.len - 1; i > 0; i--)
	{
		snake.x_buf[i] = snake.x_buf[i - 1];
		snake.y_buf[i] = snake.y_buf[i - 1];
	}

	snake.x_buf[0] = x;
	snake.y_buf[0] = y;
	gotoxy(snake.x_buf[0], snake.y_buf[0]); //在適當的位置創建出新的頭
	printf("■");
	gotoxy(0, HIGH + 5);
}

void move() //確定下一個方向為何
{
	int pre_key = key, x, y;

	if (_kbhit())//如果使用者按下了鍵盤中的某個鍵
	{
		fflush(stdin);//清空緩衝區的字元

		//getch()負責讀取方向鍵，而其會傳回兩次不同的值，第一次傳回0或224，第二次傳回的才是實際值
		key = _getch();
		key = _getch(); //第二次傳回的值才代表使用者要前進的方向
	}

	//規定移動方向不能和上一次的方向相反
	if (pre_key == 72 && key == 80)//如果(上一次方向為上&下一次方向為下)
		key = 72; //方向持續朝上
	if (pre_key == 80 && key == 72)
		key = 80;
	if (pre_key == 75 && key == 77)
		key = 75;
	if (pre_key == 77 && key == 75)
		key = 77;

	//因為此寫法是讓第三節身體消失，然後在適當的位置放置第一節身體，所以此處只需要重新定義第一節身體的位置即可
	switch (key)
	{
	case 75:
		x = snake.x_buf[0] - 2;//往左
		y = snake.y_buf[0];
		break;
	case 77:
		x = snake.x_buf[0] + 2;//往右
		y = snake.y_buf[0];
		break;
	case 72:
		x = snake.x_buf[0];
		y = snake.y_buf[0] - 1;//往上
		break;
	case 80:
		x = snake.x_buf[0];
		y = snake.y_buf[0] + 1;//往下
		break;
	}

	if (x == food.x&&y == food.y) //如果蛇和食物重合
		food_flag = 0; //代表食物不存在
	SnakeMove(x, y); //執行讓蛇移動的副程式
}


int check(void)
{
	int i;
	int stop = 0;  //遊戲停止，stop=1

	// 失敗條件1：蛇撞到牆
	if (snake.x_buf[0] == 0 | snake.x_buf[0] == WIDTH + 4 | snake.y_buf[0] == 0 | snake.y_buf[0] == HIGH + 2)
	{
		printf("Game Over!\n");
		stop = 1;
	}
	else
	{
		// 失敗條件2：蛇撞到自己
		for (i = 1; i < snake.len; i++)  //用迴圈檢查蛇的頭是否與身體重合
		{
			if (snake.x_buf[0] == snake.x_buf[i] && snake.y_buf[0] == snake.y_buf[i])
			{
				printf("You lose!\n");
				stop = 1;
			}
		}

		// 勝利條件
		if (snake.len == SNAKE_MAX_LENGHT)  //蛇的長度100
		{
			printf("You win!\n");
			stop = 1;
		}

		// 列印得分
		else
		{
			gotoxy(0, HIGH + 6);  //定位點移至(0,16)列印得分的地方
			printf("Your score: %d", snake.score);
		}
	}
	if (stop)  //stop=1，遊戲停止
	{
		history[playtimes] = snake.score;  //儲存遊戲歷史紀錄
		gotoxy(0, HIGH + 7);  //定位點移置(0,17)
		printf("輸入1以重新玩遊戲：");
		scanf_s("%d", &conti);
		if (conti == 1)  //要繼續遊戲
		{
			clrscr();  //去除之前顯示的文字，清空螢幕(清除文本函數)
			gotoxy(0, 0);  //定位點移置(0,0)
			food_flag = 0;  //食物不存在
		}
		else  //不要繼續遊戲
		{
			exit(0);  //正常退出遊戲
		}
		return 0;
	}
	else  //stop=0，遊戲還沒結束
	{
		return 1;
	}
}

void welcome()  //歡迎介面
{
	gotoxy(0, 0);  //跳到螢幕的最上方座標再印出規則
	printf("歡迎來到[貪吃蛇 Snake] !\n\n");
	printf("規則如下:\n");
	printf("1.用上下左右鍵操控貪吃蛇\n");
	printf("2.蛇若碰到自己也視為失敗\n");
	printf("3.隨著分數增加，蛇也會越跑越快喔 !");
}

void HistoryRecord() //顯示歷史紀錄
{
	gotoxy(0, 9);//跳到座標(0,9)顯示
	if (playtimes)//用玩得次數來當判斷條件
	{
		printf("※歷史紀錄：\n");
		for (int i = 0; i < playtimes; i++)//原本設的次數為0，故用此迴圈來進行判斷和累加
		{
			gotoxy(0, 10 + i);
			printf("[第 %d 次遊戲分數是 %d]", (i + 1), history[i]);//顯示遊玩的次數和分數
		}
	}
	else
	{
		printf("※歷史紀錄：無\n");
	}
}



void GameLevel()
{
	int out, level;
	gotoxy(0, 7);
	printf("輸入遊戲難度1~5(1為最簡單，5為最困難)：");
	scanf_s("%d", &level);
	clrscr();
	fflush(stdin);

	//遊戲介面
	gotoxy(0, 0);
	DrawMap();
	CreateSnake();
	do
	{
		CreateFood();
		move();
		Sleep(SPEED - (20 * snake.len * level)); //控制蛇停止的時間來控制其速度
		out = check(); //檢查是勝利還是失敗，都不是的話就繼續while迴圈
	} while (out == 1);

}

int main(void)
{
	while (1)
	{
		welcome(); //顯示歡迎畫面
		HistoryRecord(); //顯示歷史紀錄
		GameLevel(); //選擇遊戲難關
		playtimes += 1; //遊玩次數加一
	}
}
