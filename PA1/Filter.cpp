#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>

// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    // TODO: Your code goes here.
    // 1. Copy the original image for reference.
    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
    // 3. Update each pixel with the computed mean.
    GrayscaleImage original_img = image;

    int semiKernel = kernelSize / 2;
    for(int i = 0; i < original_img.get_height(); i++){
        for( int j = 0; j < original_img.get_width(); j++){
            int sum = 0;
            int count = 0;
            for (int m = -semiKernel; m <= semiKernel; m++){
                for(int n = -semiKernel; n <= semiKernel; n++){
                    int neighbor_row = i + m;
                    int neighbor_col = j + n;

                    if (neighbor_row >= 0 && neighbor_row < original_img.get_height() &&
                     neighbor_col >= 0 && neighbor_col < original_img.get_width()){
                        sum += original_img.get_pixel(neighbor_row, neighbor_col);
                    }
                    count++;
                }
            }
            int mean_value = sum / count;
            image.set_pixel(i,j,mean_value);
        }
    }
    

}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    // TODO: Your code goes here.
    // 1. Create a Gaussian kernel based on the given sigma value.
    // 2. Normalize the kernel to ensure it sums to 1.
    // 3. For each pixel, compute the weighted sum using the kernel.
    // 4. Update the pixel values with the smoothed results.

    GrayscaleImage original_img = image;

    std::vector<std::vector<double>> kernel(kernelSize, std::vector<double>(kernelSize));
    int semiKernel = kernelSize / 2;
    double sum = 0.0;

    for(int x = - semiKernel; x <= semiKernel; x++){
        for (int y = -semiKernel; y <= semiKernel; y++){
            double value = (1.0 / (2.0 * M_PI * sigma * sigma)) * 
                exp(-((x * x + y * y) / (2.0 * sigma * sigma)));
            kernel[x + semiKernel][y+semiKernel] = value;
            sum += value;
        }
    }

    for (int x = 0; x < kernelSize; x++){
        for(int y = 0; y < kernelSize; y++){
            kernel[x][y] /= sum;
        }
    }

    for(int i = 0;i < original_img.get_height();i++){
        for (int j =0; j < original_img.get_width();j++){
            double weighted_sum = 0.0;
            for(int m= -semiKernel; m <= semiKernel; m++){
                for(int n= - semiKernel; n <= semiKernel; n++){
                    int neighbor_row = i + m;
                    int neighbor_col = j + n;

                    if (neighbor_row >= 0 && neighbor_row < original_img.get_height() &&
                     neighbor_col >= 0 && neighbor_col < original_img.get_width()){
                        weighted_sum += original_img.get_pixel(neighbor_row, neighbor_col) * kernel[m + semiKernel][n + semiKernel];
                     }

                }
            
            }
            int avg_val = static_cast<int>(weighted_sum);
            avg_val = std::max(0, std::min(255, avg_val));
            image.set_pixel(i,j,avg_val);
        }
    }
}

// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
    // TODO: Your code goes here.
    // 1. Blur the image using Gaussian smoothing, use the default sigma given in the header.
    // 2. For each pixel, apply the unsharp mask formula: original + amount * (original - blurred).
    // 3. Clip values to ensure they are within a valid range [0-255].
    GrayscaleImage blur_img = image;
    apply_gaussian_smoothing(blur_img, kernelSize, 1.0);

    GrayscaleImage result(image.get_width(), image.get_height());

    for (int i = 0; i < image.get_height(); i++){
        for(int j = 0; j< image.get_width(); j++){
            int original_val = image.get_pixel(i,j);
            int blur_val = blur_img.get_pixel(i,j);
            int edge_comp = original_val - blur_val;
            int unsharp_val = static_cast<int>(original_val + amount * edge_comp);

            if (unsharp_val< 0){
                unsharp_val = 0;
            }
            else if (unsharp_val>255){
                unsharp_val = 255;
            }
            image.set_pixel(i, j, unsharp_val);
        }
    }
}
