import cv2
import numpy as np

temp_name_ME = "start"#just put original name here
name_ME = 'scaled_'+ temp_name_ME +'.png'

ST7789_BLACK = 0x0000
ST7789_RED = 0x001F
ST7789_BLUE = 0xF800
ST7789_GREEN = 0x07E0

def rgb_to_st7789_color(r, g, b):
    # Convert 8-bit RGB to 16-bit color for ST7789
    # 5 bits for red, 6 bits for green, 5 bits for blue
    r = r >> 3  # Reduce 8-bit to 5-bit (divide by 8)
    g = g >> 2  # Reduce 8-bit to 6-bit (divide by 4)
    b = b >> 3  # Reduce 8-bit to 5-bit (divide by 8)
    
    # Combine into 16-bit color (matching your defines)
    # Format: RRRRRGGGGGGBBBBB
    return (r << 11) | (g << 5) | b

def image_to_cpp_bitmap(image_path):
    # Read the image in color
    img = cv2.imread(image_path)
    
    # Ensure the image is 25x25
    if img.shape[:2] != (320, 240):
        raise ValueError(f"Image must be 25x25 pixels. Current shape: {img.shape}")
    
    img = cv2.flip(img, 0)

    # Generate C++ array declaration
    cpp_array = ""
    for y in range(320):
        for x in range(240):
            # Get BGR pixel (OpenCV uses BGR order)
            b, g, r = img[y, x]
            
            # Convert to 16-bit ST7789 color
            color_value = rgb_to_st7789_color(r, g, b)
            #if(color_value >= (0xEF00-1)): #0xEF5D
            #    color_value = int(ST7789_BLUE * 0.1 + ST7789_GREEN * 0.9 + ST7789_RED * 0.1);
            
            # Add to array
            cpp_array += f"{color_value:04X} "
        
        # Add line break every row for readability
        cpp_array += "\n"
    
    cpp_array = cpp_array.rstrip(', ') + ""
    
    return cpp_array

# Example usage
image_path = name_ME  # Replace with your image path
bitmap_data = image_to_cpp_bitmap(image_path)
print(bitmap_data)
with open( temp_name_ME+"_bitmap.txt", "w") as file:
    # Write the string to the file
    file.write(bitmap_data)
