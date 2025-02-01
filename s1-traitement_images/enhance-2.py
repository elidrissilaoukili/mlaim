import cv2
import numpy as np

# Read the input image
image = cv2.imread('lib.jpg')

# 1. Denoising (Reduces Noise)
denoised = cv2.GaussianBlur(image, (5, 5), 0)

# 2. Sharpening (Enhances Edges and Details)
kernel = np.array([[0, -1, 0], 
                   [-1, 5, -1], 
                   [0, -1, 0]])
sharpened = cv2.filter2D(denoised, -1, kernel)

# 3. CLAHE (Contrast Enhancement)
lab = cv2.cvtColor(sharpened, cv2.COLOR_BGR2LAB)  # Convert to LAB color space
l, a, b = cv2.split(lab)  # Split LAB channels
clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8, 8))  # Apply CLAHE
cl = clahe.apply(l)  # Enhance the lightness channel
enhanced_image = cv2.merge((cl, a, b))  # Merge the channels back
final_result = cv2.cvtColor(enhanced_image, cv2.COLOR_LAB2BGR)  # Convert back to BGR

# 4. Optional: Resize the Image (if needed)
width, height = 800, 600  # Define desired dimensions
resized = cv2.resize(final_result, (width, height))

# 5. Display the Enhanced Image
cv2.imshow('Enhanced Image', resized)
cv2.waitKey(0)
cv2.destroyAllWindows()

# 6. Save the Enhanced Image
cv2.imwrite('enhanced_output-2.jpg', resized)
