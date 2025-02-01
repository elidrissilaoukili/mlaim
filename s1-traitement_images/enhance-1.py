import cv2
import numpy as np

# Read the input image
image = cv2.imread('lib.jpg') 

# Apply Gaussian Blur (Denoising)
denoised = cv2.GaussianBlur(image, (5, 5), 0)

# Apply Sharpening Filter
kernel = np.array([[0, -1, 0], 
                   [-1, 5, -1], 
                   [0, -1, 0]])
sharpened = cv2.filter2D(image, -1, kernel)

# Convert to Grayscale and Apply Histogram Equalization
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
equalized = cv2.equalizeHist(gray)

# Apply CLAHE (Adaptive Histogram Equalization) for Contrast Enhancement
lab = cv2.cvtColor(image, cv2.COLOR_BGR2LAB)
l, a, b = cv2.split(lab)
clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8, 8))
cl = clahe.apply(l)
enhanced_image = cv2.merge((cl, a, b))
result = cv2.cvtColor(enhanced_image, cv2.COLOR_LAB2BGR)

# Resize the image (replace width and height with actual values)
width, height = 800, 600  # Example dimensions
resized = cv2.resize(image, (width, height))

# Apply Histogram Equalization on HSV Value Channel (Color Balance)
hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
hsv[..., 2] = cv2.equalizeHist(hsv[..., 2])
balanced = cv2.cvtColor(hsv, cv2.COLOR_HSV2BGR)

# Display the Enhanced Image
cv2.imshow('Enhanced Image', result)  # Showing the CLAHE-enhanced image
cv2.waitKey(0)
cv2.destroyAllWindows()

# Save the Enhanced Image
cv2.imwrite('enhanced_output.jpg', result)  # Save the CLAHE result
