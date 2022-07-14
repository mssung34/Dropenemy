#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "key.h"

void GameStart();
int StartStage(int stage);
void MoveCur(int y, int x);
int CreateMap(int mapLevel, int mode);
int PlayerMove();
int CreateEnemy(int curEnemy);
int CreateItem();
void StageClear(int clearStage);
void GameOver();

int main(void)
{
    int sltAuto = 0;
    int playerState = 0;
    int level = 1;

    GameStart();

    while (1)
    {
        sltAuto = StartStage(level);
        playerState = CreateMap(level, sltAuto);
        if (playerState == 1)
        {
            StageClear(level);
            level++;
        }
        else if (playerState == 2)
            break;
    }

    GameOver();

    return 0;
} // end main

void GameStart()
{
    system("clear");
    printf("\n\n\n");
    printf("\t\t  Legend of Boki: Ranning Enemy\n\n\n");
    printf("\t복이를 공격하기 위해 떨어지는 적들을 좌우로 피하자!\n\n");
    printf("\t       왼쪽 이동: [ ← ] 오른쪽 이동: [ → ]\n\n");
    printf("\t\t");
    usleep(3000000);
} // end GameStart

int StartStage(int stage)
{
    int _auto = 0;
    while (1)
    {
        system("clear");
        printf("\n\n\n");
        printf("\t\tSTAGE : %d\n\n", stage);
        printf("\t1 ) 수동진행\t2 ) 자동진행\n");
        scanf("%d", &_auto);
        if (_auto == 1 || _auto == 2)
            return _auto;
        else
            continue;
    }
} // end StartStage

void MoveCur(int y, int x)
{
    printf("\x1b[%dA\033[%dG", y, x);
} // end MoveCur

int CreateMap(int mapLevel, int mode)
{
    srand((unsigned)time(NULL));

    int mapY = 20, mapX = 42;
    int map[mapY][mapX]; // 맵 관련

    int enemyX = 0, curEnemyX = 0, curEnemyX_2 = 0; // 적 관련 변수
    int itemX = 0, curItemX = 0, curItemX_2 = 0; // 아이템 관련 변수
    int spawnPer = 0, curSpwanPer = 0, curSpwanPer_2 = 0;
    int enemyXY[mapY][mapX]; // 적 관련
    int itemXY[mapY][mapX];  // 아이템 관련

    int checkX = 0;

    int score = 0, maxScore = 100 * mapLevel;

    int playerLife[3] = {1, 1, 1};
    int playerLifeX = 3;

    static int playerX = 20;

    while (1)
    {
        system("clear");

        // ---------------------------------------------------------- 기본 맵 만들기
        for (int i = 0; i < mapY; i++)
        {
            for (int j = 0; j < mapX; j++)
            {
                if (i == 0 || i == 18 || i == 19)
                    map[i][j] = 1;
                else
                {
                    if (j == 0 || j == 41)
                        map[i][j] = 1;
                    else
                        map[i][j] = 0;
                }
            }
        } // 기본 맵 만들기 끝

        // ---------------------------------------------------------- 적 재배치
        for (int i = 0; i < mapY; i++)
        {
            for (int j = 0; j < mapX; j++)
            {
                if (enemyXY[i][j] == 1)
                {
                    if (i + 1 < 18)
                        map[i + 1][j] = 3;
                }
                if (map[i][0] == 3 || map[i][41] == 3)
                    map[i][0] = 1, map[i][41] = 1;
            }
        }
        // 적 재배치 끝

        // ---------------------------------------------------------- 아이템 재배치
        for (int i = 0; i < mapY; i++)
        {
            for (int j = 0; j < mapX; j++)
            {
                if (itemXY[i][j] == 1)
                {
                    if (i + 1 < 18)
                        map[i + 1][j] = 4;
                }
                if (map[i][0] == 4 || map[i][41] == 4)
                    map[i][0] = 1, map[i][41] = 1;
            }
        }
        // 아이템 재배치 끝

        // ---------------------------------------------------------- 플레이어 피격
        if (map[17][playerX] == 3)
        {
            playerLifeX--;
            if (playerLifeX < 1)
                return 2;
            playerLife[playerLifeX] = 0;
            map[17][playerX] = 2;
        }
        else if (map[17][playerX] == 4)
        {
            playerLife[playerLifeX] = 1;
            playerLifeX++;
            if (playerLifeX > 3)
                playerLifeX = 3;
            map[17][playerX] = 2;
        }
        // ---------------------------------------------------------- 플레이어 위치
        map[17][playerX] = 2;
        if (map[17][0] == 2 || map[17][41] == 2)
        {
            if (map[17][0] == 2)
            {
                playerX++;
                continue;
            }
            if (map[17][41] == 2)
            {
                playerX--;
                continue;
            }
        }
        // 플레이어 위치 끝

        // ---------------------------------------------------------- 드랍 랜덤생성
        while (1)
        {
            curSpwanPer = rand() % 10 + 1;
            spawnPer = curSpwanPer;
            if (spawnPer == curSpwanPer_2)
                continue;
            else
            {
                if (spawnPer <= mapLevel)
                {
                    for (int k = CreateEnemy(mapLevel); k > 0; k--)
                    {
                        curEnemyX = rand() % 40 + 1;
                        enemyX = curEnemyX;
                        if (curEnemyX == curEnemyX_2)
                            continue;
                        map[1][enemyX] = 3;
                        curEnemyX_2 = curEnemyX;
                    }
                    while(1)
                    {
                        if (CreateItem() == 1)
                        {
                            // ...?
                            curItemX = rand() % 40 + 1;
                            itemX = curItemX;
                            if (curItemX == curItemX_2 || curItemX == curEnemyX_2)
                                continue;
                            map[1][itemX] = 4;
                            curItemX_2 = curItemX;
                        }
                        break;
                    }

                }
                curSpwanPer_2 = curSpwanPer;
                break;
            }
        }
        // 드랍 랜덤생성 끝

        // ---------------------------------------------------------- 맵 출력 시작
        printf("\n\tLEVEL : %2d\tSCORE : %3d / %3d\tLIFE : ", mapLevel, score, maxScore);
        for (int a = 0; a < 3; a++)
            printf("\033[0;31m%s  \033[0;37m", playerLife[a] == 1 ? "♥" : "♡");

        printf("\n\n");
        for (int i = 0; i < mapY; i++)
        {
            for (int j = 0; j < mapX; j++)
            {
                switch (map[i][j])
                {
                case 0: // 공백
                    printf("  ");
                    continue;
                case 1: // 벽
                    printf("■ ");
                    continue;
                case 2: // 플레이어
                    printf("\033[0;34m■ \033[0;37m");
                    continue;
                case 3: // 적
                    printf("\033[0;31m● \033[0;37m");
                    continue;
                case 4: // 아이템
                    printf("\033[0;34m♥ \033[0;37m");
                    continue;

                default:
                    continue;
                }
            }
            printf("\n");
        } // 맵 출력 끝
        // ---------------------------------------------------------- 적 정보 저장
        for (int i = 0; i < mapY; i++)
        {
            for (int j = 0; j < mapX; j++)
            {
                if (map[i][j] == 3)
                    enemyXY[i][j] = 1;
                else
                    enemyXY[i][j] = 0;
            }
        }
        //적 정보저장 끝
        int itemX = rand() % 40 + 1;
        // ---------------------------------------------------------- 아이템 정보 저장
        for (int i = 0; i < mapY; i++)
        {
            for (int j = 0; j < mapX; j++)
            {
                if (map[i][j] == 4)
                    itemXY[i][j] = 1;
                else
                    itemXY[i][j] = 0;
            }
        }
        //아이템 정보저장 끝

        // ---------------------------------------------------------- 플레이어 좌표 조절
        if (mode == 1)
        {
            checkX += PlayerMove();
            if (checkX != 0)
            {
                playerX += checkX;
                checkX = 0;
            }
        }
        else if (mode == 2)
        {
            checkX = rand() % 10;
            playerX += checkX % 2 == 0 ? 1 : -1;
            usleep(60000);
        }
        // 플레이어 좌표 조절 끝
        // ---------------------------------------------------------- 점수 관련
        score++;
        if (score == maxScore)
            return 1;
    } // end while
    return 0;
} // end CreateMap

void StageClear(int clearStage)
{
    for (int i = 0; i < 5; i++)
    {
        system("clear");
        printf("\n\n\n");
        printf("\t\t%d STAGE CLEAR ! ! !\n", clearStage);
        usleep(500000);
        system("clear");
        usleep(500000);
    }
} // end StageClear

void GameOver()
{
    system("clear");
    printf("\n\n\n");
    printf("\t\t- G A M E O V E R -\n\n");
} // end GameOver

int PlayerMove()
{
    int getKey = key();

    if (getKey == KEY_RIGHT)
        return 1;
    else if (getKey == KEY_LEFT)
        return -1;
    else
        return 0;
} // end PlayerMove

int CreateEnemy(int curEnemy)
{
    srand((unsigned)time(NULL));
    while (1)
    {
        int enemy = (rand() % 4 + 1) * curEnemy;
        if (enemy > 10)
            continue;
        else
            return enemy;
    }
} // end CreateEnemy

int CreateItem()
{
    srand((unsigned)time(NULL));
    int item = (rand() % 3 + 1);
    return item;
} // end CreateItem

