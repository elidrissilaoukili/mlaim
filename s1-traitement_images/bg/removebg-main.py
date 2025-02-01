from rembg import remove
from PIL import Image
import io

def remove_background(input_path, output_path):
    with open(input_path, "rb") as file:
        input_image = file.read()
    output_image = remove(input_image)
    with open(output_path, "wb") as file:
        file.write(output_image)

# Example usage
input_image_path = "capture.png"
output_image_path = "output.png"
remove_background(input_image_path, output_image_path)
