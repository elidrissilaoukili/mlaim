import os
from rembg import remove

def remove_background(input_path, output_path):
    with open(input_path, "rb") as file:
        input_image = file.read()
    output_image = remove(input_image)
    with open(output_path, "wb") as file:
        file.write(output_image)

if __name__ == "__main__":
    images_folder = "./images"
    
    if not os.path.exists(images_folder):
        print(f"Error: Folder '{images_folder}' does not exist.")
        exit(1)

    for filename in os.listdir(images_folder):
        input_path = os.path.join(images_folder, filename)
        output_path = os.path.join(images_folder, filename)  # Replace in the same folder with the same name

        # Process only valid image files
        if filename.lower().endswith((".png", ".jpg", ".jpeg")):
            print(f"Processing: {filename}")
            try:
                remove_background(input_path, output_path)
                print(f"Background removed: {filename}")
            except Exception as e:
                print(f"Failed to process {filename}: {e}")
        else:
            print(f"Skipped: {filename} (not an image file)")
