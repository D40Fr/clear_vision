#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>
#include <iostream>

// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    // 1. Copy the original image for reference.
    GrayscaleImage copy_image = image; // copy constructor
    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
    int pixel_value = 0, kernel_index_row = 0, kernel_index_column = 0;
    for(int i = 0; i < copy_image.get_height(); i++) {
        for (int j = 0; j < copy_image.get_width(); j++) {
            
            int kernel_total = 0;
            for(int i_kernel = 0; i_kernel < kernelSize; i_kernel++) {
                for (int j_kernel = 0; j_kernel < kernelSize; j_kernel++) {
                    kernel_index_row = i - (kernelSize/2) + i_kernel;
                    kernel_index_column = j - (kernelSize/2) + j_kernel;
                    
                    if (kernel_index_row < 0 || kernel_index_column < 0 || kernel_index_row >= copy_image.get_height() || kernel_index_column>= copy_image.get_width()) {
                        pixel_value = 0;
                    } else {
                        pixel_value = copy_image.get_pixel(kernel_index_row, kernel_index_column);
                    }
                    
                    kernel_total += pixel_value;
                }
                
            }
            // 3. Update each pixel with the computed mean.
            int avrage = kernel_total / (kernelSize*kernelSize);
            image.set_pixel(i,j, avrage);
        }
        
    }
    
}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    // 1. Create a Gaussian kernel based on the given sigma value.
    int center= kernelSize/2;
    double sum=0;

    std::vector<std::vector<double>> kernel(kernelSize, std::vector<double>(kernelSize));
    for (int i=0; i<kernelSize; i++) {
        for (int j = 0; j<kernelSize; j++) {
            int x = center- j;
            int y = center - i;
            kernel[i][j] = ( (1 / (2.0 * M_PI * sigma*sigma)) * exp(-(x*x + y*y) / (2.0*sigma*sigma)));
            sum += kernel[i][j];
        }
    }
    // 2. Normalize the kernel to ensure it sums to 1.
    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            kernel[i][j] /= sum;
        }
    }
    // 3. For each pixel, compute the weighted sum using the kernel.
    GrayscaleImage copy_image = image;
    int pixel_value = 0, kernel_index_row = 0, kernel_index_column = 0;
    for(int i = 0; i < copy_image.get_height(); i++) {
        for (int j = 0; j < copy_image.get_width(); j++) {
            
            double kernel_total = 0;
            for(int i_kernel = 0; i_kernel < kernelSize; i_kernel++) {
                for (int j_kernel = 0; j_kernel < kernelSize; j_kernel++) {
                    kernel_index_row = i - (kernelSize/2) + i_kernel;
                    kernel_index_column = j - (kernelSize/2) + j_kernel;
                    
                    if (kernel_index_row < 0 || kernel_index_column < 0 || kernel_index_row >= copy_image.get_height() || kernel_index_column>= copy_image.get_width()) {
                        pixel_value = 0;
                    } else {
                        pixel_value = copy_image.get_pixel(kernel_index_row, kernel_index_column);
                    }
                    kernel_total += pixel_value * kernel[i_kernel][j_kernel];
                }
                
            }
            // 4. Update the pixel values with the smoothed results.
            image.set_pixel(i, j, (int)(kernel_total));
        }
        
    }    
}

// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
    GrayscaleImage original_image = image;
    GrayscaleImage blurred_image = image;

    // 1. Blur the image using Gaussian smoothing, use the default sigma given in the header.
    apply_gaussian_smoothing(blurred_image, kernelSize, 1);
    //std::cout<<"Original: "<<original_image.get_pixel(150,150)<<std::endl;
    //std::cout<<"Blurred: "<<blurred_image.get_pixel(150,150)<<std::endl;
    // 2. For each pixel, apply the unsharp mask formula: original + amount * (original - blurred).
    for (int i = 0; i < original_image.get_height(); i++) {
        for (int j = 0; j < original_image.get_width(); j++) {
            int original_pixel = original_image.get_pixel(i, j);
            int blurred_pixel = blurred_image.get_pixel(i, j);
            
            // Calculate the edge image as the difference between the original and blurred pixel
            //int edge_pixel = original_pixel - blurred_pixel;
            
            // Step 4: Enhance the edges by scaling with the 'amount'
            //int sharpened_pixel = original_pixel + amount * edge_pixel;

            int unsharp_pixel = original_pixel + amount * (original_pixel - blurred_pixel);
/*             if(unsharp_pixel != sharpened_pixel) {
                std::cout<<"Bro wtf"<<std::endl; 
                std::cout<<sharpened_pixel<<std::endl;
                std::cout<<unsharp_pixel<<std::endl; 
            }

            // 3. Clip values to ensure they are within a valid range [0-255].
            if (sharpened_pixel < 0) sharpened_pixel = 0;
            if (sharpened_pixel > 255) sharpened_pixel = 255; */

            if (unsharp_pixel < 0) unsharp_pixel = 0;
            if (unsharp_pixel > 255) unsharp_pixel = 255;
            
            // Set the sharpened pixel back in the image
            image.set_pixel(i, j, unsharp_pixel);
        }
        
    }
    
}
