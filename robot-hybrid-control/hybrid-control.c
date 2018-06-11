/**
* Author: Claudio M Copppola (clc20)
* Robot: 61
* BT: FA103254
* Version: 1.0
* Description: This program is designed for the Formula AllCode. The robot 
* is to explore a simple maze and record information about it in an internal
* data structure.    
*/

#include "allcode_api.h"
#include "stdlib.h"

#define LEFTWHEEL 22
#define RIGHTWHEEL 24
#define DRIVESPEED 20
#define ROWS 4
#define COLS 4
#define CELLS 16
#define NORTH 0
#define EAST 90
#define SOUTH 180
#define WEST 270

unsigned short maze[ROWS][COLS] = {{0}};

unsigned short crumbRoute[ROWS][COLS] = {{0}};

unsigned short crumbs = 1;
unsigned short curCrumb = 0;

unsigned short posX = 1; // Starting X position
unsigned short posY = 0; // Starting Y position

unsigned short nestX;
unsigned short nestY;

unsigned short mazeExplored = 0;
unsigned short nestLocated = 0;

int compass = 0;

unsigned short startX = 0;
unsigned short endX = 60;
unsigned short startY = 0;
unsigned short endY = 10;

/**
* The robot looks through the cells in the maze and checks whether it has seen all 16 cells.
*/
void checkMaze()
{
	unsigned short x = 0;
	unsigned short y = 0;
	unsigned short cell = 0; 
	
	for(x = 0; x < ROWS; x++)
	{
		for(y = 0; y < COLS; y++)
		{
			if (maze[x][y] == 1)
			{
				cell++;	
			}
			if (cell == CELLS)
			{
				FA_BTSendString("\n***ALL CELLS LOCATED***\n\n",35);
				mazeExplored = 1; // Sets mazeExplored to true
				// Two beeps indicate that the robot has checked every cell in the maze
				FA_PlayNote (500, 250);
				FA_DelayMillis(200);
				FA_PlayNote (500, 250);
				FA_DelayMillis(200);
			}
		}
	}			
}

/**
* updateMaze() will update the robots position on the maze and 
* also draws the maze on the LCD for each cell visited.
*/
void updateMaze()
{	
	switch (compass)
	{
		case NORTH:	
		
			FA_BTSendString("NORTH\n",15);
			startX = endX;
			startY = endY;
			endY = endY + 4;
			posY = posY + 1;
			maze[posX][posY] = 1;
			break;
		
		case EAST:
		
			FA_BTSendString("EAST\n",15);
			startX = endX;
			startY = endY;
			endX = endX + 4;
			posX = posX + 1;
			maze[posX][posY] = 1;
			break;
		
		case SOUTH:
		
			FA_BTSendString("SOUTH\n",15);
			startX = endX;
			startY = endY;
			endY = endY - 4;
			posY = posY - 1;
			maze[posX][posY] = 1;
			break; 
		
		case WEST:
	
			FA_BTSendString("WEST\n",15);
			startX = endX;
			startY = endY;
			endX = endX - 4;
			posX = posX - 1;
			maze[posX][posY] = 1;
			break;
	}
	
	FA_LCDLine(startX,startY, endX, endY); // Draws the positions on the LCD

	// Sends position over bluetooth
	FA_BTSendString(" (",15);
	FA_BTSendNumber(posX);
	FA_BTSendString(",",2);
	FA_BTSendNumber(posY);
	FA_BTSendString(")\n",5);		
}

/**
* updateCompass() will take the current postion on the compass, direction and it's heading and 
* calculates it's new compass position. 
*
* Note: Wherever the robot faces when the program starts will be maze North. As there would be interference 
* on embedded compass, using this compass is much more efficient for navigation.  
*
* @param degrees where the robot is facing (N,E,S,W).
* @param direction is what what way the robot is turning (left or right).
* @param heading which way the robot will be facing.
* 
* @return degrees will return the new compass 
*/
int updateCompass(degrees,direction,heading)
{
	
	// Checks whether the direction is pointing to left or right
	if(direction == 0)
	{
		degrees = degrees + heading;
	}
	else if(direction == 1)
	{
		degrees = degrees - heading;
	}

	// This section calculates the heading of the compass by checking which value is close 
	if (degrees < -225)
	{
		degrees = SOUTH; 
	}
	else if (degrees < 0)
	{
		degrees = WEST; 
	}
	else if (degrees < 45)
	{
		degrees = NORTH; 
	}
	else if (degrees < 135)
	{
		degrees = EAST;
	}
	else if (degrees < 225)
	{
		degrees = SOUTH; 
	}
	else if (degrees < 315)
	{
		degrees = WEST;
	}
	else if (degrees < 405)
	{
		degrees = NORTH; 
	}
	else if (degrees < 495)
	{
		degrees = EAST; 
	}

	return degrees;		
}

/**
* checkNest() will check that the cell is in the nest. Once the nest has been detected for the first time it will do 1 beep, when it finds 
* the nest again it will do 3 beeps and end the program.    
*
* @param nestFound is true or false. True if nest has been found, false if not.
*
* @return 1 True if nest is already found
* @return 0 False if nest is not found yet
*/
int checkNest(nestFound)
{
	switch (nestFound)
	{
		
		case 0:

			if (FA_ReadLight() < 400)
			{
				FA_PlayNote (500, 250); // If the robot has detected the nest it will make a sound
				FA_Right(180);	// Turns right 180 degrees (IR sensors have difficulty reading dark surfaces)
				compass = updateCompass(compass, 0 ,180); 
				nestLocated = 1;
				nestX = posX; // Adds X position to nest X position
				nestY = posY; // Adds Y position to nest Y position
				FA_BTSendString("***NEST LOCATED***\n\n",30);	
	
				return 1;
			}
			else 
			{
				return 0;
			}
			break;
	
		case 1:
			
			// When the nest has been located and the postions match and the maze has been explored then end the program 
			if(nestLocated == 1 && nestX == posX && nestY == posY && mazeExplored == 1)
			{	
				FA_PlayNote (500, 250);
				FA_DelayMillis(200);
				FA_PlayNote (500, 250);
				FA_DelayMillis(200);
				FA_PlayNote (500, 250);
				FA_BTSendString("***NEST REACHED***",20);	
				exit(EXIT_SUCCESS);
			}
			break;
	}

	return 0;
}

/**
* checkFront() uses the two front sensors to check if whether the robot is approaching an obstruction. If true it moves back and turns left or right until theres
* no obstruction infront of the robot. 
*/
void checkFront()
{
	while (FA_ReadIR(1) > 200)
	{
		
		FA_SetMotors(LEFTWHEEL,0);
		FA_DelayMillis(200);
	}	
	while (FA_ReadIR(3) > 200)	
	{	
		FA_SetMotors(0,RIGHTWHEEL);
		FA_DelayMillis(200);
		
	}
	FA_SetMotors(LEFTWHEEL,RIGHTWHEEL);
}

/**
* checkEnvironment() will follow the the right wall. It checks the right, front, left and if at a dead end will perform a turn around. 
*/
void checkEnvironment()
{
	if (FA_ReadIR(4) < 50)
	{
		// If cell to the right is open then turn right 90 degrees
		FA_Right(90);
		compass = updateCompass(compass, 0 , 90); 
	}
	else if (FA_ReadIR(2) < 50)
	{
		// Keep moving forward 
		compass = updateCompass(compass, 0 , 0); 
	}
	else if (FA_ReadIR(0) < 50)
	{		
		// If cell to the left is open then turn left 90 degrees
		FA_Left(90);
		compass = updateCompass(compass, 1 , 90); 
	}
	else	
	{
		// If at a dead end turn right 180 degrees
		FA_Right(180);
		compass = updateCompass(compass, 0 , 180); 
	}
}
/**
* checkCrumbs() sets it's current postion on the bread route to crumb value it's on. Depending on the the compass will determine where the robot checks, it compares it's current position with the cell in 
* front, then right and left to see if the value is lower than the current crumb. This will continue until it reaches the nest which is crumb 1. 
*/
void checkCrumbs()
{	
	curCrumb = crumbRoute[posX][posY];
	FA_BTSendString("CURRENT CRUMB ",20);
	FA_BTSendNumber(curCrumb);
	FA_BTSendString(" (",5);
	FA_BTSendNumber(posX);
	FA_BTSendString(",",2);
	FA_BTSendNumber(posY);
	FA_BTSendString(")\n",5);

	switch (compass)
	{
		case NORTH:	

			if (FA_ReadIR(2) < 50 && crumbRoute[posX][posY + 1] < curCrumb && crumbRoute[posX][posY + 1] != 0)
			{ 
				compass = updateCompass(compass, 0 , 0); 
			}
			else if (FA_ReadIR(4) < 50 && crumbRoute[posX + 1][posY] < curCrumb && crumbRoute[posX + 1][posY] != 0)
			{
				FA_Right(90);
				compass = updateCompass(compass, 0 , 90); 
			}
			else if (FA_ReadIR(0) < 50 && crumbRoute[posX - 1][posY] < curCrumb && crumbRoute[posX - 1][posY] != 0)
			{
				FA_Left(90);
				compass = updateCompass(compass, 1 , 90); 
			} 
			break;
	
		case EAST:

			if (FA_ReadIR(2) < 50 && crumbRoute[posX + 1][posY] < curCrumb && crumbRoute[posX + 1][posY] != 0)
			{ 
				compass = updateCompass(compass, 0 , 0); 
			}
			else if (FA_ReadIR(4) < 50 && crumbRoute[posX][posY - 1] < curCrumb && crumbRoute[posX][posY - 1] != 0) 
			{
				FA_Right(90);
				compass = updateCompass(compass, 0 , 90); 
			}	
			else if (FA_ReadIR(0) < 50 && crumbRoute[posX][posY + 1] < curCrumb && crumbRoute[posX][posY + 1] != 0)
			{
				FA_Left(90);
				compass = updateCompass(compass, 1 , 90); 
			} 
			break;

		case SOUTH:

			if (FA_ReadIR(2) < 50 && crumbRoute[posX][posY - 1] < curCrumb && crumbRoute[posX][posY - 1] != 0)
			{ 
			compass = updateCompass(compass, 0 , 0); 
			}
			else if (FA_ReadIR(4) < 50 && crumbRoute[posX - 1][posY] < curCrumb && crumbRoute[posX - 1][posY] != 0)
			{
				FA_Right(90);
				compass = updateCompass(compass, 0 , 90); 
			}
			else if (FA_ReadIR(0) < 50 && crumbRoute[posX + 1][posY] < curCrumb && crumbRoute[posX + 1][posY] != 0)
			{
				FA_Left(90);
				compass = updateCompass(compass, 1 , 90); 
			}
			break;

		case WEST:
		
			if (FA_ReadIR(2) < 50 && crumbRoute[posX - 1][posY] < curCrumb && crumbRoute[posX - 1][posY] != 0)
			{ 
			compass = updateCompass(compass, 0 , 0); 
			}
			else if(FA_ReadIR(4) < 50 && crumbRoute[posX][posY + 1] < curCrumb && crumbRoute[posX][posY + 1] != 0) 
			{
				FA_Right(90);
				compass = updateCompass(compass, 0 , 90); 	
			}
			else if (FA_ReadIR(0) < 50 && crumbRoute[posX][posY - 1] < curCrumb && crumbRoute[posX][posY - 1] != 0) 
			{	
				FA_Left(90);
				compass = updateCompass(compass, 1 , 90); 
			}
			break;
	}	
}

/**
* stopMotors() will have a delaying process before stopping the motors and resetting the encoders. 
*/
void stopMotors()
{
		// LED ON notifies the user that a line has been detected
		FA_LEDOn(3); 
		FA_LEDOn(4);
		FA_DelayMillis(430);
		FA_LEDOff(3);
		FA_LEDOff(4);
		FA_SetMotors(0,0); // Stops in the middle of the cell
		FA_ResetEncoders();
}

/**
* setMotors() sets the amount of power tto each of the wheels and drive speed for moving and turning.
*
* Note: The performance of the robots movement are determined by it's battery life. If below 80% the robot will veer to the 
* left more which may affect it's postion on the cell and distance when it's sensors read values.  
*/
void setMotors()
{
	FA_SetMotors(LEFTWHEEL,RIGHTWHEEL);
	FA_SetDriveSpeed(DRIVESPEED);	
}

/**
* findNest() is the second operation that robot performs when searching for nest. It'll check if the cell it's currently on is in fact the nest, then 
* it will check for obstacles and then move forwards. Once it has detected a line the robot updates the map and goes through it's delay process before reaching 
* the middle of the new cell. The robot chhecks it's environment to see which crumb the robot should follow. 
*/
void findNest()
{
	checkNest(1);

	checkFront();	
	
	setMotors();	
	
	if(FA_ReadLine(0) < 200 && FA_ReadLine(1) < 200) // Checks both line sensors 
	{	
		updateMaze();			
		
		stopMotors();
	
		FA_DelayMillis(500); // Comment out this function to see the robot find the nest faster
	
		checkCrumbs();
	}	
}

/**
* exploreMaze() is the first operation that the robot performs whilst explorating the maze. It checks if there are obstructions in front so that it's wheels or sides don't
* get caught or the robot hits a wall, it also sets the motors to drive forwards. Once the robot detects a line it updates the maze and performs it's
* delaying process before reaching the middle of the cell. Checks if the cell is the nest, if not then it checks it's environment.
* Once the nest has been detected it will leave virtual crumbs as it carries on exploring the maze until every cell has been found.   
*/
void exploreMaze()
{
	unsigned short nestDetected = 0;

	checkFront();
	
	setMotors();

	if(FA_ReadLine(0) < 200 && FA_ReadLine(1) < 200) // Checks both line sensors for finding lines
	{	
		updateMaze();			
		
		stopMotors();
		
		FA_DelayMillis(1000);
	
		nestDetected = checkNest(0);

		if (nestDetected == 0)
		{
			checkEnvironment();
		}	

		if(nestLocated == 1 && crumbRoute[posX][posY] == 0)
		{	
			// Leaves virtual crumb at current position
			crumbRoute[posX][posY] = crumbs;
			curCrumb = crumbRoute[posX][posY];
			FA_BTSendString("CURRENT CRUMB ",20);
			FA_BTSendNumber(crumbs);
			FA_BTSendString(" (",5);
			FA_BTSendNumber(posX);
			FA_BTSendString(",",2);
			FA_BTSendNumber(posY);
			FA_BTSendString(")\n",5);
			crumbs++;
		}
		else 
		{
		// Sets it's current crumb to the position it is at on the crumb route 
		curCrumb = crumbRoute[posX][posY];	
		}
		checkMaze();
	}
}

/**
* main() has a standard and bluetooth option that the user can select before the robot explores the maze. Standard will activate the robot and 
* allow it to perform normally. Bluetooth will update the user on a terminal about it positions, nest location, crumbs, cells update
* and when it has finished, as long as it's connected to a device.       
*
* @return 0
*/
int main()
{
	FA_RobotInit();
    
    FA_LCDBacklight(50);

	maze[posX][posY] = 1; // Sets it's starting position as found

	while (1)
	{
		FA_LCDPrint("RB(0) STANDARD", 20, 0, 0, 0, 1); // Press the right button for standard 
		FA_LCDPrint("LB(1) BLUETOOTH", 20, 0, 8, 0, 1); // Press the left button for bluetooth (Must have a device that connects to the robot)
		if (FA_ReadSwitch (0) == 1) // Standard option
		{
			FA_LCDClear();
			FA_DelayMillis(2000);
			checkEnvironment();
			do
			{
				exploreMaze();
			}
			while(mazeExplored != 1);
			while(1)
			{
				findNest();
			}
		}
		else if (FA_ReadSwitch(1) == 1) // Bluetooth option
		{
			FA_LCDClear();
			while(1)
			{
				if (FA_BTConnected() == 0) // If there is no connection then don't start the program
				{
					FA_LCDPrint("NOT CONNECTED", 20, 0, 0, 0, 1);
					FA_DelayMillis(1000);	
					FA_LCDClear();
				}
				else // Run the program when the devices connnect 
				{
					FA_LCDPrint("CONNECTED", 20, 0, 0, 0, 1);
					FA_BTSendString("CONNECTED\n", 20);
					FA_DelayMillis(2000);
					FA_LCDClear();
					checkEnvironment();		
					do
					{
						exploreMaze();
					}
					while(mazeExplored != 1);
					FA_BTSendString("***FIND NEST***\n",20);
					while(1)
					{
						findNest();
					}
				}
			}	
		}
	}

 	return 0;
}