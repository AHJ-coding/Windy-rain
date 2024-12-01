/**
* Developer: Algorithm_Team5
* Date: 2024.11.13 ~ 2024.??.??
* Course: 알고리즘
* Summary:
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

// 구조체 정의
typedef struct {
    int x;
    int y;
    char word[20];
    int active;
    int direction; // 대각선 이동 방향 (-1: 왼쪽, 0: 수직, 1: 오른쪽)
    int isReversed;
} AC_word;

// 전역 변수 설정
AC_word AC_Screen_Word[10];
char AC_playerInput[20];
int AC_playerInputLength = 0;
int AC_HP = 5;
int AC_Score = 0;
int AC_ActiveWordCount = 0;

// 함수 선언
void GotoXY(int x, int y); //단어 위치 업데이트 및 게임 정보 실시간 표시 
void ClearPosition(int x, int y); // 이전위치 표시 단어 지우고 새로운 위치 이동 단어 표시 
void AC_Init_Word(int i);  // 단어 초기화, 위치 방향 뒤집기 상태 랜덤 설정 
void AC_DownWord(int AC_i); // 낙하 동작 구현, 단어 하강, 하단 도달시 hp 감소 , 대각선 이동 
void AC_Compare_Word(void); // 정답 체크, 맞으면 점수증가 단어 제거 틀리면 hp감소 
void AC_DisplayGameOver(); // 게임 종료 화면 최종 점수 출력 

int CheckWordOverlap(int newX); // 단어 겹침 방지 
char* strrev(char* str); // 문자열 뒤집기 

// 커서 위치 이동 함수
void GotoXY(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// 문자열 뒤집기 함수 
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

// 위치 중복 체크 함수
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

// 단어 초기화 함수
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

    // 대각선 방향 랜덤 설정 (왼쪽, 오른쪽, 수직)
    int directions[3] = { -1, 0, 1 };
    AC_Screen_Word[i].direction = directions[rand() % 3];

    // 뒤집기 상태 초기화
    AC_Screen_Word[i].isReversed = rand() % 2;
}

// 단어 낙하 함수
void AC_DownWord(int AC_i) {
    if (!AC_Screen_Word[AC_i].active) return;

    ClearPosition(AC_Screen_Word[AC_i].x, AC_Screen_Word[AC_i].y); // 이전 위치 지우기

    if (AC_Screen_Word[AC_i].y >= 20) {
        AC_HP--;
        AC_Screen_Word[AC_i].active = 0;
        AC_ActiveWordCount--;

        if (AC_HP <= 0) {
            AC_DisplayGameOver();
            exit(0); // 게임 종료
        }
    }
    else {
        AC_Screen_Word[AC_i].y++; // 단어 낙하
    }

    if (AC_Screen_Word[AC_i].active) {
        char displayWord[20];
        strcpy(displayWord, AC_Screen_Word[AC_i].word);

        // 대각선 이동 로직
        if (AC_Screen_Word[AC_i].direction != 0) {
            int newX = AC_Screen_Word[AC_i].x + AC_Screen_Word[AC_i].direction * 2;
            if (newX > 0 && newX < 70) {
                AC_Screen_Word[AC_i].x = newX;
            }
            else {
                // 방향 반전
                AC_Screen_Word[AC_i].direction *= -1;
            }
        }

        // 단어 뒤집기
        if (AC_Screen_Word[AC_i].isReversed) {
            strrev(displayWord);
        }

        GotoXY(AC_Screen_Word[AC_i].x, AC_Screen_Word[AC_i].y);
        printf("%s", displayWord);
    }
}

// 단어 비교 및 점수 처리 함수
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

                // 맞춘 단어 위치 지우기
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

    // for 루프를 완전히 돌고 나서도 일치하는 단어를 찾지 못한 경우에만 HP 감소
    AC_HP--;
    if (AC_HP <= 0) {
        AC_DisplayGameOver();
        exit(0); // 게임 종료
    }
    AC_playerInputLength = 0;
    AC_playerInput[0] = '\0';
}

// 정답일 때 공백으로 채우는 함수
void ClearPosition(int x, int y) {
    GotoXY(x, y);
    printf("      ");
}

// 게임 오버 화면 표시 함수
void AC_DisplayGameOver() {
    system("cls");
    printf("\nGame Over!\n");
    printf("최종 점수: %d\n", AC_Score);
}

// 메인 함수
int main() {
    srand(time(NULL));
    time_t startTime = time(NULL); // 시작 시간 기록
    int timeLimit = 30; // 시간 제한을 30초로 설정
    int wordDropCount = 0;

    system("cls");
    int i;

    for (i = 0; i < 10; i++) {
        AC_Screen_Word[i].active = 0;
    }

    while (1) {
        // 현재 시간을 계산하여 게임 시간이 제한을 초과했는지 확인
        if (difftime(time(NULL), startTime) >= timeLimit) {
            AC_DisplayGameOver(); // 게임 오버 화면 표시
            break; // 게임 종료
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

        // 입력 처리
        if (_kbhit()) {
            char ch = _getch();
            if (ch == '\r') { // Enter 키
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
        printf("HP: %d | Score: %d | 입력 중: %s          ", AC_HP, AC_Score, AC_playerInput);

        wordDropCount++;
        Sleep(220); // 게임 속도 조절
    }
    return 0;
}
