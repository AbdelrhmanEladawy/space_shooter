#include <iostream>
#include <stdlib.h>
#include <stdio.h>     //Standard c and c++ libraries
#include <windows.h>   // To take control over the terminal ... and use sleep function
#include <conio.h>	   // to use getch() and kbhit() funcions
#include <list>
#include <random>
#include <time.h>
#include <chrono>
//#include <time.h>
//#include <unistd.h>

using namespace std;
using namespace std::chrono;

#define UP 72 // arrow keys' ascii numbers
#define LEFT 75
#define RIGHT 77
#define DOWN 80

// The default size of the Windows terminal is 25 rows x 80 columns
unsigned int SCREEN_WIDTH = 80;
unsigned int SCREEN_HEIGHT = 25;


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
//just to be more  smooth in printing
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
{
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
    printf("                 Press : ");    
    gotoxy(x, y + 7);
    printf("                       1. to start playing");
    gotoxy(x, y + 8);
    printf("                       2. to test mode to test time complexity");
    gotoxy(x, y + 9);
    printf("                       3. (or \"e\") to exit the game");
    gotoxy(x, y + 14);
    printf("team members : ");
    gotoxy(x, y + 15);
    printf("		Abdelrhman Ahmed");
	gotoxy(x, y + 16);
	printf("		Abdelrhman abdelnaser");
	gotoxy(x, y + 17);
	printf("		Hassan basiony");
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
    
    //check if the bullet reaches the bottom or top of the board
    bool isOut()
    {
		if (y <= 3 || y >= SCREEN_HEIGHT - 3 )
        {
            gotoxy(x, y);
            printf(" "); // It disappears
            return true; // And informs that it should no longer exist
        }
        else
            return false;
    }
   
    void Erase() //disappears the old one
    {
        gotoxy(x, y);
        printf(" ");
    }

    //change the coordinates of the bullet
    void Move(int dir = 1) //for direction ... ( 1:up , 2:down )
    {
		Erase(); //disappears the old one
		(dir == 1) ? (y--) : (y++) ; //if condition
		gotoxy(x, y);
		printf("*"); // The shape of the bullet
    }
};

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

    // Displays HP and energy points shape, I aligned them with the labels printed in DrawGameLimits
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

    // This is draw our spaceship body
    void Draw()
    {
        gotoxy(x, y);
        printf("  %c  ", 30);
        gotoxy(x, y + 1);
        printf("  %c  ", 4);
        gotoxy(x, y + 2);
        printf("%c%c%c%c%c", 17, 30, 223, 30, 16);
    }

    // To Erase the old spaceship from screen
    void Erase()
    {
        gotoxy(x, y);
        printf("     ");
        gotoxy(x, y + 1);
        printf("     ");
        gotoxy(x, y + 2);
        printf("     ");
    }

    // Triggered by the enemy ship buulet that hit the spaceship
    void Damage()
    {
        energy--;
        if (energy == 0)
        {
            Explosion();
        }
        else 
        {
            Erase(); // You can not omit this part, is meant to visually tell you that you were hit
            gotoxy(x, y);
            printf("  *  "); //damage shape
            gotoxy(x, y + 1);
            printf("  *  ");
            gotoxy(x, y + 2);
            printf("*****");
            Sleep(100);
        }
    }

    // When you lose a heart
    void Explosion()
    {
        hp--;
        Erase();
        gotoxy(x, y); //Explosion shape
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

        // If you still have a heart or more
        if (hp > 0)
            energy = 5;

        // If you don't
        else
            imDead = true;
    }

    //check if the Bullet Collision with our shape
	bool Collision(Bullet B)
	{   //+5 and +3 is the dimension of the shape
		if ( ( (B.X() >= x) && (B.X() <= x+5) ) && ( (B.Y() >= y) && (B.Y() <=y+3) ) ) 
        {
            B.Erase();
            Damage();
			return true;
        }

		else 
			return false;
	}

    // The main function of the spaceship
    //it is call draw function at the end
    void Move()
    {
        if (_kbhit())             //hit a key
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

class EnemyShip
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

	EnemyShip(int _x , int _y , int _hp = 1 )
	{
		x = _x;
        y = _y;
        hp = _hp;              
        is_Destroyed = false; // By default you are not dead
	};

	bool isDestroyed()
	{
		return is_Destroyed;
	}

    // This is draw our enemyship body
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

	// Triggered by the sapce ship bullet that hit the enemyship
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
		Erase();
        Sleep(100);
    }

    //check if the Bullet Collision with our shape
	bool Collision(Bullet B)
	{ //+5 and +3 is the dimension of the shape
		if ( ( (B.X() >= x) && (B.X() <= x+5) ) && ( (B.Y() >= y) && (B.Y() <=y+3) ) )
        {
            B.Erase();
            Damage(); // The enemy_ship_itr hurts
			return true;
        }

		else 
			return false;
	}

    //it is call draw function at the end
	void Move( int moving_dir = 0 )
	{
        Erase();
		if( moving_dir ) //for moving direction ... (0:stoped , 1:left , 2:rigth , 3:up , 4:down )
		{
			switch (moving_dir)
			{
			case 1:
				if (x > 2)
					x -= 1;
				break;
			case 2:
				if (x + 4 < SCREEN_WIDTH-3 )
					x += 1;
				break;
			case 3:
				if (y > 3)
					y -= 1;
				break;
			case 4:
				if (y + 2 < SCREEN_HEIGHT-3 )
					y += 1;
				break;
			default:
				break;
			}
		}

		Draw();
	}
};

void Game_Loop(int option = 1 , int N = 12) //option is the mode ,,, N : is number of enemys
{

    auto start = high_resolution_clock::now();

    int score = 0;
    
	SpaceShip space_ship( SCREEN_WIDTH/2 , SCREEN_HEIGHT-5 );
	space_ship.DrawSpaceShipInfo(); //draw info

	list<Bullet *> SpaceShip_Bullets;          // We will use a dynamic list for the Bullets in the game
    list<Bullet *>::iterator Ship_bullet_itr; // And an iterator for the list ... more faster

	list<EnemyShip *> enemy_ship;
	list<EnemyShip *>::iterator enemy_ship_itr;

	list<Bullet *> enemyShip_Bullets;
    list<Bullet *>::iterator enemy_bullet_itr;

    //********************* make enemyShip and add it to list  *********************
	int number_of_enemy = N;

    if (option == 1) //normal mode
        for(int i = 10 ;i <= number_of_enemy*5  ; i += 6 ) //5 is the size of ship
        {
            enemy_ship.push_back(new EnemyShip( i+2 , 3 ,1 ) );
        }

    if (option == 2) //time mode option test
        for(int i = (SCREEN_WIDTH)/2-2 , j = 3, k = 1 , m = 0 ; m < number_of_enemy ; (i = i < (SCREEN_WIDTH)/2 ? (SCREEN_WIDTH)/2 + 5*k++ +2 : (SCREEN_WIDTH)/2 - 5*k++ -2 ) , m++ )
        {
            if( i > SCREEN_WIDTH-5 || i < 3 )
            {
                i = (SCREEN_WIDTH)/2 + ( (j-3)/4 %2 == 0 ? 2 : -2) ;
                j+=4;
                k = 1;
            }
            enemy_ship.push_back(new EnemyShip( i+2 , j ,1 ) );
        }

    //timer : is for calc time ... timer_count : counter for bullets ... mov_dir : the direction of enemy ship moving
    //for moving direction ... (0:stoped , 1:left , 2:rigth , 3:up , 4:down )
    int timer = 0 , timer_count = 0 , mov_dir = 2; 
    bool test_mode_out = true; double  time_for_print=0; //for time test
	while ( !space_ship.isDead() && !enemy_ship.empty() )
	{
        if (timer == 10) //if the timer count 10, reset it.
            timer = 0;   //reset the timer
    
        timer++; //each loop , inc the timer

        //********************* add a new SpaceShip Bullet to list  *********************
		if (_kbhit())
        {
            char key = _getch();
            if (key == ' ')     // If you press the space bar you add a Ship_bullet to the Ship_bullet list
                SpaceShip_Bullets.push_back(new Bullet(space_ship.X() + 2, space_ship.Y() - 1));
        }

		for(enemy_ship_itr = enemy_ship.begin(); enemy_ship_itr != enemy_ship.end(); enemy_ship_itr++)
		{
        //********************* add a new enemy Ship Bullet to list  *********************
            if( rand()%10 == timer ) //if random number = to timer
            {
                timer_count++;       //counter +1 ,this count to make bullet more slower
                if(timer_count == 3)
                {
                    enemyShip_Bullets.push_back(new Bullet((*enemy_ship_itr)->X() + 2, (*enemy_ship_itr)-> Y() + 1));
                    timer_count = 0; //reset count
                }
            }
            
        //****************************** moving enemy ships  ******************************
           if(option == 2 )
                (*enemy_ship_itr)-> Move(0);
            else if(timer == 10)
            {
                if( mov_dir == 2) //2 means right
                {
                    if ( (*enemy_ship.back()).X() + 6 < SCREEN_WIDTH -1  )
                        (*enemy_ship_itr)-> Move(2); //2 means right
                    else
                        mov_dir = 1;
                }

                else if ( mov_dir == 1) //1 means left
                {
                    if( (*enemy_ship.front()).X() > 3 ) //3 is the begging of borad
                        (*enemy_ship_itr)-> Move(1); //1 means left
                    else
                        mov_dir = 2;
                }
                else
                    (*enemy_ship_itr)-> Move(0); //0 means constant , so just draw it
            }
		}

        //****************************** moving our ship bullets  *****************************
		for (Ship_bullet_itr = SpaceShip_Bullets.begin(); Ship_bullet_itr != SpaceShip_Bullets.end(); Ship_bullet_itr++)
        { // For every Ship_bullet that is in space
            (*Ship_bullet_itr)->Move(1); //1 means up
            if ((*Ship_bullet_itr)->isOut())
            {                     // If the Ship_bullet reached the end of the map
				delete (*Ship_bullet_itr); // It gets deleted
				Ship_bullet_itr = SpaceShip_Bullets.erase(Ship_bullet_itr);
            }
        }

        //***************************** moving enemyenemy  bullets  *****************************
		for (enemy_bullet_itr = enemyShip_Bullets.begin(); enemy_bullet_itr != enemyShip_Bullets.end(); enemy_bullet_itr++)
		{ // For every bullet that is in space
			(*enemy_bullet_itr)->Move(2); //2 means down
			if ((*enemy_bullet_itr)->isOut())
			{                     // If the bullet reached the end of the map
				delete (*enemy_bullet_itr); // It gets deleted
				enemy_bullet_itr = SpaceShip_Bullets.erase(enemy_bullet_itr);
                if(option == 2) //time mode option test
                    goto test_mode; //break the loop
			}
		}

        //******************* Collision enemy_ship with our ship bullets   *******************
		for (enemy_ship_itr = enemy_ship.begin(); enemy_ship_itr != enemy_ship.end(); enemy_ship_itr++)
		{
			for ( Ship_bullet_itr = SpaceShip_Bullets.begin() ; Ship_bullet_itr != SpaceShip_Bullets.end() ;Ship_bullet_itr++ )
			{										
				if ( (*enemy_ship_itr)-> Collision( **Ship_bullet_itr)  )
				{
					delete (*Ship_bullet_itr);
					Ship_bullet_itr = SpaceShip_Bullets.erase(Ship_bullet_itr);
					if( (*enemy_ship_itr)-> isDestroyed() )
					{
						delete (*enemy_ship_itr);
						enemy_ship_itr = enemy_ship.erase(enemy_ship_itr);
						score += 10;		// you get 10 points
					}
				}
			}
		}

        //******************* Collision our ship with enemy ship bullets   *******************
		for ( enemy_bullet_itr = enemyShip_Bullets.begin() ; enemy_bullet_itr != enemyShip_Bullets.end() ;enemy_bullet_itr++ )
		{										
			if ( space_ship.Collision( **enemy_bullet_itr)  )
			{
				delete (*enemy_bullet_itr);
				enemy_bullet_itr = enemyShip_Bullets.erase(enemy_bullet_itr);
                if(option == 2) //time mode option test
                    goto test_mode; //break the loop
			}
		}

        //*************************** move out ship and draw it   ***************************
		space_ship.Move();
        
        gotoxy(56, 1);
        printf("%d", score);
        
        if( test_mode_out == true && option == 2) //time mode option test
        {
            auto stop_1 = high_resolution_clock::now();
            auto duration_1 = duration_cast<microseconds>(stop_1 - start);
            time_for_print = (duration_1.count() / 1000.0);
            test_mode_out = false;
        }

        //************ this line of code is very important for visualization  ***************
        Sleep(50); // ** This is essential, otherwise the game would be unplayable **
	}
	
	if ( space_ship.isDead() )  // If you died
		GameOverDefeatMessage();
    else  // If you won
		GameOverVictoryMessage();

    Sleep(2000);

    test_mode : //label for goto
    auto stop_2 = high_resolution_clock::now();
    if (option == 2) //time mode option test
    {
        auto duration_2 = duration_cast<microseconds>(stop_2 - start);
        gotoxy(0,SCREEN_HEIGHT+1);
        
        cout<<"for "<<number_of_enemy<<" enemy \t and \t "<<SCREEN_WIDTH<<" X "<<SCREEN_HEIGHT<<" dimensions"<<"\n";
        cout<<"********************* time for just print the game and start it *********************\n";
        cout << "time for just print the game is:- " << time_for_print/ 1000.0 << " milliseconds\n"<< endl;
        cout << "time for just print the game is:- " << time_for_print/ 1000000.0 << " seconds\n"<< endl;

        cout<<"********************* time for  time for 1 full loop iteration **********************\n";
        cout << "time for  time for 1 full loop is:- " << (duration_2.count() / 1000.0) << " milliseconds\n"<< endl;
        cout << "time for  time for 1 full loop is:- " << (duration_2.count() / 1000000.0) << " seconds\n"<< endl;
        cout << "So , multiply this number by N loop of game until game over or win"<< endl;

        cout<<"\n\n Press any key to continue";
        _getch();
    }

    system("cls");
    return;
}

int main()
{
    srand((unsigned)time(NULL));
	HideCursor();
    system("cls");

    do
    {
        WelcomeMessage();
        gotoxy(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
        char op = getche();

        if (op == '1' || op == ' ')
        {
            system("cls");
            DrawGameLimits();
            Game_Loop(1);
        }
        else if (op == '2' || op == 't' )
        {
            system("cls");
            int N ,sc_w,sc_h;
            cout<<"enter the width of screen : ";
            cin>>sc_w;
            SCREEN_WIDTH = sc_w;
            cout<<"enter the hight of screen : ";
            cin>>sc_h;
            SCREEN_HEIGHT = sc_h;
            cout<<"enter the number of enemy : ";
            cin>>N;
            system("cls");
            DrawGameLimits();
            Game_Loop(2,N); //test_mode
        }
        else if (op == '3' || op == 'e' || op == 'E' )
        {
            gotoxy(0,SCREEN_HEIGHT+1);
            exit(0);
        }

    } while (1);

	return 0;
}