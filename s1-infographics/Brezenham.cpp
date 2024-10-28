#include <iostream>
#include <graphics.h>
using namespace std;

void drawline(int x1, int y1, int x2, int y2)
{	
	int dx, dy, p, x, y;

	dx = x2 - x1;
	dy = y2 - y1;

	x = x1;
	y = y1;

	p = 2 * dy - dx;

	while (x < x2)
	{
		if (p >= 0)
		{
			putpixel(x, y, 7);
			y = y + 1;
			p = p + 2 * dy - 2 * dx;
		}
		else
		{
			putpixel(x, y, 7);
			p = p + 2 * dy;
		}
		x = x + 1;
		delay(60);
	}
}

int main()
{
	int gd = DETECT, gm, error, x2, y2, x1, y1;
	initgraph(&gd, &gm, NULL);

	cout << "Enter co-ordinates of first point: ";
	cin >> x1 >> y1;

	cout << "Enter co-ordinates of second point: ";
	cin >> x2 >> y2;
	drawline(x1, y1, x2, y2);

	return 0;
}

/*
Output -->
Enter co-ordinates of first point: 100 150
Enter co-ordinates of second point: 200 250
*/