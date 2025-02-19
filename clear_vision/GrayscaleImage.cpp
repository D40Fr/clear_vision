#include "GrayscaleImage.h"
#include <iostream>
#include <cstring>  // For memcpy
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdexcept>


// Constructor: load from a file
GrayscaleImage::GrayscaleImage(const char* filename) {

    // Image loading code using stbi
    int channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, STBI_grey);

    if (image == nullptr) {
        std::cerr << "Error: Could not load image " << filename << std::endl;
        exit(1);
    }

    // Dynamically allocate memory for a 2D matrix based on the given dimensions.
    // Fill the matrix with pixel values from the image
    data = new int* [height];
    for (int i = 0; i < height; i++) {
        data[i] = new int[width];
        for (int j = 0; j < width; j++) {
            data[i][j] = image[i * width + j];  // 1D to 2D using row major
        }
    }

    // Free the dynamically allocated memory of stbi image
    stbi_image_free(image);
}

// Constructor: initialize from a pre-existing data matrix
GrayscaleImage::GrayscaleImage(int** inputData, int h, int w) {
    // Don't forget to dynamically allocate memory for the matrix.
    data = new int*[h];
    for (int i = 0; i < h; i++) {
        data[i] = new int[w];
    }
    // Initialize the image with a pre-existing data matrix by copying the values.
    height = h;
    width = w;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            data[i][j] = inputData[i][j];
        }
    }
}

// Constructor to create a blank image of given width and height
GrayscaleImage::GrayscaleImage(int w, int h) : width(w), height(h) {
    // Just dynamically allocate the memory for the new matrix.
    data = new int* [height];
    for (int i = 0; i < height; i++) {
        data[i] = new int[width];
    }
}

// Copy constructor
GrayscaleImage::GrayscaleImage(const GrayscaleImage& other) {
    
    // Copy constructor: dynamically allocate memory and 
    width = other.width;
    height = other.height;
    data = new int*[height];
    for (int i = 0; i < height; i++) {
        data[i] = new int[width];
    }
    // copy pixel values from another image.
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            data[i][j] = other.data[i][j];
        }
    }
}

// Destructor
GrayscaleImage::~GrayscaleImage() {
    // Destructor: deallocate memory for the matrix.
    for(int i = 0; i < height; i++) {
        delete[] data[i];
    }
    delete[] data;
}

// Equality operator
bool GrayscaleImage::operator==(const GrayscaleImage& other) const {
    // Check if two images have the same dimensions and pixel values.
    // If they do, return true.
    if ( (get_height() != other.get_height()) || (get_width() != other.get_width())) {
        return false;
    }
    for (int i = 0; i < get_height(); i++) {
        for (int j = 0; j < get_width(); j++) {
            if (data[i][j] != other.get_pixel(i,j))
            {
                return false;
            }
        }
    }
    return true;
}

// Addition operator
GrayscaleImage GrayscaleImage::operator+(const GrayscaleImage& other) const {
    // Create a new image for the result
    GrayscaleImage result(width, height);
    
    // Add two images' pixel values and return a new image, clamping the results.
    for (int i = 0; i < get_height(); i++) {
        for (int j = 0; j < get_width(); j++) {
            result.data[i][j] = data[i][j] + other.get_pixel(i,j);
            if(result.data[i][j] < 0) {
                result.data[i][j] = 0; 
            }
            else if (result.data[i][j] > 255) {
                result.data[i][j] = 255;
            }
        }
    }
    return result;
}

// Subtraction operator
GrayscaleImage GrayscaleImage::operator-(const GrayscaleImage& other) const {
    // Create a new image for the result
    GrayscaleImage result(width, height);
    
    // Subtract pixel values of two images and return a new image, clamping the results.
    for (int i = 0; i < get_height(); i++) {
        for (int j = 0; j < get_width(); j++) {
            result.data[i][j] = data[i][j] - other.get_pixel(i,j);
            if(result.data[i][j] < 0) {
                result.data[i][j] = 0; 
            }
            else if (result.data[i][j] > 255) {
                result.data[i][j] = 255;
            }
        }
    }

    return result;
}

// Get a specific pixel value
int GrayscaleImage::get_pixel(int row, int col) const {
    return data[row][col];
}

// Set a specific pixel value
void GrayscaleImage::set_pixel(int row, int col, int value) {
    data[row][col] = value;
}

// Function to save the image to a PNG file
void GrayscaleImage::save_to_file(const char* filename) const {
    // Create a buffer to hold the image data in the format stb_image_write expects
    unsigned char* imageBuffer = new unsigned char[width * height];

    // Fill the buffer with pixel data (convert int to unsigned char)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            imageBuffer[i * width + j] = static_cast<unsigned char>(data[i][j]);
        }
    }

    // Write the buffer to a PNG file
    if (!stbi_write_png(filename, width, height, 1, imageBuffer, width)) {
        std::cerr << "Error: Could not save image to file " << filename << std::endl;
    }

    // Clean up the allocated buffer
    delete[] imageBuffer;
}
