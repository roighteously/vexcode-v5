#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;


// START V5 MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END V5 MACROS


// Robot configuration code.
bumper GroundBumCtrl = bumper(Brain.ThreeWirePort.A);
bumper OneBumCtrl = bumper(Brain.ThreeWirePort.B);
bumper TwoBumCtrl = bumper(Brain.ThreeWirePort.C);
motor Motor1 = motor(PORT1, ratio18_1, false);

limit OneLim = limit(Brain.ThreeWirePort.D);
limit TwoLim = limit(Brain.ThreeWirePort.E);
bumper GroundBum = bumper(Brain.ThreeWirePort.F);
bumper TwoBumElev = bumper(Brain.ThreeWirePort.G);
#pragma endregion VEXcode Generated Robot Configuration

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       {author}                                                  */
/*    Created:      {date}                                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// Include the V5 Library
#include "vex.h"
  
// Allows for easier use of the VEX Library
using namespace vex;

// Initialize useful variables
int touchX = 0;
int touchY = 0;
int floorID = 0;
char* floorStr = "Ground"; // We know we will start at Ground level

void initGUI() { // This is the funky touch screen GUI that lets us visualize the cool things
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1,1);
  Brain.Screen.setFillColor(blue);
  Brain.Screen.drawRectangle(0, 0, 500, 250);
  Brain.Screen.setFillColor(green);
  Brain.Screen.drawRectangle(200,0,50,250);
  Brain.Screen.drawRectangle(300,0,50,250);
  Brain.Screen.drawRectangle(400,0,50,250);
  Brain.Screen.setFillColor(red);
  // Setup the cool background ^

  // Now the text \/
  Brain.Screen.print("Project 5.2.10");
  Brain.Screen.newLine();
  Brain.Screen.print("The Baskelevator by AML");
  Brain.Screen.newLine();
  Brain.Screen.newLine();
  Brain.Screen.print("Floor ID: %d", floorID); // Allow us to visualize floor internal ID
  Brain.Screen.newLine();
  Brain.Screen.print("Floor: %s", floorStr); // Allow us to see the floor name (Ground, one, two)
  Brain.Screen.newLine();
  Brain.Screen.newLine();
  Brain.Screen.print("Touch X: %d", touchX); // Where we have touched the screen (X & Y)
  Brain.Screen.newLine();
  Brain.Screen.print("Touch Y: %d", touchY);

  // Label the touch screen buttons
  Brain.Screen.setCursor(7,19);
  Brain.Screen.print("Ground");

  Brain.Screen.setCursor(7,32);
  Brain.Screen.print("One");

  Brain.Screen.setCursor(7,42);
  Brain.Screen.print("Two");
  Brain.Screen.setCursor(10,42);
}

void goFloorG() {
  if(floorID == 0) return;
  Motor1.spin(forward); // We know we will always need to go down as there is no lower floor
  floorID = 0;
  floorStr = "Going to Ground";
  waitUntil(GroundBum.pressing()); 
  // Wait until ground sensor is pressed, now stop and go up a bit to ensure nothing is damaged.
  floorStr = "Ground";
  Motor1.stop();
  Motor1.spin(reverse);
    wait(150, msec);
  Motor1.stop();
}

void goFloorOne() {
  if(floorID == 1) return; // Check if we are on floor 1, if not
  Motor1.spin(forward); // Go down (for coming from floor 2), however
  if(floorID == 0)  { // if we're on floor 0/ground, go up
    Motor1.spin(reverse);
  }
  waitUntil(!OneLim.pressing() || !TwoLim.pressing()); // Make sure nothing is being pressed
  floorID = 1;
  floorStr = "Going to One";
  waitUntil(OneLim.pressing() || TwoLim.pressing()); 
  // Now make sure sensors are pressed because for sure we are at the floor.
  Motor1.stop();
  floorStr = "One";
}

void goFloorTwo() {
  if(floorID == 2) return; // Check to make sure we are already on floor 2, if not then continue
  Motor1.spin(reverse); // We know we need to go down as this is the highest floor
  floorID = 2; // Set the floor ID
  floorStr = "Going to Two";
  waitUntil(TwoBumElev.pressing()); // Wait until the top sensor has been pressed
  floorStr = "Two"; // Now we've arrived, stop the motor and display fancy text onscreen.
  Motor1.stop();
}

void touchProcess() {
  // Control the screen for the elevator: when pressed in an area go to that floor
  touchX = Brain.Screen.xPosition();
  touchY = Brain.Screen.yPosition();
  if(touchX > 200 && touchX < 250 && floorID != 0) {
    goFloorG(); // If we are not on floor 0, and screen is touched in the area of the button
                // go to the ground floor.
  }
  if(touchX > 300 && touchX < 400 && floorID != 1) {
    goFloorOne();// If we are not on floor 1, and screen is touched in the area of the button
                // go to the first floor.
  }
  if(touchX > 400 && touchX < 500 && floorID != 2) {
    goFloorTwo();// If we are not on floor 2, and screen is touched in the area of the button
                // go to the second floor.
  }
}

// The Baskelevator:
// This elevator is one of the most elevating projects made in elevator history.
// By allowing control from each floor, the elevator will go to said floor using sensors.
// With a gimmick, touch controls, we have innovated the elevation industry to ensure we are the most
// elevating elevator made for days where you need to be elevated.
int main() {  
  Brain.Screen.released(touchProcess); // When screen is pressed: process corresponding button
  GroundBumCtrl.released(goFloorG); // When the Ground button is pressed, go to Ground floor.
  OneBumCtrl.released(goFloorOne); // When the 1st floor button is pressed, go to 1st floor.
  TwoBumCtrl.released(goFloorTwo); // When the 2nd floor button is pressed, go to 2nd floor.
  while(true) {
    initGUI(); // Make the screen draw :yay:
    // Also allows us to have graphics and able to see where to press for touch controls
    wait(16, msec); // Best possible refresh rate to make scanline as removable as possible
  }
  
}
