#include "c_img.h"
#include "seamcarving.h"
#include <stdio.h>

int main(void){
    struct rgb_img *im;
    struct rgb_img *cur_im;
    struct rgb_img *grad;
    double *best;
    int *path;

    read_in_img(&im, "6x5.bin");

    for(int i = 0; i < 3; i++){
        printf("i = %d\n", i);
        calc_energy(im,  &grad);
        dynamic_seam(grad, &best);
        recover_path(best, grad->height, grad->width, &path);
        remove_seam(im, &cur_im, path);

        char filename[200];
        sprintf(filename, "img%d.bin", i);

        write_img(cur_im, filename);
        destroy_image(im);
        destroy_image(grad);
        free(best);
        free(path);
        im = cur_im; 
    }
    
    destroy_image(im);

    return 0;
}