import pygame
import sys
import math

# Initialize pygame
pygame.init()

# Set up display
width, height = 640, 480
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("Circle Drawing")

# Define colors
background_color = (0, 0, 128)  # Similar to setbkcolor(9)
axis_color = (255, 105, 180)    # Similar to setcolor(5)
pixel_color = (255, 105, 180)    # Pixel color for the circles

# Clear screen and draw axis lines
def repere():
    screen.fill(background_color)
    pygame.draw.line(screen, axis_color, (0, height // 2), (width, height // 2))
    pygame.draw.line(screen, axis_color, (width // 2, 0), (width // 2, height))

# Draw a pixel adjusted to the coordinate system
def tracer(xd, yd):
    xd = xd + width // 2
    yd = height // 2 - yd
    if 0 <= xd < width and 0 <= yd < height:  # Ensure it's within bounds
        screen.set_at((xd, yd), pixel_color)

# Draw a circle using the midpoint circle algorithm
def cyrcle(h, k, r):
    x = 0
    y = r
    s = 3 - 2 * r

    while x <= y:
        tracer(x + h, y + k)
        tracer(-x + h, y + k)
        tracer(x + h, -y + k)
        tracer(-x + h, -y + k)
        tracer(y + h, x + k)
        tracer(-y + h, x + k)
        tracer(y + h, -x + k)
        tracer(-y + h, -x + k)
        
        if s > 0:
            s = s + 4 * (x - y) + 10
            y -= 1
        else:
            s = s + 4 * x + 6
        x += 1

def main():
    h1, k1, r1 = 100, 100, 100  # First circle
    h2, k2, r2 = -100, -100, 50 # Second circle
    
    repere()
    cyrcle(h1, k1, r1)
    cyrcle(h2, k2, r2)

    # Update display and keep window open until closed
    pygame.display.flip()
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()

if __name__ == "__main__":
    main()
