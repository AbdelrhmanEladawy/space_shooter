# Space Shooter Game
**Space Shooter Game**
It's an arcade game meant for those who love adventure and challenges, *developed by:* Abdelrahman Ahmed, Abdelrahman Abdelnaser and Hassan Osama, the game is based on **C++.**
Basically, the user controls a spaceship and faces enemy ships that are controlled by the computer.
The attack method is bullets from both sides.\
For time complexity measurement, we change some parameters like width and height, and number of enemies, and check the difference.

![alt text](https://github.com/AbdelrhmanEladawy/space_shooter/blob/main/space_shooter.PNG?raw=true)

The game was developed for Embedded Systems course project, EECE department, Cairo University.

## How to run
any **c/c++ editor** with **MinGW** Compiler is enough.
for C++ version, it's _recommended_ to use **C++11** or higher.\
for example: VS code.

## flow of the project 
First, I draw the Welcome Message, Spaceship and enemy ship, and border for the game. then it's entered an infinite loop which will leave it when the spaceship is dead or destroy all the enemy ships. \
inside the loop, I check if there is a button clicked or not. For example, if "space" is clicked, that will add a new bullet to list data structure. Also, it checks if the bullets reach the ships or not, and checks for collision between bullets and ships. Moreover, there is a counter to how many hearts are still alive.
