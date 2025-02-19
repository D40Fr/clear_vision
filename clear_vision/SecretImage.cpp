#include "SecretImage.h"

// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {
    GrayscaleImage copy_image = image;
    // 1. Dynamically allocate the memory for the upper and lower triangular matrices.
    height = copy_image.get_height();
    width = copy_image.get_width();
    int size_of_upper = height* (height+1) / 2; // +1 incuding diagonal
    int size_of_lower = height * (height-1) / 2;

    upper_triangular = new int[size_of_upper];
    lower_triangular = new int[size_of_lower];
    int up_counter = 0;
    int low_counter = 0;

    int size = height*width;
    // 2. Fill both matrices with the pixels from the GrayscaleImage.
    for (int i = 0; i < height; i++) {
        for (int j=0; j < width; j++) {
            if (j >= i){
                upper_triangular[up_counter++] = copy_image.get_pixel(i,j);
            }
            else {
                lower_triangular[low_counter++] = copy_image.get_pixel(i,j);
            }
        }
        
    }
    

}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int * upper, int * lower) {
    // Since file reading part should dynamically allocate upper and lower matrices.
    // You should simply copy the parameters to instance variables.
    width = w;
    height = h;
    upper_triangular = upper;
    lower_triangular = lower;

}


// Destructor: free the arrays
SecretImage::~SecretImage() {
    // Simply free the dynamically allocated memory
    // for the upper and lower triangular matrices.
    delete[] upper_triangular;
    delete[] lower_triangular;

}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);

    int up_counter = 0;
    int low_counter = 0;
    int size = image.get_height() * image.get_width();

    for (int i = 0; i < height; i++) {
        for (int j=0; j < width; j++) {
            if (j >= i){
                image.set_pixel(i,j,upper_triangular[up_counter++]);
            }
            else {
                image.set_pixel(i,j,lower_triangular[low_counter++]);
            }
        }
        
    }
    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {
    // Update the lower and upper triangular matrices 
    // based on the GrayscaleImage given as the parameter.

    int up_counter = 0;
    int low_counter = 0;


    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j >= i) {
                upper_triangular[up_counter++] = image.get_pixel(i, j);
            }
            else {
                lower_triangular[low_counter++] = image.get_pixel(i, j);
            }
        }
    }
}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {
    
    std::ofstream my_file(filename);
    
    if (my_file.is_open() == false) {
        throw std::runtime_error("Can't open the file for writing.");
    }
    int size_of_upper = height* (height+1) / 2;
    int size_of_lower = height * (height-1) / 2;

    // 1. Write width and height on the first line, separated by a single space.
    my_file << width << " " << height << "\n";

    // 2. Write the upper_triangular array to the second line.
    for (int i = 0; i < size_of_upper; i++) {
        my_file << upper_triangular[i];
        if (i != size_of_upper - 1) { // if for dont print space at the end
            // Ensure that the elements are space-separated. 
            // If there are 15 elements, write them as: "element1 element2 ... element15"
            my_file<< " "; 
        }
    }
    my_file<< "\n";

    // 3. Write the lower_triangular array to the third line in a similar manner
    // as the second line.
    for (int i = 0; i < size_of_lower; i++) {
        my_file << lower_triangular[i];
        if (i != size_of_lower - 1) {
            // Ensure that the elements are space-separated. 
            // If there are 15 elements, write them as: "element1 element2 ... element15"
            my_file << " ";
        }
    }
    my_file << "\n";
    my_file.close();
}

// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string& filename) {
    // 1. Open the file and read width and height from the first line, separated by a space.
    std::ifstream my_file(filename);
    if (my_file.is_open() == false) {
        throw std::runtime_error("Can't open the file for reading.");
    }
    int w,h;
    std::string line;
    std::getline(my_file, line);
    std::istringstream line1(line);
    line1 >> w >> h;

    // 2. Calculate the sizes of the upper and lower triangular arrays.
    int size_of_upper = h* (h+1) / 2;
    int size_of_lower = h * (h-1) / 2;

    // 3. Allocate memory for both arrays.
    int* upper = new int[size_of_upper];
    int* lower = new int[size_of_lower];

    // 4. Read the upper_triangular array from the second line, space-separated.
    std::getline(my_file, line);
    std::istringstream line2(line);
    for (int i = 0; i < size_of_upper; i++) {
        line2 >> upper[i];
    }
    
    // 5. Read the lower_triangular array from the third line, space-separated.
    std::getline(my_file, line);
    std::istringstream line3(line);
    for (int i = 0; i < size_of_lower; i++) {
        line3 >> lower[i];
    }

    // 6. Close the file and return a SecretImage object initialized with the
    //    width, height, and triangular arrays.
    my_file.close();
    SecretImage secret_image(w, h, upper, lower);
    return secret_image;
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
