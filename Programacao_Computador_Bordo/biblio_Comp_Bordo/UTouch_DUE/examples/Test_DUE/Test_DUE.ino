// UTouch_QuickDraw (C)2010-2012 Henning Karlsen
// Modified for CTE Shield for Arduino DUE and CTE LCD Module with Font IC
// web: http://stores.shop.ebay.com/coldtears-electronics-store
// web: http://www.henningkarlsen.com/electronics
//
// This program is a quick demo of how to use the library.
//
// This program requires the CTE modified UTFT library.
//
// It is assumed that the display module is connected to an
// appropriate shield or that you know how to change the pin 
// numbers in the setup.
//
#include "SPI.h"
#include <UTFT.h>
#include <UTouch.h>
#define FLASH_CS_PIN 52 
#define SPI_RATE 2
extern uint8_t BigFont[];
UTFT        myGLCD(CTE50);   // Remember to change the model parameter to suit your display module!
UTouch      myTouch(6,5,32,3,2);
int x, y;
char stCurrent[20]="";
int stCurrentLen=0;
char stLast[20]="";

void setup()
{
// Initial setup
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();

  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_MEDIUM);

  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(0, 0, 255);
  

}
// Draw a red frame while a button is touched
void waitForIt(int x1, int y1, int x2, int y2)
{
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
  while (myTouch.dataAvailable())
    myTouch.read();
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
}

void loop()
{
  while (true)
  {
    if (myTouch.dataAvailable())
    {
      myTouch.read();
      x=myTouch.getX();
      y=myTouch.getY();
      
    
        if ((x>=100) && (x<=200))  // Button: 2
        {
          myGLCD.setColor(255, 0, 0);
           myGLCD.print("TOUCHED", CENTER, 192);
            delay(500);
            myGLCD.print("            ", CENTER, 192);
            delay(500);
        }
        if ((x>=200) && (x<=300))  // Button: 3
        {
            myGLCD.setColor(255, 0, 0);
            myGLCD.print("TOUCHED", CENTER, 192);
            delay(500);
            myGLCD.print("            ", CENTER, 192);
            delay(500);
        }
        if ((x>=300) && (x<=400))  // Button: 4
        {
           myGLCD.setColor(255, 0, 0);
            myGLCD.print("TOUCHED", CENTER, 192);
            delay(500);
            myGLCD.print("            ", CENTER, 192);
            delay(500);
        }
        if ((x>=400) && (x<=500))  // Button: 5
        {
           myGLCD.setColor(255, 0, 0);
            myGLCD.print("TOUCHED", CENTER, 192);
            delay(500);
            myGLCD.print("            ", CENTER, 192);
            delay(500);
        }
      }


      
      }
    }

