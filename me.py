import cv2
import numpy as np

name_ME = "start.png"

# Method 1: Basic image resize
def resize_image_basic(image_path, width=240, height=320):
    # Read the image
    img = cv2.imread(image_path)
    
    # Check if image is loaded successfully
    if img is None:
        print(f"Error: Could not read the image at {image_path}")
        return None
    
    # Resize the image to specific dimensions
    resized_img = cv2.resize(img, (width, height))
    
    return resized_img

# Method 2: Resize with aspect ratio preservation
def resize_image_aspect_ratio(image_path, scale_percent=50):
    # Read the image
    img = cv2.imread(image_path)
    
    # Check if image is loaded successfully
    if img is None:
        print(f"Error: Could not read the image at {image_path}")
        return None
    
    # Calculate the new dimensions
    width = int(img.shape[1] * scale_percent / 100)
    height = int(img.shape[0] * scale_percent / 100)
    
    # Resize the image
    resized_img = cv2.resize(img, (width, height), interpolation=cv2.INTER_AREA)
    
    return resized_img

# Method 3: Resize to fit within a max dimension while maintaining aspect ratio
def resize_image_max_dimension(image_path, max_dimension=800):
    # Read the image
    img = cv2.imread(image_path)
    
    # Check if image is loaded successfully
    if img is None:
        print(f"Error: Could not read the image at {image_path}")
        return None
    
    # Get original image dimensions
    height, width = img.shape[:2]
    
    # Calculate scaling factor
    if height > width:
        scale = max_dimension / height
    else:
        scale = max_dimension / width
    
    # Calculate new dimensions
    new_width = int(width * scale)
    new_height = int(height * scale)
    
    # Resize the image
    resized_img = cv2.resize(img, (new_width, new_height), interpolation=cv2.INTER_AREA)
    
    return resized_img

# Example usage
def main():
    # Replace with your image path
    image_path = name_ME
    
    # Method 1: Fixed size resize
    resized_img1 = resize_image_basic(image_path)
    if resized_img1 is not None:
        cv2.imshow('Resized Image (Fixed Size)', resized_img1)
        cv2.imwrite('scaled_'+name_ME, resized_img1)
    
    # Method 2: Percentage resize
    resized_img2 = resize_image_aspect_ratio(image_path, scale_percent=75)
    if resized_img2 is not None:
        cv2.imshow('Resized Image (Percentage)', resized_img2)
    
    # Method 3: Max dimension resize
    resized_img3 = resize_image_max_dimension(image_path)
    if resized_img3 is not None:
        cv2.imshow('Resized Image (Max Dimension)', resized_img3)
    
    # Wait for a key press and then close all windows
    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
