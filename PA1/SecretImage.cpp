#include "SecretImage.h"
#include <fstream>
#include <sstream>


// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {
    // TODO: Your code goes here.
    // 1. Dynamically allocate the memory for the upper and lower triangular matrices.
    // 2. Fill both matrices with the pixels from the GrayscaleImage.
    width = image.get_width();
    height = image.get_height();

    int upper_size = (width*(width+1))/ 2;
    int lower_size = (width*(width-1))/ 2;

    upper_triangular = new int[upper_size];
    lower_triangular = new int[lower_size];

    int upper_ind = 0;
    int lower_ind = 0;

    for(int i =0; i < height; ++i){
        for (int j =0; j< width; ++j){
            if (j >= i){
                upper_triangular[upper_ind++] = image.get_pixel(i,j);
            }
            else{
                lower_triangular[lower_ind++] = image.get_pixel(i,j);
            }
        }
    }
}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int * upper, int * lower) {
    // TODO: Your code goes here.
    // Since file reading part should dynamically allocate upper and lower matrices.
    // You should simply copy the parameters to instance variables.
    width = w;
    height = h;

    int upper_size = (width*(width+1))/ 2;
    upper_triangular = upper;
    for (int i = 0; i < upper_size; ++i){
        upper_triangular[i] = upper[i];
    }

    int lower_size = (width*(width-1))/ 2;
    lower_triangular = lower;
    for (int i = 0; i < lower_size; ++i){
        lower_triangular[i] = lower[i];
    }

}


// Destructor: free the arrays
SecretImage::~SecretImage() {
    // TODO: Your code goes here.
    // Simply free the dynamically allocated memory
    // for the upper and lower triangular matrices.
    if (upper_triangular != nullptr) {
            delete[] upper_triangular;
            upper_triangular = nullptr; 
        }
    if (lower_triangular != nullptr) {
            delete[] lower_triangular;
            lower_triangular = nullptr; 
        }

}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);

    // TODO: Your code goes here.
    // 1. Fill the image with the pixels from the upper and lower triangular matrices.
    // 2. Return the GrayscaleImage object.
    int index = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = i; j < width; ++j) {
            image.set_pixel(i, j, upper_triangular[index]);
            ++index;
        }
    }
    index = 0;
    for(int i =1; i< height; ++i){
        for(int j =0; j<i; ++j){
            image.set_pixel(i,j, lower_triangular[index]);
            ++index;
        }
    }

    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {
    // TODO: Your code goes here.
    // Update the lower and upper triangular matrices 
    // based on the GrayscaleImage given as the parameter.
    int idx_upp = 0;
    int idx_low = 0;

    int width = image.get_width();
    int height = image.get_height();

    for(int i = 0;i < height;++i){
        for(int j = 0; j < width; ++j){
            if (j >= i){
                upper_triangular[idx_upp++] = image.get_pixel(i,j);
            }
            else{
                lower_triangular[idx_low++] = image.get_pixel(i,j);
            }
        }
    }


}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {
    // TODO: Your code goes here.
    // 1. Write width and height on the first line, separated by a single space.
    // 2. Write the upper_triangular array to the second line.
    // Ensure that the elements are space-separated. 
    // If there are 15 elements, write thm as: "element1 element2 ... element15"
    // 3. Write the lower_triangular array to the third line in a similar manner
    // as the second line.

    std::ofstream file(filename);

    if (!file.is_open()){
        std::cerr << "error opening file 1"<<std::endl;
        exit(1);
    }

    file << width << " " << height << std::endl;

    for(int i= 0; i< (width*(width+1))/ 2; ++i){
        file << upper_triangular[i];
        if (i != (width*(width+1))/ 2 -1){
            file << " ";
        }
    }
    file << std::endl;

    for(int i=0; i< (width*(width-1))/ 2; ++i){
        file << lower_triangular[i];
        if (i != (width*(width-1))/ 2 -1){
            file << " ";
        }
    }
    file << std::endl;

    file.close();
}

// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string& filename) {
    // TODO: Your code goes here.
    // 1. Open the file and read width and height from the first line, separated by a space.
    // 2. Calculate the sizes of the upper and lower triangular arrays.
    // 3. Allocate memory for both arrays.
    // 4. Read the upper_triangular array from the second line, space-separated.
    // 5. Read the lower_triangular array from the third line, space-separated.
    // 6. Close the file and return a SecretImage object initialized with the
    //    width, height, and triangular arrays.

    std::ifstream file(filename);
    if (!file.is_open()){
        std::cerr<<"error opening file 2 "<<std::endl;
        exit(1);
    }

    int width, height;
    file >> width >> height;

    int upper_size = (width*(width+1))/ 2;
    int lower_size = (width*(width-1))/ 2;

    int* upper_triangular = new int[upper_size];
    int* lower_triangular = new int[lower_size];

    for(int i = 0;i <upper_size; ++i){
        file >> upper_triangular[i];
    }
    for(int i = 0; i < lower_size; ++i){
        file >> lower_triangular[i];
    }

    file.close();
    
    return SecretImage(width, height, upper_triangular, lower_triangular);
}

// Returns a pointer to the upper triangular part of the secret image.
int * SecretImage::get_upper_triangular() const {
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int * SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const {
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const {
    return height;
}
