import sys
from rembg import remove

def remove_background(input_path, output_path):
    with open(input_path, "rb") as file:
        input_image = file.read()
    output_image = remove(input_image)
    with open(output_path, "wb") as file:
        file.write(output_image)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python removebg.py <input_path> <output_path>")
        sys.exit(1)

    input_path = sys.argv[1]
    output_path = sys.argv[2]
    remove_background(input_path, output_path)

