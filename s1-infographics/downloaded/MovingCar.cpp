//program to simulate vehicle locomotion

#include<iostream>
#include<graphics.h>
using namespace std;
int main()
{
int gdriver=DETECT,gmode,i=0,j=0;
initgraph(&gdriver,&gmode,NULL);
for(i;i<420;i++)
{
setcolor(YELLOW);
line(0,245,650,245); //road
line(0+i,200,210+i,200); //car middle line
line(50+i,200,70+i,170); //back glass
line(70+i,170,140+i,170); //roof line(upper)
line(140+i,170,160+i,200); //front glass
line(85+i,170,85+i,200); //vertical back window line
line(125+i,170,125+i,200); //vertical front window line
line(0+i,200,0+i,230); //dickey vertical
line(210+i,200,210+i,230); //bonat vertical
line(0+i,230,50+i,230); //back bottom horizontal
 circle(65+i,230,15); //back wheel circle
line(80+i,230,130+i,230); //middle bottom horizontal
circle(145+i,230,15); //front wheel circle
line(210+i,230,160+i,230); //front bottom horizonal
 pieslice(65+i,230,359-j,360-j,15); //back wheel lines 
pieslice(65+i,230,179-j,180-j,15);  //back wheel lines
pieslice(65+i,230,89-j,90-j,15); //back wheel lines
pieslice(65+i,230,269-j,270-j,15); //back wheel lines
pieslice(145+i,230,359-j,360-j,15); //front wheel lines.
pieslice(145+i,230,179-j,180-j,15); //front wheel lines
pieslice(145+i,230,89-j,90-j,15); //front wheel lines
pieslice(145+i,230,269-j,270-j,15); //front wheel lines

if(j==179)
j=0; 
++j;
delay(500);//speed
cleardevice();//to clear previous pixels
}
// closegraph();
return 0;
}