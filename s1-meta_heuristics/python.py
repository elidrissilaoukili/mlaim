import matplotlib.pyplot as plt

def init_graph():
    """Initialize the graphical window."""
    plt.figure(figsize=(8, 6))
    plt.xlim(0, 640)
    plt.ylim(0, 480)
    plt.gca().set_aspect('equal', adjustable='box')

def draw_axes():
    """Draw the X and Y axes."""
    plt.axhline(y=240, color='black', linewidth=1)  # X-axis
    plt.axvline(x=320, color='black', linewidth=1)  # Y-axis

def put_pixel(x, y, color):
    """Plot a pixel at (x, y) with a given color."""
    plt.plot(x, y, marker='o', color=color)

def first_octant(xd, yd, xf, yf):
    """Draw a line in the first octant."""
    dx = xf - xd
    dy = yf - yd
    s = 2 * dy - dx
    x, y = xd, yd

    for i in range(dx + 1):
        put_pixel(x + 320, 240 - y, 'blue')  # Adjusting coordinates
        if s > 0:
            y += 1
            s += 2 * (dy - dx)
        s += 2 * dy
        x += 1

def main():
    # Define starting and ending points
    xd, yd = 100, 100
    xf, yf = 300, 200

    init_graph()    # Initialize graphics
    draw_axes()     # Draw axes
    first_octant(xd, yd, xf, yf)  # Draw line in the first octant

    plt.title('Bresenham\'s Line Drawing Algorithm - First Octant')
    plt.grid(True)
    plt.show()  # Display the graph

if __name__ == "__main__":
    main()
