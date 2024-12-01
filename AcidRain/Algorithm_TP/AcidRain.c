/**
* Developer: Algorithm_Team5
* Date: 2024.11.13 ~ 2024.??.??
* Course: �˰���
* Summary:
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

// ����ü ����
typedef struct {
    int x;
    int y;
    char word[20];
    int active;
    int direction; // �밢�� �̵� ���� (-1: ����, 0: ����, 1: ������)
    int isReversed;
} AC_word;

// ���� ���� ����
AC_word AC_Screen_Word[10];
char AC_playerInput[20];
int AC_playerInputLength = 0;
int AC_HP = 5;
int AC_Score = 0;
int AC_ActiveWordCount = 0;

// �Լ� ����
void GotoXY(int x, int y); //�ܾ� ��ġ ������Ʈ �� ���� ���� �ǽð� ǥ�� 
void ClearPosition(int x, int y); // ������ġ ǥ�� �ܾ� ����� ���ο� ��ġ �̵� �ܾ� ǥ�� 
void AC_Init_Word(int i);  // �ܾ� �ʱ�ȭ, ��ġ ���� ������ ���� ���� ���� 
void AC_DownWord(int AC_i); // ���� ���� ����, �ܾ� �ϰ�, �ϴ� ���޽� hp ���� , �밢�� �̵� 
void AC_Compare_Word(void); // ���� üũ, ������ �������� �ܾ� ���� Ʋ���� hp���� 
void AC_DisplayGameOver(); // ���� ���� ȭ�� ���� ���� ��� 

int CheckWordOverlap(int newX); // �ܾ� ��ħ ���� 
char* strrev(char* str); // ���ڿ� ������ 

// Ŀ�� ��ġ �̵� �Լ�
void GotoXY(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// ���ڿ� ������ �Լ� 
char* strrev(char* str) {
    if (!str) return NULL;
    int i = 0, len = strlen(str);
    char temp;
    for (i = 0; i < len / 2; i++) {
        temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
    return str;
}

// ��ġ �ߺ� üũ �Լ�
int CheckWordOverlap(int newX) {
	int j;
    for (j = 0; j < 10; j++) {
        if (AC_Screen_Word[j].active &&
            abs(AC_Screen_Word[j].x - newX) < 10) {
            return 1;
        }
    }
    return 0;
}

// �ܾ� �ʱ�ȭ �Լ�
void AC_Init_Word(int i) {
    if (AC_ActiveWordCount >= 5) return;

    int newX;
    do {
        newX = rand() % 60 + 1;
    } while (CheckWordOverlap(newX));

    AC_Screen_Word[i].x = newX;
    AC_Screen_Word[i].y = 0;

    char* words[10] = { "apple", "banana", "orange", "grape", "kiwi", "melon", "peach", "lemon", "pear", "cherry" };
    strcpy(AC_Screen_Word[i].word, words[rand() % 10]);

    AC_Screen_Word[i].active = 1;
    AC_ActiveWordCount++;

    // �밢�� ���� ���� ���� (����, ������, ����)
    int directions[3] = { -1, 0, 1 };
    AC_Screen_Word[i].direction = directions[rand() % 3];

    // ������ ���� �ʱ�ȭ
    AC_Screen_Word[i].isReversed = rand() % 2;
}

// �ܾ� ���� �Լ�
void AC_DownWord(int AC_i) {
    if (!AC_Screen_Word[AC_i].active) return;

    ClearPosition(AC_Screen_Word[AC_i].x, AC_Screen_Word[AC_i].y); // ���� ��ġ �����

    if (AC_Screen_Word[AC_i].y >= 20) {
        AC_HP--;
        AC_Screen_Word[AC_i].active = 0;
        AC_ActiveWordCount--;

        if (AC_HP <= 0) {
            AC_DisplayGameOver();
            exit(0); // ���� ����
        }
    }
    else {
        AC_Screen_Word[AC_i].y++; // �ܾ� ����
    }

    if (AC_Screen_Word[AC_i].active) {
        char displayWord[20];
        strcpy(displayWord, AC_Screen_Word[AC_i].word);

        // �밢�� �̵� ����
        if (AC_Screen_Word[AC_i].direction != 0) {
            int newX = AC_Screen_Word[AC_i].x + AC_Screen_Word[AC_i].direction * 2;
            if (newX > 0 && newX < 70) {
                AC_Screen_Word[AC_i].x = newX;
            }
            else {
                // ���� ����
                AC_Screen_Word[AC_i].direction *= -1;
            }
        }

        // �ܾ� ������
        if (AC_Screen_Word[AC_i].isReversed) {
            strrev(displayWord);
        }

        GotoXY(AC_Screen_Word[AC_i].x, AC_Screen_Word[AC_i].y);
        printf("%s", displayWord);
    }
}

// �ܾ� �� �� ���� ó�� �Լ�
void AC_Compare_Word(void) {
	int i;
    for (i = 0; i < 10; i++) {
        if (AC_Screen_Word[i].active) {
            char checkWord[20];
            strcpy(checkWord, AC_Screen_Word[i].word);

            if (strcmp(AC_playerInput, checkWord) == 0 ||
                (AC_Screen_Word[i].isReversed &&
                    strcmp(AC_playerInput, strrev(checkWord)) == 0)) {
                AC_Score++;

                // ���� �ܾ� ��ġ �����
                GotoXY(AC_Screen_Word[i].x, AC_Screen_Word[i].y);
                printf("      ");

                AC_Screen_Word[i].active = 0;
                AC_ActiveWordCount--;
                AC_playerInputLength = 0;
                AC_playerInput[0] = '\0';
                return;
            }
        }
    }

    // for ������ ������ ���� ������ ��ġ�ϴ� �ܾ ã�� ���� ��쿡�� HP ����
    AC_HP--;
    if (AC_HP <= 0) {
        AC_DisplayGameOver();
        exit(0); // ���� ����
    }
    AC_playerInputLength = 0;
    AC_playerInput[0] = '\0';
}

// ������ �� �������� ä��� �Լ�
void ClearPosition(int x, int y) {
    GotoXY(x, y);
    printf("      ");
}

// ���� ���� ȭ�� ǥ�� �Լ�
void AC_DisplayGameOver() {
    system("cls");
    printf("\nGame Over!\n");
    printf("���� ����: %d\n", AC_Score);
}

// ���� �Լ�
int main() {
    srand(time(NULL));
    time_t startTime = time(NULL); // ���� �ð� ���
    int timeLimit = 30; // �ð� ������ 30�ʷ� ����
    int wordDropCount = 0;

    system("cls");
    int i;

    for (i = 0; i < 10; i++) {
        AC_Screen_Word[i].active = 0;
    }

    while (1) {
        // ���� �ð��� ����Ͽ� ���� �ð��� ������ �ʰ��ߴ��� Ȯ��
        if (difftime(time(NULL), startTime) >= timeLimit) {
            AC_DisplayGameOver(); // ���� ���� ȭ�� ǥ��
            break; // ���� ����
        }

        if (wordDropCount % 20 == 0) {
            int initializedWords = 0;
            int i;
            for (i = 0; i < 10; i++) {
                if (!AC_Screen_Word[i].active) {
                    AC_Init_Word(i);
                    initializedWords++;
                    if (initializedWords >= 2) {
                        break;
                    }
                }
            }
        }

        int i;
		for (i = 0; i < 10; i++) {
            if (AC_Screen_Word[i].active) {
                AC_DownWord(i);
            }
        }

        // �Է� ó��
        if (_kbhit()) {
            char ch = _getch();
            if (ch == '\r') { // Enter Ű
                AC_playerInput[AC_playerInputLength] = '\0';
                AC_Compare_Word();
            }
            else if (ch == '\b') {
                if (AC_playerInputLength > 0) {
                    AC_playerInputLength--;
                    AC_playerInput[AC_playerInputLength] = '\0';
                }
            }
            else if (AC_playerInputLength < 19) {
                AC_playerInput[AC_playerInputLength++] = ch;
                AC_playerInput[AC_playerInputLength] = '\0';
            }
        }

        GotoXY(0, 22);
        printf("HP: %d | Score: %d | �Է� ��: %s          ", AC_HP, AC_Score, AC_playerInput);

        wordDropCount++;
        Sleep(220); // ���� �ӵ� ����
    }
    return 0;
}
