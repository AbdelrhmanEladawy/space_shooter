#include <iostream>
#include <stdlib.h>
#include <stdio.h>     //Standard c and c++ libraries
#include <windows.h>   // To take control over the terminal ... and use sleep function
#include <conio.h>	   // to use getch() and kbhit() funcions
#include <list>
using namespace std;

#define UP 72 // arrow keys' ascii numbers
#define LEFT 75
#define RIGHT 77
#define DOWN 80

// The default size of the Windows terminal is 25 rows x 80 columns
#define SCREEN_WIDTH  80
#define SCREEN_HEIGHT 25

char SpaceShip_shape[3][5] = { ' ' , ' ' , (char)30  , ' ' , ' ' ,
						 ' ' , ' ' , (char)4   , ' ' , ' ' ,
						 (char)17  , (char)30  , (char)223 , (char)30  , (char)16	};

char enemyShip_shape[3][5] = { (char)0xfff1 , (char)0xfff1 , (char)0xfff1 , (char)0xfff1 , (char)0xfff1 ,
						       '|', (char)0xfff1, (char)0xfff1, (char)0xfff1, '|',
						       ' ', ' ', (char)0xfff1, ' ', ' ',};

char damage_shape[3][5] = { ' ' , ' ' , '*' , ' ' , ' ' ,
						    ' ' , ' ' , '*' , ' ' , ' ' ,
					  	    '*' , '*' , '*' , '*' , '*'};

// Allows to move inside the terminal using coordinates
//using windows.h library
void gotoxy(int x, int y)
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x; // Starts from 0
    dwPos.Y = y; // Starts from 0
    SetConsoleCursorPosition(hCon, dwPos);
}

// Hides the cursor in terminal
void HideCursor()
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 1;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hCon, &cci);
}

// To clean a certain space in the terminal using two points ( a(x,y) ,b(x,y)
void DrawWhiteSpace(int a_x, int a_y, int b_x, int b_y)
{
    for (int i = a_x; i < b_x; i++)
    {
        for (int j = a_y; j < b_y; j++)
        {
            gotoxy(i, j);
            printf(" ");
        }
    }
}

// This will draw a rectangular frame defined by two points ( a(x,y) ,b(x,y) )
void DrawWindowFrame(int a_x, int a_y, int b_x, int b_y)
{
    DrawWhiteSpace(a_x, a_y, b_x, b_y);
    for (int i = a_x; i < b_x; i++) //upper and lower boundary
    {
        gotoxy(i, a_y);
        printf("%c", 205);
        gotoxy(i, b_y);
        printf("%c", 205);
    }
    for (int i = a_y; i < b_y; i++) //right and left boundary
    {
        gotoxy(a_x, i);
        printf("%c", 186);
        gotoxy(b_x, i);
        printf("%c", 186);
    }
    gotoxy(a_x, a_y);		//corners
    printf("%c", 201);
    gotoxy(b_x, a_y);
    printf("%c", 187);
    gotoxy(a_x, b_y);
    printf("%c", 200);
    gotoxy(b_x, b_y);
    printf("%c", 188);
}

// Draws the game limits, and information that doesn't have to be printed repeatedly
void DrawGameLimits()
{
    DrawWindowFrame(1, 2, SCREEN_WIDTH -1 , SCREEN_HEIGHT -2 ); // The default size of the Windows terminal is 25 rows x 80 columns
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
    gotoxy(x, y + 10);
    printf("team members : ");
    gotoxy(x, y + 11);
    printf("		Abdelrhman Ahmed");
	gotoxy(x, y + 12);
	printf("		Abdelrhman");
	gotoxy(x, y + 13);
	printf("		Hassan");
}

// When you lose the game you see this in screen
void GameOverDefeatMessage()
{
    int a_x = 30;
    int a_y = 11;
    int b_x = a_x + 23;
    int b_y = a_y + 4;
    DrawWindowFrame(a_x, a_y, b_x, b_y);
    gotoxy(a_x + 1, a_y + 2);
    printf("      DEFEAT!!!");

	gotoxy(1,24);
}

// When you win the game you see this in screen
void GameOverVictoryMessage()
{
    int a_x = 30;
    int a_y = 11;
    int b_x = a_x + 23;
    int b_y = a_y + 4;
    DrawWindowFrame(a_x, a_y, b_x, b_y);
    gotoxy(a_x + 1, a_y + 2);
    printf("      VICTORY!!!");

	gotoxy(1,24);
}

// A special message for your special needs
void SpecialMessage()
{
    DrawWhiteSpace(1, 1, SCREEN_WIDTH , SCREEN_HEIGHT-1 );
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

    SpaceShip(int _x = SCREEN_WIDTH/2 , int _y = SCREEN_HEIGHT-5 )
    {
        x = _x;
        y = _y;
        hp = 3;         // I designed the game to have 3 lifes
        energy = 5;     // And 5 energy points every life
        imDead = false; // By default you are not dead
    }

	bool isDead()
    {
        DrawSpaceShipInfo(); // It's annoying to die and still see a heart on the screen
        return imDead;
    }

    // Displays HP and energy points, I aligned them with the labels printed in DrawGameLimits
    void DrawSpaceShipInfo()
    {
        gotoxy(5, 1);
        printf("     ");
        for (int i = 0; i < hp; i++) // draw HP
        {
            gotoxy(5 + i, 1);
            printf("%c", 3);
        }
        
        gotoxy(23, 1);
        printf("     ");
        for (int i = 0; i < energy; i++) // draw energy
        {
            gotoxy(23 + i, 1);
            printf("%c", 222);
        }
    }

    // This is our spaceship body
    void Draw()
    {
        gotoxy(x, y);
        printf("  %c  ", 30);
        gotoxy(x, y + 1);
        printf("  %c  ", 4);
        gotoxy(x, y + 2);
        printf("%c%c%c%c%c", 17, 30, 223, 30, 16);
    }

    // Erase the old spaceship from screen
    void Erase()
    {
        gotoxy(x, y);
        printf("     ");
        gotoxy(x, y + 1);
        printf("     ");
        gotoxy(x, y + 2);
        printf("     ");
    }

    // Triggered by the asteroids that hit the spaceship
    void Damage()
    {
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

    // When you lose a heart :c
    void Explosion()
    {
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

    // The main function of the spaceship
    void Move()
    {
        if (_kbhit())
        {                        // If you move the spaceship
           Erase();				 //to delete the old spaceship
            char key = _getch(); // What did you type?
            switch (key)
            { // Checks if the spaceship won't leave the game boundaries
            case LEFT:
			case 'a':
			case 'A':
                if (x > 2)
                    x -= 1;
                break;
            case RIGHT:
			case 'd':
			case 'D':
                if (x + 4 < SCREEN_WIDTH-3 )
                    x += 1;
                break;
            case UP:
			case 'w':
			case 'W':
                if (y > 3)
                    y -= 1;
                break;
            case DOWN:
			case 's':
			case 'S':
                if (y + 2 < SCREEN_HEIGHT-3 )
                    y += 1;
                break;
            }
        }
        
        Draw(); // The spaceship is drawn regardless if you moved it or not, if you did then it will appear in it's new position.
    }
};

class enemyShip
{
private:
    int x;       // x coordinate
    int y;       // y coordinate
    int hp;      // heart points
    bool is_Destroyed; // is the ship dead?

public:
	int X() { return x; }
    int Y() { return y; }
    int HP() { return hp; }

	enemyShip(int _x , int _y , int _hp = 1 )
	{
		x = _x;
        y = _y;
        hp = _hp;         // I designed the game to have 3 lifes
        is_Destroyed = false; // By default you are not dead
	};

	bool isDestroyed()
	{
		return is_Destroyed;
	}

	void Draw()
	{
		gotoxy(x, y);
        printf("±±±±±");
        gotoxy(x, y + 1);
        printf("|±±±|");
        gotoxy(x, y + 2);
        printf("  ±  ");
	}

	 // Erase the old enemyShip from screen
    void Erase()
    {
        gotoxy(x, y);
        printf("     ");
        gotoxy(x, y + 1);
        printf("     ");
        gotoxy(x, y + 2);
        printf("     ");
    }

	    // Triggered by the asteroids that hit the spaceship
    void Damage()
    {
        hp--;
        if (hp == 0)
        {
			is_Destroyed = true;
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

    // When you lose a heart :c
    void Explosion()
    {
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
        { // If the Ship_bullet reaches the top of the map
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
        printf("."); // The shape of the Ship_bullet
    }
};

int main()
{
	system("cls");
	HideCursor();
	DrawGameLimits();

	list<Bullet *> Ship_Bullets;          // We will use a dynamic list for the Bullets in the game
    list<Bullet *>::iterator Ship_bullet; // And an iterator for the list

	SpaceShip ss(40,20);

	//there will be list of enemy
	enemyShip es(40,3);
	es.Draw();

	ss.DrawSpaceShipInfo();
	int score = 0;

	while ( true )
	{
		if (_kbhit())
        {
            char key = _getch();
            if (key == ' ')
            { // If you press the space bar you add a Ship_bullet to the Ship_bullet list
                Ship_Bullets.push_back(new Bullet(ss.X() + 2, ss.Y() - 1));
            }
        }

		for (Ship_bullet = Ship_Bullets.begin(); Ship_bullet != Ship_Bullets.end(); Ship_bullet++)
        { // For every Ship_bullet that is in space
            (*Ship_bullet)->Move();
            if ((*Ship_bullet)->isOut())
            {                     // If the Ship_bullet reached the end of the map
				delete (*Ship_bullet); // It gets deleted
				Ship_bullet = Ship_Bullets.erase(Ship_bullet);
            }
        }

		ss.Move();		//move and draw
        gotoxy(56, 1);
        printf("%d", score);
        Sleep(30); // ** This is essential, otherwise the game would be unplayable **

	}
	
	if (!ss.isDead())  // If you died
        GameOverVictoryMessage();
    else  // If you won
        GameOverDefeatMessage();

	return 0;
}