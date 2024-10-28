// program to draw inscribed and circumscribed circles in the triangle

# include <graphics.h>
# include <math.h>
# include <iostream>
using namespace std ;

void DDALine(int x1,int y1,int x2,int y2,int iColor);

int main()
{
int x1,x2,y1,y2,r,r1,iColor;
int gd,gm; gd=DETECT;
initgraph(&gd,&gm,NULL);
  

// Triangle
DDALine(100,113.4,50,200,7); 
DDALine(50,200,150,200,7); // horizontal
DDALine(150,200,100,113.4,7);

// Inside Circle
r=50/sqrt(3);
x1=(100+50+150)/3;
y1=(200+200+113.4)/3;
circle(x1,y1,r);


//OutSide Circle
r1=100/sqrt(3);
circle(x1,y1,r1);
delay(10000);
  
    return 0;
}



void DDALine(int x1,int y1,int x2,int y2,int iColor)
{
    float dX,dY,Step;
    float xInc,yInc,i,x,y;

    dX = x1 - x2;
    dY = y1 - y2;

    if (fabs(dX) > fabs(dY))
    {
        Step = fabs(dX);
    }
    else
    {
        Step = fabs(dY);
    }

    xInc = dX/Step;
    yInc = dY/Step;

    x = x1;
    y = y1;
    
    for (i=1; i<=Step; i++)
    {
        putpixel(x,y,7);
        x -= xInc;
        y -= yInc;
    }
    
    return ;
}
