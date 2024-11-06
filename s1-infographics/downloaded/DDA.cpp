#include<iostream>
#include<graphics.h>
using namespace std;

int main(){
	float x,y,x1,y1,x2,y2,dx,dy,step,Xinc,Yinc;
	int i,gd,gm;
	
	initgraph(&gd,&gm,NULL);
	
	cout<<"Enter the value of x1 and y1 : ";
	cin>>x1>>y1;
	cout<<"Enter the value of x2 and y2: ";
	cin>>x2>>y2;

	dx=(x2-x1);
	dy=(y2-y1);
 
	if(dx>=dy)
		step=dx;
	else
		step=dy;
 
	Xinc=dx/step;
	Yinc=dy/step;
 
	x=x1;
	y=y1;
 
	i=0;
	
	while(i<=step)
	{
		putpixel(x,y,5);
		x=x+Xinc;   // x+=Xinc
		y=y+Yinc;
		i=i+1;
		delay(60);		
	}
	return 0;
}