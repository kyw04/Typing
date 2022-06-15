#pragma warning(disable : 4996) 
/* ������ȣ 4996�� ������ �����ϰ� ���α׷� ���� */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <string.h>

#define ENTER1 10
#define ENTER2 13
#define BACKSPACE 8
#define CTRL_BACKSPACE 127 // Ű���� ���� ��ó�� ��Ŵ 
#define WORD_MAX 100 // ��� ������ �ܾ���� �ִ� ��

// ��� �߰� ���� 
char word[WORD_MAX][25] = { "apple", "left", "right", "hello", "world", "door", "also", "always", "and", "or",
							"angry", "animal", "bad", "because", "metaphor", "use", "best", "better", "big", "bird",
							"chance", "word", "number", "result", "score", "time", "student", "class", "floor", "common",
							"could", "cow", "custom", "difficult", "different", "easy", "empty", "enough", "every", "example",
							"joke", "king", "life", "light", "little", "low", "long", "luck", "much", "new", 
							"newspaper", "note", "odor", "oil", "only", "orange", "pencil", "person", "you", "black", NULL};
int r_rand = -1;

typedef struct _Word
{
	int x = 0;
	int y = 0;
	int number;
	int lenght = 0;
	bool IsErased = true;
}Words;
typedef struct _Player
{
	int Hp = 10;
	int Score = 0;
	int Level = 1;
	char input;
}Player;
void Goto(int x, int y) // �Է� ��ġ�� �����ϴ� �Լ� 
{
	COORD Pos;
	Pos.X = 2 * x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
void SetConsoleView()
{
	system("mode con:cols=100 lines=25"); // cm(��� ������Ʈ) ũ�� ����  
	system("title typing practice"); // cmd �̸� ���� 
}
void DrawUI(Player* player)
{
	Goto(20, 0);
	printf("Level : %2d", player->Level);
	Goto(35, 0);
	printf("Hp : %2d, Score : %12d", player->Hp, player->Score);
	Goto(0, 0);
}
void DrawInput(const char* note)
{
	Goto(20, 23);
	printf("%s", note);
}
void DrawWord(Words words[], Player* player, int* count) // �������� �ܾ ����� �ִ� �Լ� 
{
	int i = 0, j = 0;
	for (i = 0; i < *count; i++)
	{
		if (words[i].y >= 25 && words[i].IsErased == false) // �ܾ ȭ�鿡 �Ⱥ��� ��� 
		{
			player->Hp -= 1;
			for (j = i; j < *count - 1; j++)
			{
				words[j].number = words[j + 1].number;
				words[j].x = words[j + 1].x;
				words[j].y = words[j + 1].y;
			}
			words[j].IsErased = true;
			(*count)--;
		}
		else if (words[i].IsErased == false)
		{
			Goto(words[i].x, words[i].y);
			printf("%s", word[words[i].number]);
		}
	}
}
void GameOverMessage(const Player player)
{
	printf("Level : %d\n", player.Level);
	printf("Score : %d\n", player.Score);

}
void AddWord(Words words[], int* count)
{
	if (r_rand == rand() % words[0].lenght) // ���������� �Ȱ��� �ܾ ������ ���� ���� 
	{
		r_rand--;
		if (words[0].lenght == 1)
			r_rand = 0;
		else if (r_rand < 0)
			r_rand = words[0].lenght - 1;
	}
	else
		r_rand = rand() % words[0].lenght;

	if (words[*count].IsErased == true)
	{
		words[*count].number = r_rand;
		words[*count].x = rand() % 45;
		words[*count].y = 1;
		words[(*count)++].IsErased = false;
	}
}
void AddWordsX(Words words[])
{
	int i;
	for (i = 0; i < WORD_MAX; i++)
	{
		if (words[i].IsErased == false)
		{
			words[i].y += 1;
		}
	}
}
int FindWord(Words words[], char* note, int* count)
{
	int i, score = 0;
	for (i = 0; i <= WORD_MAX; i++)
	{
		if (words[i].IsErased == false)
		{
			int j = 0, ret = 0, count1 = 0, count2 = 0;
			while (word[words[i].number][j] != NULL) { count1++; j++; } j = 0; // �ܾ��忡 �ִ� �ܾ� ���� ī��Ʈ 
			while (note[j] != NULL) { count2++; j++; } // ���� �� �ܾ� ���� ī��Ʈ 
			int max = count1 > count2 ? count1 : count2; // � �ܾ �� ū�� �Ǵ� 
			for (j = 0; j < max; j++)
			{
				if (word[words[i].number][j] != note[j])
				{
					ret++;
					break;
				}
			}
			if (ret == 0)
			{
				score = 10;
				if (*count > 1)
				{
					for (j = i; j < *count - 1; j++)
					{
						words[j].number = words[j + 1].number;
						words[j].x = words[j + 1].x;
						words[j].y = words[j + 1].y;
					}
					words[j].IsErased = true;
				}
				else
					words[i].IsErased = true;
				*count -= 1;
				break;
			}
		}
	}
	return score;
}
int NoteSet(Player* player, Words words[], char* note, char c, int note_index, int* word_count)
{
	if (c == ENTER1 || c == ENTER2)
	{
		player->Score += player->Level * FindWord(words, note, word_count);
		note[0] = NULL;
		note_index = 0;
	}
	else if (c == BACKSPACE)
	{
		note[note_index--] = NULL;
	}
	else if (c == CTRL_BACKSPACE)
	{
		note[0] = NULL;
		note_index = 0;
	}
	else
	{
		char temp[2];
		temp[0] = c;
		temp[1] = NULL;
		strcat(note, temp);
		note_index++;
	}
	return note_index;
}

int main(void)
{
	Words words[WORD_MAX];
	Player player;
	char note[100] = "";
	char c;
	int speed = 30, count = 0, percentage = 2;
	int game_time = 0, note_index = 0, next_level = 100;

	srand(time(NULL)); // rand �� ���� (��� �Ȱ��� ��ƾ�� �ݺ� �Ǵ� ���� ����) 
	for (int i = 0; word[i][0] != NULL; i++) // ������ �ܾ��� ���̸� Ȯ�� 
	{
		words[0].lenght += 1;
	}

	SetConsoleView(); // cmd ũ��� �̸��� �ٲٴ� �Լ� ���� 

	while (player.Hp > 0 && words[0].lenght > 0) // �÷��̾��� ü���� 0���� ũ�鼭 ������ �ܾ ���� �� �ݺ�
	{
		game_time++;

		if (_kbhit()) // input
		{
			c = _getch();
			if (c == -32)
				c = _getch();
			note_index = NoteSet(&player, words, note, c, note_index, &count);
		}

		if (speed <= game_time) // �ܾ ������ �ð�
		{
			game_time = 0;
			player.Score += 1;
			AddWordsX(words);
			if (rand() % percentage == 0) // �ܾ ���� ������ �Ǵ� 
				AddWord(words, &count);
		}

		if (player.Score >= next_level) // level up
		{
			player.Level++;
			next_level *= 2;
			if (speed > 0)
				speed -= 2;
			if (player.Level % 10 == 0)
				if (percentage > 0)
					percentage--;
		}

		DrawUI(&player); // HP, Score, Level
		DrawWord(words, &player, &count); // words
		DrawInput(note); // note
		Sleep(20); // 20�ʸ� ��ٸ� 
		system("cls"); // cmd Ŭ���� 
	}

	// game over
	if (words[0].lenght == 0) // �ܾ ���� �� 
	{
		printf("�ܾ �߰��� �ּ���!\n");
	}
	else // �ܾ �ִµ� ���ӿ��� Ż�� �Ǿ��� �� 
	{
		GameOverMessage(player);
	}

	system("pause");
	return 0;
}