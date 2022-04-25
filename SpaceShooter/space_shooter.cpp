/*
 * Space Shooter - a simple C++ game for Windows
 * developed by Carlos Hernández Castañeda - 2016
 *
 */
#include <iostream>
#include <stdlib.h> //Standard c and c++ libraries
#include <conio.h>
#include <stdio.h>
#include <windows.h> // To take control over the terminal
#include <list>
using namespace std;
#define UP 72 // arrow keys' ascii numbers
#define LEFT 75
#define RIGHT 77
#define DOWN 80

void gotoxy(int x, int y)
{ // Allows to move inside the terminal using coordinates
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x; // Starts from 0
    dwPos.Y = y; // Starts from 0
    SetConsoleCursorPosition(hCon, dwPos);
}
void HideCursor()
{ // Hides the cursor :3
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 1;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hCon, &cci);
}

void DrawWhiteSpace(int a_x, int a_y, int b_x, int b_y)
{ // To clean a certain space in the terminal
    for (int i = a_x; i < b_x; i++)
    {
        for (int j = a_y; j < b_y; j++)
        {
            gotoxy(i, j);
            printf(" ");
        }
    }
}
void DrawWindowFrame(int a_x, int a_y, int b_x, int b_y)
{ // This will draw a rectangular frame defined by two points
    DrawWhiteSpace(a_x, a_y, b_x, b_y);
    for (int i = a_x; i < b_x; i++)
    {
        gotoxy(i, a_y);
        printf("%c", 205);
        gotoxy(i, b_y);
        printf("%c", 205);
    }
    for (int i = a_y; i < b_y; i++)
    {
        gotoxy(a_x, i);
        printf("%c", 186);
        gotoxy(b_x, i);
        printf("%c", 186);
    }
    gotoxy(a_x, a_y);
    printf("%c", 201);
    gotoxy(b_x, a_y);
    printf("%c", 187);
    gotoxy(a_x, b_y);
    printf("%c", 200);
    gotoxy(b_x, b_y);
    printf("%c", 188);
}
void DrawGameLimits()
{                                  // Draws the game limits, and information that doesn't have to be printed repeatedly
    DrawWindowFrame(1, 2, 79, 23); // The default size of the Windows terminal is 25 rows x 80 columns
    gotoxy(2, 1);
    printf("HP:");
    gotoxy(16, 1);
    printf("ENERGY:");
    gotoxy(50, 1);
    printf("SCORE:");
}
void WelcomeMessage()
{ // The main title, I tried to center it as best as I could
    int x = 13;
    int y = 6;
    gotoxy(x, y);
    printf(" _____                    _____ _           _           ");
    gotoxy(x, y + 1);
    printf("|   __|___ ___ ___ ___   |   __| |_ ___ ___| |_ ___ ___ ");
    gotoxy(x, y + 2);
    printf("|__   | . | .'|  _| -_|  |__   |   | . | . |  _| -_|  _|");
    gotoxy(x, y + 3);
    printf("|_____|  _|__,|___|___|  |_____|_|_|___|___|_| |___|_|  ");
    gotoxy(x, y + 4);
    printf("      |_|");
    gotoxy(x, y + 6);
    printf("                 Press any key to play");
    gotoxy(x, y + 7);
    printf("         developed by Carlos Hernandez C. - 2016");
}
void GameOverDefeatMessage()
{ // When you lose the game you see this in screen
    int a_x = 30;
    int a_y = 11;
    int b_x = a_x + 23;
    int b_y = a_y + 4;
    DrawWindowFrame(a_x, a_y, b_x, b_y);
    gotoxy(a_x + 1, a_y + 2);
    printf("      DEFEAT!!!");
}
void GameOverVictoryMessage()
{ // When you win the game you see this in screen
    int a_x = 30;
    int a_y = 11;
    int b_x = a_x + 23;
    int b_y = a_y + 4;
    DrawWindowFrame(a_x, a_y, b_x, b_y);
    gotoxy(a_x + 1, a_y + 2);
    printf("      VICTORY!!!");
}
void SpecialMessage()
{ // A special message for your special needs
    DrawWhiteSpace(1, 1, 80, 24);
    gotoxy(30, 11);
    printf("Thanks for playing! :3");
}

class SpaceShip
{
private:
    int x;       // x coordinate
    int y;       // y coordinate
    int hp;      // heart points
    int energy;  // energy points
    bool imDead; // is the ship dead?
public:
    int X() { return x; }
    int Y() { return y; }
    int HP() { return hp; }

    bool isDead()
    {
        DrawSpaceShipInfo(); // It's annoying to die and still see a heart on the screen
        return imDead;
    }

    SpaceShip(int _x, int _y)
    {
        x = _x;
        y = _y;
        hp = 3;         // I designed the game to have 3 lifes
        energy = 5;     // And 5 energy points every life
        imDead = false; // By default you are not dead
    }
    void DrawSpaceShipInfo()
    { // Displays HP and energy points, I aligned them with the labels printed in DrawGameLimits
        gotoxy(5, 1);
        printf("     ");
        for (int i = 0; i < hp; i++)
        {
            gotoxy(5 + i, 1);
            printf("%c", 3);
        }
        gotoxy(23, 1);
        printf("     ");
        for (int i = 0; i < energy; i++)
        {
            gotoxy(23 + i, 1);
            printf("%c", 222);
        }
    }
    void Draw()
    { // This is our spaceship
        gotoxy(x, y);
        printf("  %c  ", 30);
        gotoxy(x, y + 1);
        printf("  %c  ", 4);
        gotoxy(x, y + 2);
        printf("%c%c%c%c%c", 17, 30, 223, 30, 16);
    }
    void Erase()
    { // This was or spaceship
        gotoxy(x, y);
        printf("     ");
        gotoxy(x, y + 1);
        printf("     ");
        gotoxy(x, y + 2);
        printf("     ");
    }
    void Damage()
    { // Triggered by the asteroids that hit the spaceship
        energy--;
        if (energy == 0)
        {
            Explosion();
        }
        else
        {
            Erase(); // You can omit this part, is meant to visually tell you that you were hit
            gotoxy(x, y);
            printf("  *  ");
            gotoxy(x, y + 1);
            printf("  *  ");
            gotoxy(x, y + 2);
            printf("*****");
            Sleep(100);
        }
    }
    void Explosion()
    { // When you lose a heart :c
        hp--;
        Erase();
        gotoxy(x, y);
        printf("  *  ");
        gotoxy(x, y + 1);
        printf("  *  ");
        gotoxy(x, y + 2);
        printf("*****");
        Sleep(100);
        Erase();
        gotoxy(x, y);
        printf(" * * ");
        gotoxy(x, y + 1);
        printf("* * *");
        gotoxy(x, y + 2);
        printf(" * * ");
        Sleep(100);
        Erase();
        gotoxy(x, y);
        printf("*   *");
        gotoxy(x, y + 1);
        printf(" * * ");
        gotoxy(x, y + 2);
        printf("* * *");
        Sleep(100);
        if (hp > 0)
        { // If you still have a heart or more
            energy = 5;
        }
        else
        { // If you don't
            imDead = true;
        }
    }
    void Move()
    { // The main function of the spaceship
        if (_kbhit())
        {                        // If you move the spaceship
            Erase();             // Look I'm invisible
            char key = _getch(); // What did you type?
            switch (key)
            { // Checks if the spaceship won't leave the game boundaries
            case LEFT:
                if (x > 2)
                    x -= 1;
                break;
            case RIGHT:
                if (x + 4 < 77)
                    x += 1;
                break;
            case UP:
                if (y > 3)
                    y -= 1;
                break;
            case DOWN:
                if (y + 2 < 22)
                    y += 1;
                break;
            }
        }
        Draw(); // The spaceship is drawn regardless if you moved it or not, if you did then it will appear in it's new position.
    }
};
class Asteroid
{
private:
    int x;
    int y;

public:
    int X() { return x; }
    int Y() { return y; }

    Asteroid(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
    void Draw()
    {
        gotoxy(x, y);
        printf("*"); // Fear the asteroids!!
    }
    void Collision(SpaceShip &ss) // The asteroid finds the spaceship
    {
        if (((x >= ss.X()) && (x <= ss.X() + 5)) && ((y >= ss.Y()) && (y <= ss.Y() + 2)))
        {                // Depending on the shape of the spaceship you have to tinker when the asteroid really hits you
            ss.Damage(); // The asteroid hurts
            gotoxy(x, y);
            printf(" ");         // And the asteroid is "destroyed"
            x = rand() % 74 + 3; // The truth is it just teleports to the top of the map
            y = 4;
        }
        else
        {
            gotoxy(x, y);
            printf(" ");
            y++;
            if (y > 22)
            {                        // If the asteroid goes too down in the map
                x = rand() % 74 + 3; // It will be teleported to the top
                y = 4;
            }
            Draw();
        }
    }
};
class Bullet
{
private:
    int x;
    int y;

public:
    int X() { return x; }
    int Y() { return y; }
    Bullet(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
    bool isOut()
    {
        if (y <= 3)
        { // If the bullet reaches the top of the map
            gotoxy(x, y);
            printf(" "); // It disappears
            return true; // And informs the ame that it should no longer exist :c
        }
        else
        {
            return false;
        }
    }
    void Move()
    {
        gotoxy(x, y);
        printf(" ");
        y--;
        gotoxy(x, y);
        printf("."); // The shape of the bullet
    }
};
int main()
{
    HideCursor();
    WelcomeMessage();
    _getch();
    DrawGameLimits();

    list<Bullet *> Bullets;          // We will use a dynamic list for the bullets in the game
    list<Bullet *>::iterator bullet; // And an iterator for the list

    list<Asteroid *> Asteroids; // The same goes for the asteroids
    list<Asteroid *>::iterator asteroid;

    for (int i = 0; i < 10; i++) // Pick as many asteroids as you want
    {                            // They are randomly placed in the map but not too low
        Asteroids.push_back(new Asteroid(rand() % 78 + 1, rand() % 4 + 3));
    }

    SpaceShip ss = SpaceShip(40, 20); // Here our adventure begins
    int score = 0;                    // Your score :3

    while (!ss.isDead() && score != 100) // If you die or reach 100 points the game ends
    {
        if (_kbhit())
        {
            char key = _getch();
            if (key == ' ')
            { // If you press the space bar you add a bullet to the bullet list
                Bullets.push_back(new Bullet(ss.X() + 2, ss.Y() - 1));
            }
        }
        for (bullet = Bullets.begin(); bullet != Bullets.end(); bullet++)
        { // For every bullet that is in space
            (*bullet)->Move();
            if ((*bullet)->isOut())
            {                     // If the bullet reached the end of the map
                delete (*bullet); // It gets deleted
                bullet = Bullets.erase(bullet);
            }
        }
        for (asteroid = Asteroids.begin(); asteroid != Asteroids.end(); asteroid++)
        { // Every asteroid checks if the spaceship shares it's coordinates :3
            (*asteroid)->Collision(ss);
        }
        for (asteroid = Asteroids.begin(); asteroid != Asteroids.end(); asteroid++)
        {
            for (bullet = Bullets.begin(); bullet != Bullets.end(); bullet++)
            {                                // asteroid-bullet collision
                int astX = (*asteroid)->X(); // Coordinates of the asteroid
                int astY = (*asteroid)->Y();
                int bulX = (*bullet)->X(); // Coordinates of the bullet
                int bulY = (*bullet)->Y();
                if ((astX == bulX) && ((astY == bulY) || (astY + 1 == bulY)))
                { // Chances are that in the Y axis they never reach the same value, that case must be considered
                    gotoxy(bulX, bulY);
                    printf(" "); // Makes the bullet invisible
                    gotoxy(astX, astY);
                    printf("X");
                    printf(" ");      // I still have my doubts in this part, but it tries to signal a collision, sometimes the X remains theme...
                    delete (*bullet); // You delete the bullet
                    bullet = Bullets.erase(bullet);
                    delete (*asteroid); // And the asteroid
                    asteroid = Asteroids.erase(asteroid);
                    Asteroids.push_back(new Asteroid(rand() % 78 + 1, rand() % 4 + 3)); // in order to not reduce the number of asteroids I add one everytime one is destroyed
                    score += 10;                                                        // And you get 10 points for a job well done :3
                }
            }
        }
        ss.Move();
        gotoxy(56, 1);
        printf("%d", score);
        Sleep(30); // This is essential, otherwise the game would be unplayable
    }
    if (!ss.isDead())
    { // If you died
        GameOverVictoryMessage();
    }
    else
    { // If you won
        GameOverDefeatMessage();
    }
    Sleep(5000);
    SpecialMessage(); // Surprise
    Sleep(5000);
    return 0;
}