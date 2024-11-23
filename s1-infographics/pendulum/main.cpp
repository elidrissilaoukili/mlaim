#include "../lib.h"

// void pende(int x, int y, int radius, int direction, int step, int minX, int maxX)
// {
//     while (!kbhit())
//     {
//         cleardevice();

//         // Draw the pendulum
//         line(250, 50, 400, 50);
//         line(320, 50, x, y - radius);
//         circle(x, y, radius);

//         // // Update x position for swinging motion
//         x += direction * step;
//         y = y - 1;
//         // // Reverse direction if hitting the boundaries
//         if (x >= maxX || x <= minX)
//         {
//             y = y + 1;
//             direction *= -1;
//         }

//         delay(50);
//     }
// }

void pende(int x, int y, int radius, int direction, int step, int minX, int maxX)
{
    int originX = 320;
    int originY = 50;
    int length = y - originY;

    while (!kbhit())
    {
        cleardevice();

        line(250, 50, 400, 50);

        // Calculate the new y-coordinate based on the pendulum length
        int deltaX = x - originX;
        y = originY + sqrt(length * length - deltaX * deltaX * 1.01);

        // Draw the pendulum string and bob
        line(originX, originY, x, y - radius);
        circle(x, y, radius);

        x += direction * step;

        if (x >= maxX || x <= minX)
        {
            direction *= -1;
        }

        delay(50);
    }
}

int main()
{
    init_graph();

    pende(320, 300, 20, 1, 5, 200, 450);

    close_graph();
    return 0;
}