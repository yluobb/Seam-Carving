# Exercise of Seam-carving Algorithm

Seam-carving is a content-aware image resizing technique where the image is reduced in size by one pixel of width (or height) at a time. A vertical seam in an image is a path of pixels connected from the top to the bottom with one pixel in each row; a horizontal seam is a path of pixels connected from the left to the right with one pixel in each column. Unlike standard content-agnostic resizing techniques, such as cropping and scaling, seam carving preserves the most interest features (such as aspect ratio, set of objects present, etc.) of the image. 
This project implements the seam-carving algorithm, 

This project is a fast implementation of the seam-carving technique which resizes a H-by-W image, the algorithms is as follows:

1. Compute the dual-gradient energy function, which calculates the energy - the perceptual importance - of each pixel of the image. The energy is high for pixels in the image where there is a rapid colour change (such as the boundary between a human face and the background). The seam-carving technique avoids removing such high-energy pixels.
    
    The energy of pixel (y,x) is 
    $\sqrt{\Delta_x^2(y,x) + \Delta_y^2(y,x)}$. 
    Here, 
    
    $\Delta_x^2(y,x) = R_x (y,x)^2 + G_x (y,x)^2 + B_x (y,x)^2 $     
    $\Delta_y^2(y,x) = R_y (y,x)^2 + G_y (y,x)^2 + B_y (y,x)^2 $
    
    $R_x, R_y, ..., B_Y$ are the differences in the red, green, and blue components of pixels surrounding the central pixels, along the x and y-axis. For example, $R_x (y,x) = (y,x+1)(red) - (y,x-1)(red) $

Note: In image processing, pixel (y,x) refers to the pixel in column x and row y, with pixel (0,0) at the upper-left corner and pixel (H-1,W-1) at the lower-right corner.


2. Find the vertical 'seams' such that the sum of the dual-gradient energy values in the pixels along the path is as small as possible, this is the seam that will be removed. This will be identified using dynamic programming.

3. Remove the pixels along the vertical "seam" from the image and produce a new, reduced image.


## Directory Structure

- `main.c`: uses the functions declared in h and c file to implement the complete autocomplete function,and can be modified to test the c file.

- `c_img.c`:
    - `create_img`: create anl, image with a structure 'rgb_img', specified height, width, and pixel values.
    - `read_2byte`: reads and returns the bytes of FILE *fp 
    - `write_2byte`: writes the bytes to FILE *fp
    - `read_in_png`: read in an image file and create a structure 'rgb_img' with the height, width, and pixel values form the file
    - `write_img`: write to a 'rbg_img' image
    - `get_pixel`: returns the pixel values of an image
    - `set_pixel`: writes to the pixel values of an image
    - `destroy_image`: frees up the memory allocated for an image
    - `print_grad`: prints the dual-gradient energy 

- `c_img.h`: declares C functions written in `c_img.c` and macro definitions, which are to be shared between several source files
    - `rgb_img`: specifies the structure of 'rgb_img' for each image to be reduced

- `seamcarving.c`:
    - `calc_energy`: compute the dual-gradient energy function, and place it in the `struct rgb_img *grad`.
    - `dynamic_seam`: allocates and computes the dunamic array *best_arr. `(*best_arr)[i*width+j]` contains the minimum cost of a seam from the top of grad to the point (i,j).
    - `recover_path`: allocates a path through the minimum seam as defined by the array best.
    - `remove_seam`: creates the destination image, and writes to it the source image, with the seam removed.

- `seamcarving.h`: declares C functions written in `seamcarving.c` and macro definitions, which are to be shared between several source files

- `png2bin`: changes a png file to a bin file for easier processing. Lines 38-52 illustrated example png-to-bin or bin-to-png commands.

- `3x4.bin`, `3x4.png`, `6x5.bin`, `6x5.png`, `HJoceanSmall.bin`, `HJoceanSmall.png`: example bin and png files for testing.



## Compilation Intruction

Use the command to compile: 'gcc -Wall seamcarving.c c_img.c main.c'. Note that the h files must also be placed in the same folder as the C files. 


## Testing Example

The current 'main.c' file uses `6x5.bin` to test the seamcarving algorithm. In line 12 the `6x5.bin` image file was read by the program and the image is reduced for 3 rounds from lines 14-30. In each round, the dual-gradiet is found and stored in a new image, after the seam is found and removed from the original image, a new image of reduced height and width is reduced and the new pixel values are wrote to the new image. In line 22, a new image file is produced with an reduced image file, of a name `imgi.bin`, where 'i' indicates how many rounds of reduction the image has gone through. In line 29, the old image is replaced by the new one. 

Those lines can be modified to test on different bin-files. To use the algorithm on png-files, don't forget to convert png-files to bin-files first using `png2bin`. The image files being read by the program must be placed in the same folder as the C files. 


## Credit

The project was originally designed by Josh Hug, which was ported to C and used as assignments in University of Toronto Engineering course ESC190 by Michael Guerzhoy, implemented by Bonnie Luo and Sonia Zeng in 2021 Winter.

