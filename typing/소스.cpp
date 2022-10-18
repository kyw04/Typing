#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <string.h>

#define ENTER1 10
#define ENTER2 13
#define BACKSPACE 8
#define CTRL_BACKSPACE 127 // 키들의 값을 전처리 시킴 
#define WORD_MAX 500 // 사용 가능한 단어들의 최대 값

// 영어만 추가 가능 
char word[WORD_MAX][25] = { "apple", "left", "right", "hello", "world", "door", "also", "always", "and", "or",
							"angry", "animal", "bad", "because", "metaphor", "use", "best", "better", "big", "bird",
							"chance", "word", "number", "result", "score", "time", "student", "class", "floor", "common",
							"could", "cow", "custom", "difficult", "different", "easy", "empty", "enough", "every", "example",
							"joke", "king", "life", "light", "little", "low", "long", "luck", "much", "new", 
							"newspaper", "note", "odor", "oil", "only", "orange", "pencil", "person", "you", "black", 
                            "red", "yellow", "phone", "paper", "book", "screen", "key", "city", "backpack", "music",
                            "rice", "dish", "tree", "forest", "weather", "coke", "noddle", "dukyoung", "school", "glasses",
                            "pain", "bottle", "head", "shirt", "succes", "effort", "banana", "bell", "tape", "hand",
                            "finger", "face", "keybord", "shoes", "car", "bike", "kick", "gray", "white", "blue", 
                            "green", "button", "chair", "table", "fish", "sea", "send", "coconat", "desk", "camera",
                            "work", "knee", "war", "snow", "eyes", "foot", "heart", "mother", "father", "brother",
                            "sisther", "baby", "shark", "hero", "sofa", "filed", "hair", "stairs", "ear", "seed", "sprout",
                            "block", "window", "basket", "pig", "dragon", "roll", "duck", "dog", "puppy", "cat", 
                            "water", "elephant", "flower", "gift", "switch", "box", "apartment", "carrot", "grape", "wheel",
                            "paint", "handle", "television", "queen", "research", "wife", "wonder", "woman", "welcome", "effect",
                            "day", "minute", "hour", "second", "mouse", "knife", "mask", "bear", "butterfly", "beautiful",
                            "korea", "japan", "usa", "russia", "chaina", "plus", "minus", "multiply", "division", "shape",
                            "palce", "position", "pollution", "earth", "sun", "son", "smoke", "coffee", "warm", "cold",
                            "headset", "glue", "post", "memo", "monkey", "snake", "chicken", "sheep", "tiger", "lion",
                            "bedding", "make", "think", "track", "star", "space", "ship", "boat", "eraser", "language",
                            "grand", "soccer", "baseball", "fire", "down", "up", "leaf", "rabbit", "human", "great",
                            "nice", "perfect", "line", "lie", "ticket", "sound", "cloud", "bulb", "old", "rnak",
                            "fly", "air", "airplane", "movie", "helicopter", "message", "art", "bot", "girl", "man",
                            "lift", "umbrella", "broom", "drink", "hospital", "watch", "leg", "game", "song", "behind",
                            NULL};
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

void Goto(int x, int y) // 입력 위치를 변경하는 함수 
{
	COORD Pos;
	Pos.X = 2 * x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void SetConsoleView()
{
	system("mode con:cols=100 lines=25"); // cm(명령 프롬프트) 크기 설정  
	system("title typing practice"); // cmd 이름 설정 
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

void DrawWord(Words words[], Player* player, int* count) // 떨어지는 단어를 출력해 주는 함수 
{
	int i = 0, j = 0;
	for (i = 0; i < *count; i++)
	{
		if (words[i].y >= 25 && words[i].IsErased == false) // 단어가 화면에 안보일 경우 
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
	if (r_rand == rand() % words[0].lenght) // 연속적으로 똑같은 단어가 나오는 것을 방지 
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
			while (word[words[i].number][j] != NULL) { count1++; j++; } j = 0; // 단어장에 있는 단어 길이 카운트 
			while (note[j] != NULL) { count2++; j++; } // 내가 쓴 단어 길이 카운트 
			int max = count1 > count2 ? count1 : count2; // 어떤 단어가 더 큰지 판단 
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
				Beep(600, 100);
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
	if (c == 0)
		return note_index;
		
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

	srand(time(NULL)); // rand 값 변경 (계속 똑같은 루틴이 반복 되는 것을 방지) 
	for (int i = 0; word[i][0] != NULL; i++) // 떨어질 단어의 길이를 확인 
	{
		words[0].lenght += 1;
	}

	SetConsoleView(); // cmd 크기와 이름을 바꾸는 함수 실행 

	while (player.Hp > 0 && words[0].lenght > 0) // 플레이어의 체력이 0보다 크면서 떨어질 단어가 있을 때 반복
	{
		game_time++;

		if (_kbhit()) // input
		{
			c = _getch();
			if (c == -32)
				c = _getch();
			note_index = NoteSet(&player, words, note, c, note_index, &count);
		}

		if (speed <= game_time) // 단어가 떨어질 시간
		{
			game_time = 0;
			player.Score += 1;
			AddWordsX(words);
			if (rand() % percentage == 0) // 단어가 새로 생길지 판단 
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
		Sleep(20); // 20초를 기다림 
		system("cls"); // cmd 클리어 
	}

	// game over
	if (words[0].lenght == 0) // 단어가 없을 때 
	{
		printf("단어를 추가해 주세요!\n");
	}
	else // 단어가 있는데 게임에서 탈락 되었을 때 
	{
		GameOverMessage(player);
	}

	system("pause");
	return 0;
}
