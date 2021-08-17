#include <iostream>
#include <random>
#include <stdlib.h>
#include <chrono>

using namespace std;
 
enum Cell {
    ZERO = '0',
    CROSS = 'X',
    EMPTY = '_'
};

enum Progress {
    IN_PROGRESS,
    WON_AI,
    WON_HUMAN,
    DRAW
};

struct Coordinate {
    size_t y{ 0 };
    size_t x{ 0 };
};

#pragma pack (push, 1)
struct MyFirstGame {
    Cell** ppField = nullptr; 
    const size_t SIZE = 3;
    size_t turn{ 0 };
    Cell ai;
    Cell human;
    Progress progress = { IN_PROGRESS };
};
#pragma pack (pop)

int32_t __fastcall getRandomNum(int32_t min, int32_t max)
{
    const static auto seed = chrono::system_clock::now().time_since_epoch().count();
    static mt19937_64 generator(seed);
    uniform_int_distribution<int32_t> dis(min, max);
    return dis(generator);
}

void __fastcall initGame(MyFirstGame& game)
{
    game.ppField = new Cell * [game.SIZE];

    for (size_t i = 0; i < game.SIZE; i++)
    {
        game.ppField[i] = new Cell[game.SIZE];
    }

    for (size_t y = 0; y < game.SIZE; y++)
    {
        for (size_t x = 0; x < game.SIZE; x++)
        {
            game.ppField[y][x] = EMPTY;
        }
    }
    if (getRandomNum(0, 1000) > 500)
    {
        game.human = CROSS;
        game.ai = ZERO;
        game.turn = 0;
    }
    else
    {
        game.human = ZERO;
        game.ai = CROSS;
        game.turn = 1;
    }
}

void __fastcall deinitGame(MyFirstGame& game)
{
    for (size_t i = 0; i < game.SIZE; i++)
    {
        delete [] game.ppField[i];
    }
    delete[] game.ppField;
}

void __fastcall drawGame(const MyFirstGame& game)
{
    cout << "     ";
    for (size_t x = 0; x < game.SIZE; x++)
        cout << x + 1 << "   ";
    cout << endl;

    for (size_t y = 0; y < game.SIZE; y++)
    {
        cout << " " << y + 1 << " | ";
        for (size_t x = 0; x < game.SIZE; x++)
        {
            cout << (char)game.ppField[y][x] << " | ";
        }
        cout << endl;
    }

    cout << endl << "Human: " << (char)game.human << endl;
    cout << "Computer: " << (char)game.ai << endl << endl;
}

inline void clearScreen()
{
    system("cls");
}



Progress __fastcall getWon(MyFirstGame& game)
{
    for (size_t y = 0; y < game.SIZE; y++)
    {
        if (game.ppField[y][0] == game.ppField[y][1] && game.ppField[y][0] == game.ppField[y][2])
        {
            if (game.ppField[y][0] == game.human)
                return WON_HUMAN;

            if (game.ppField[y][0] == game.ai)
                return WON_AI;
        }
    }
    for (size_t x = 0; x < game.SIZE; x++)
    {
        if (game.ppField[0][x] == game.ppField[1][x] && game.ppField[0][x] == game.ppField[2][x])
        {
            if (game.ppField[0][x] == game.human)
                return WON_HUMAN;

            if (game.ppField[0][x] == game.ai)
                return WON_AI;
        }
    }

    if (game.ppField[0][0] == game.ppField[1][1] && game.ppField[0][0] == game.ppField[2][2])
    {
        if (game.ppField[0][0] == game.human)
            return WON_HUMAN;

        if (game.ppField[0][0] == game.ai)
            return WON_AI;
    }

    if (game.ppField[1][1] == game.ppField[2][0] && game.ppField[1][1] == game.ppField[0][2])
    {
        if (game.ppField[1][1] == game.human)
            return WON_HUMAN;

        if (game.ppField[1][1] == game.ai)
            return WON_AI;
    }

    bool draw = true;
    for (size_t y = 0; y < game.SIZE; y++)
    {
        for (size_t x = 0; x < game.SIZE; x++)
        {
            if (game.ppField[y][x] == EMPTY)
            {
                draw = false;
                break;
            }
        }

        if (!draw)
            break;
    }
    
    if (draw)
        return DRAW;

    return IN_PROGRESS;
}

Coordinate __fastcall getHumanCoordinate(MyFirstGame& game)
{
    Coordinate c{ 0 };

    do {
        cout << "Enter X: ";
        cin >> c.x;
        cout << "Enter Y: ";
        cin >> c.y;
    } while (c.x ==0 || c.y == 0 || c.x > 3 || c.y > 3 || game.ppField[c.y-1][c.x-1] != EMPTY);
    c.x--;
    c.y--;

    return c;
}

Coordinate __fastcall getAICoordinate(MyFirstGame& game)
{
    for (size_t y = 0; y < game.SIZE; y++)
    {
        for (size_t x = 0; x < game.SIZE; x++)
        {
            if (game.ppField[y][x] == EMPTY)
            {
                game.ppField[y][x] = game.ai;
                if (getWon(game) == WON_AI)
                {
                    game.ppField[y][x] = EMPTY;
                    return { y, x };
                }
                game.ppField[y][x] = EMPTY;
            }
        }
    }

    for (size_t y = 0; y < game.SIZE; y++)
    {
        for (size_t x = 0; x < game.SIZE; x++)
        {
            if (game.ppField[y][x] == EMPTY)
            {
                game.ppField[y][x] = game.human;
                if (getWon(game) == WON_HUMAN)
                {
                    game.ppField[y][x] = EMPTY;
                    return { y, x };
                }
                game.ppField[y][x] = EMPTY;
            }
        }
    }

    if (game.ppField[1][1] == EMPTY)
        return { 1, 1 };
    Coordinate array[4];
    size_t number{ 0 };

    if (game.ppField[0][0] == EMPTY)
    {
        array[number] = { 0, 0 };
        number++;
    }

    if (game.ppField[2][2] == EMPTY)
    {
        array[number] = { 2, 2 };
        number++;
    }

    if (game.ppField[2][0] == EMPTY)
    {
        array[number] = { 2, 0 };
        number++;
    }

    if (game.ppField[0][2] == EMPTY)
    {
        array[number] = { 0, 2 };
        number++;
    }

    if (number > 0)
    {
        const size_t randIndex = getRandomNum(0, 1000) % number;
        return array[randIndex];
    }

    number = 0;
    if (game.ppField[0][1] == EMPTY)
    {
        array[number] = { 0, 1 };
        number++;
    }

    if (game.ppField[2][1] == EMPTY)
    {
        array[number] = { 2, 1 };
        number++;
    }

    if (game.ppField[1][0] == EMPTY)
    {
        array[number] = { 1, 0 };
        number++;
    }

    if (game.ppField[1][2] == EMPTY)
    {
        array[number] = { 1, 2 };
        number++;
    }

    if (number > 0)
    {
        const size_t randIndex = getRandomNum(0, 1000) % number;
        return array[randIndex];
    }
}

void __fastcall Congrats(const MyFirstGame& game)
{
    if (game.progress == WON_HUMAN)
        cout << "Human won! " << endl;

    else if (game.progress == WON_AI)
        cout << "Computer won! " << endl;

    else if (game.progress == DRAW)
        cout << "Draw! " << endl;
}

int main()
{
    MyFirstGame game;

    initGame(game);

    clearScreen();

    drawGame(game);

    do {
        if (game.turn % 2 == 0)
        {
            Coordinate c = getHumanCoordinate(game);
            game.ppField[c.y][c.x] = game.human;
        }
        else
        {
            Coordinate c = getAICoordinate(game);
            game.ppField[c.y][c.x] = game.ai;
        }

        clearScreen();
        drawGame(game);
        game.turn++;

        game.progress = getWon(game);

    } while (game.progress == IN_PROGRESS);

    Congrats(game);

    deinitGame(game);
}