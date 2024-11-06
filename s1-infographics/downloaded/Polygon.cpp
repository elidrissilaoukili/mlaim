
/* PROGRAM FOR POLYGON (SCAN LINE ALGORITHM)

 */

#include<iostream>
#include<graphics.h>
using namespace std;
class polygon
{
	int xy[10][2];
	int v;

public:
	void read();
	void display();
	void sort(int a[10],int n);
	void scanline();
};

void polygon :: read()
{
	do
	{
		cout<<"\nEnter the total number of vertices in polygon : ";
		cin>>v;
		if(v<3)
			cout<<"\nError !!!\n polygon must have at least three vertices\nTRY AGAIN";
	}while(v<3);

	for(int i=0;i<v;i++)
	{
		cout<<"\nEnter vertex no. "<<i+1<<" (eg: x y ) : ";
		cin>>xy[i][0];
		cin>>xy[i][1];

	}

}

void polygon :: display()
{
	for(int j=0;j<v-1;j++)
	{
		line(xy[j][0],xy[j][1],xy[j+1][0],xy[j+1][1]);

	}
	line(xy[v-1][0],xy[v-1][1],xy[0][0],xy[0][1]);
}

void polygon :: sort(int a[],int n)
{
	int temp;

	for (int i=0;i<n-1;i++)			//Bubble sort
	{
		for(int g=0;g<n-1;g++)
		{
			if(a[g]>a[g+1])
			{
				temp=a[g];
				a[g]=a[g+1];
				a[g+1]=temp;

			}

		}

	}

}

void polygon :: scanline()
{
	int ymin=xy[0][1];
	for(int i=0;i<v;i++)
	{
		if(ymin>xy[i][1])
		{
			ymin=xy[i][1];

		}

	}

	int ymax=xy[0][1];
		for(int i=0;i<v;i++)
		{
			if(ymax<xy[i][1])
			{
				ymax=xy[i][1];

			}

		}

	int j;
	float m[10],dx,dy;
	for( j=0;j<v-1;j++)
		{
			dy=xy[j+1][1]-xy[j][1];
			dx=xy[j+1][0]-xy[j][0];
			if(dx==0)
				m[j]=0;
			if(dy==1)
				m[j]=1;
			else
				m[j]=dy/dx;
		}
	dy=xy[0][1]-xy[j][1];
	dx=xy[0][0]-xy[j][0];

	if(dx==0)
		m[j]=0;
	if(dy==1)
		m[j]=1;
	else
		m[j]=dy/dx;

	int cnt,xint[10];
	for(int y=ymin;y<=ymax;y++)
	{
		cnt=0;
		for(int i=0;i<v;i++)
		{
			if((y>xy[i][1] && y<=xy[i+1][1]) || (y<xy[i][1] && y>=xy[i+1][1]))
			{

				int temp_xint=xy[i][0] + (1/m[i])*(y-xy[i][1]);

				int flag=0,x1,y1,x2,y2,l, ignore=0;

				for(l=0;l<v-1;l++)
				{
					if((temp_xint==xy[l][0]) &&(y==xy[l][1]) ) //checking if it is a vertex
					{
						flag=1;

						break;

					}
				}

				if(flag==1)  //find 2 y values
				{
					if(l==0)
						y1=xy[v-1][1];
					else
						y1=xy[l-1][1];


					y2=xy[l+1][1];


					//check whether the y values lie on same side of cuurent y

					if((y1>y&&y2>y)|| (y1<y && y2<y))
					{
					ignore=1;
					}

				}


				if(ignore==0)
				{
					xint[cnt]=temp_xint;
					cnt++;
				}

			}

		}

		sort(xint,cnt);

		int k=0;
		for( k=0;k<cnt-1;k+=2)
		{



			line(xint[k],y,xint[k+1],y);
			delay(100);
		}
	}
}

int main()
{
	polygon p;
	p.read();

	int gd,gm;
	gd=DETECT;
	initgraph(&gd,&gm,NULL);

	p.display();
	p.scanline();
	getch();
	closegraph();

}

/*
Output -->

Enter the total number of vertices in polygon : 3       

Enter vertex no. 1 (eg: x y ) : 300 400

Enter vertex no. 2 (eg: x y ) : 122 432

Enter vertex no. 3 (eg: x y ) : 100 150

*/
