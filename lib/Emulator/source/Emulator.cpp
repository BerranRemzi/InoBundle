#include <graphics.h>
#include <iostream>
#include <stdlib.h>
#include "Arduino.h"

int APIENTRY WinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR     lpCmdLine,
  int       nShowCmd
)
{
   initwindow(400, 400);
   setup();
   //int c = Test(5, 10);
   if(false == Serial.isOpen()){
      FreeConsole();
   }
   //int gd = DETECT, gm;
   int x = 320, y = 240, radius;
       
   //initgraph(&gd, &gm, "C:\\TC\\BGI");
   
   for ( radius = 25; radius <= 125 ; radius = radius + 20){
      circle(x, y, radius);
	  //Serial.print(kbhit());
	  getch();
   }
       
   getch();
   closegraph();
   return 0;
}

