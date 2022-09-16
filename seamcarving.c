#include <math.h>
#include <stdio.h>

#include "seamcarving.h"
#include "c_img.h"

void calc_energy(struct rgb_img *im, struct rgb_img **grad){
    int h = im->height;
    int w = im->width;

    create_img(grad, h, w);

    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            double sq_res = 0;

            // upperleft corner
            if(x == 0 && y == 0){  
                // calculate delta x
                for(int k = 0; k < 3; k++)  
                    sq_res += pow((double)(get_pixel(im, y, x+1, k)) - (double)(get_pixel(im, y, w-1, k)), (double)2);
                
                 // calculate delta y
                for(int k = 0; k < 3; k++)  
                    sq_res += pow((double)(get_pixel(im, y+1, x, k)) - (double)(get_pixel(im, h-1, x, k)), (double)2);
            }

            // upperight corner
            else if(x == w-1 && y == 0){
                // calculate delta x
                for(int k = 0; k < 3; k++)  
                    sq_res += pow((double)(get_pixel(im, y, 0, k)) - (double)(get_pixel(im, y, x-1, k)), (double)2);
                
                 // calculate delta y
                for(int k = 0; k < 3; k++)  
                    sq_res += pow((double)(get_pixel(im, y+1, x, k)) - (double)(get_pixel(im, h-1, x, k)), (double)2);
            }

            // lowerleft corner
            else if(x == 0 && y == h-1){
                // calculate delta x
                for(int k = 0; k < 3; k++)  
                    sq_res += pow((double)(get_pixel(im, y, x+1, k)) - (double)(get_pixel(im, y, w-1, k)), (double)2);
                
                 // calculate delta y
                for(int k = 0; k < 3; k++)  
                    sq_res += pow((double)(get_pixel(im, 0, x, k)) - (double)(get_pixel(im, y-1, x, k)), (double)2);
            }

            // loweright corner
            else if(x == w-1 && y == h-1){
                // calculate delta x
                for(int k = 0; k < 3; k++)  
                    sq_res += pow((double)(get_pixel(im, y, 0, k)) - (double)(get_pixel(im, y, x-1, k)), (double)2);
                
                 // calculate delta y
                for(int k = 0; k < 3; k++)  
                    sq_res += pow((double)(get_pixel(im, 0, x, k)) - (double)(get_pixel(im, y-1, x, k)), (double)2);
            }

            // left vertical edge
            else if(x == 0){
                // calculate delta x
                for(int k = 0; k < 3; k++)  
                    sq_res += pow((double)(get_pixel(im, y, x+1, k)) - (double)(get_pixel(im, y, w-1, k)), (double)2);
                
                 // calculate delta y
                for(int k = 0; k < 3; k++)  
                    sq_res += pow((double)(get_pixel(im, y+1, x, k)) - (double)(get_pixel(im, y-1, x, k)), (double)2);
            }

            // right vertical edge
            else if(x == w-1){
                // calculate delta x
                for(int k = 0; k < 3; k++)  
                    sq_res += pow((double)(get_pixel(im, y, 0, k)) - (double)(get_pixel(im, y, x-1, k)), (double)2);
                
                 // calculate delta y
                for(int k = 0; k < 3; k++)  
                    sq_res += pow((double)(get_pixel(im, y+1, x, k)) - (double)(get_pixel(im, y-1, x, k)), (double)2);
            }

            // upper hori edge
            else if(y == 0){
                // calculate delta x
                for(int k = 0; k < 3; k++)  
                    sq_res += pow((double)(get_pixel(im, y, x+1, k)) - (double)(get_pixel(im, y, x-1, k)), (double)2);
                
                 // calculate delta y
                for(int k = 0; k < 3; k++)  
                    sq_res += pow((double)(get_pixel(im, y+1, x, k)) - (double)(get_pixel(im, h-1, x, k)), (double)2);
            }

            // lower hori edge
            else if(y == h-1){
                // calculate delta x
                for(int k = 0; k < 3; k++)  
                    sq_res += pow((double)(get_pixel(im, y, x+1, k)) - (double)(get_pixel(im, y, x-1, k)), (double)2);
                
                 // calculate delta y
                for(int k = 0; k < 3; k++)  
                    sq_res += pow((double)(get_pixel(im, 0, x, k)) - (double)(get_pixel(im, y-1, x, k)), (double)2);
            }

            // middle area
            else{
                // calculate delta x
                for(int k = 0; k < 3; k++)  
                    sq_res += pow((double)(get_pixel(im, y, x+1, k)) - (double)(get_pixel(im, y, x-1, k)), (double)2);
                
                // calculate delta y
                for(int k = 0; k < 3; k++)  
                    sq_res += pow((double)(get_pixel(im, y+1, x, k)) - (double)(get_pixel(im, y-1, x, k)), (double)2);
            }

            set_pixel(*grad, y, x, (uint8_t)(sqrt(sq_res)/10), (uint8_t)(sqrt(sq_res)/10), (uint8_t)(sqrt(sq_res)/10));
            
            // printf("%d\t", (int)(sqrt(sq_res)/10));  //CHANGE
        }
        // printf("\n");  //CHANGE
    }
}

double min(double num1, double num2){
    return (num1 < num2) ? num1 : num2;
}

void dynamic_seam(struct rgb_img *grad, double **best_arr){
    int h = grad->height;
    int w = grad->width;

    *best_arr = (double *)malloc(sizeof(double) * (h * w));

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {

            if (y == 0) { // if it is on the first row
                (*best_arr)[y * w + x] = get_pixel(grad, y, x, 0);
            }

            else if (x == 0) { // if it is on the first column
                (*best_arr)[y * w + x] = get_pixel(grad, y, x, 0) + min((*best_arr)[(y-1) * w + x], (*best_arr)[(y-1) * w + x + 1]);  //compare top with top right + add
            }

            else if (x == w - 1) {
                (*best_arr)[y * w + x] = get_pixel(grad, y, x, 0) + min((*best_arr)[(y-1) * w + x], (*best_arr)[(y-1) * w + x - 1]);  //compare top with top left + add
            }

            else {
                (*best_arr)[y * w + x] = min((*best_arr)[(y-1) * w + x - 1], (*best_arr)[(y-1) * w + x]); //compare top left with top first
                (*best_arr)[y * w + x] = get_pixel(grad, y, x, 0) + min((*best_arr)[y * w + x], (*best_arr)[(y-1) * w + x + 1]); //compare the smaller one with top right + add
            }
        }
    }
}

void recover_path(double *best, int height, int width, int **path){
    *path = (int *)malloc(sizeof(int) * height);

    int i = height - 1;      //last row
    int j;   // 2nd column of the last row
    double last_min = (best)[i * width];
    int last_min_posi = 0;

    //find the starting point of the last row
    for (j = 1; j < width; j++){
        if ((best[i * width + j]) < last_min) {
            last_min = (best)[i * width + j];
            last_min_posi = j;
        }
    } (*path)[i] = last_min_posi;  

    int starting_posi = last_min_posi;
    for (int k = height - 2; k > -1; k--){  // now loop from the secoond last row to the 1st row
        int local_min = best[k * width + starting_posi];  // initialize the comparison from the original position

        // compare the left with right, whichever one is smaller is the smaller_one
        int smaller_one = (best[k * width + starting_posi - 1] < best[k * width + starting_posi + 1]) ? best[k * width + starting_posi - 1] : best[k * width + starting_posi + 1];
        int new_posi = (best[k * width + starting_posi - 1] < best[k * width + starting_posi + 1]) ? (starting_posi - 1) : (starting_posi + 1); //update the position

        // compare the center with the smaller_one
        // local_min is the value of the center position, not the real min
        starting_posi = (smaller_one < local_min) ? new_posi : starting_posi;

        (*path)[k] = starting_posi;
    }
}


void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path){
    // The function creates the destination image, and writes to it the source image, with the seam removed.

    create_img(&(*dest), src->height, (src->width)-1);

    for (int y = 0; y < src->height; y++){
        for (int x = 0; x < path[y]; x++){
            int r = get_pixel(src, y, x, 0);
            int g = get_pixel(src, y, x, 1);
            int b = get_pixel(src, y, x, 2);

            set_pixel(*dest, y, x, r, g, b);
        }
        
        for (int x = path[y]+1; x < src->width; x++){
            int r = get_pixel(src, y, x, 0);
            int g = get_pixel(src, y, x, 1);
            int b = get_pixel(src, y, x, 2);

            set_pixel(*dest, y, x-1, r, g, b);
        }
    }
}