#include "Crypto.h"
#include "GrayscaleImage.h"


// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage& secret_image, int message_length) {
    std::vector<int> LSB_array;

    // 1. Reconstruct the SecretImage to a GrayscaleImage.
    GrayscaleImage reconstructed_image = secret_image.reconstruct();
    
    // 2. Calculate the image dimensions.
    int height = reconstructed_image.get_height();
    int width = reconstructed_image.get_width();
    
    // 3. Determine the total bits required based on message length.
    int bit_lenght = message_length * 7;
    
    // 4. Ensure the image has enough pixels; if not, throw an error.
    int total_pixel = height * width;
    if( total_pixel < bit_lenght) {
        throw std::runtime_error("Not enough pixels.");
    }
    
    // 5. Calculate the starting pixel from the message_length knowing that  
    //    the last LSB to extract is in the last pixel of the image.
    int starting_x = (total_pixel - bit_lenght) % width;
    int starting_y = (total_pixel - bit_lenght) / width;
    
    // 6. Extract LSBs from the image pixels and return the result.
    for (int i = 0; i < bit_lenght; i++) {
        int pixel_value = reconstructed_image.get_pixel(starting_y, starting_x);
        int lsb = pixel_value % 2; // get lsb
        LSB_array.push_back(lsb);

        starting_x++;
        if (starting_x == width) { // row ended, go to next column
            starting_x = 0;
            starting_y++;
        }
    }

    return LSB_array;
}


// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int>& LSB_array) {
    std::string message;
    // 1. Verify that the LSB array size is a multiple of 7, else throw an error.
    if(LSB_array.size() % 7 != 0) {
        throw std::runtime_error("LSB array is not multiply of 7");
    }

    // 2. Convert each group of 7 bits into an ASCII character.
    for (int i = 0; i < LSB_array.size()/7; i++) { //move char by char
        std::bitset<7> binary_char;
        for (int j = 0; j < 7; j++) { 
            binary_char[j] = LSB_array[i*7 + (6-j)];
        }
        
        // 3. Collect the characters to form the decrypted message.
        char ascii = (char)(binary_char.to_ulong());
        message += ascii;
    }

    // 4. Return the resulting message.
    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> LSB_array;
    // 1. Convert each character of the message into a 7-bit binary representation.
    //    You can use std::bitset.
    // 2. Collect the bits into the LSB array.
    // 3. Return the array of bits.
    int size_of_message = message.size();
    for (int i = 0; i < size_of_message; i++) {
        std::bitset<7> binary_char(message[i]);
        for (int j = 0; j < 7; j++) {
            LSB_array.push_back(binary_char[6-j]);
        }
    }
    return LSB_array;
}

// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& LSB_array) {
    
    GrayscaleImage copy_image = image;
    // 1. Ensure the image has enough pixels to store the LSB array, else throw an error.
    int height = copy_image.get_height();
    int width = copy_image.get_width();
    int total_pixel = height * width;
    if( total_pixel < LSB_array.size()) {
        throw std::runtime_error("Not enough pixels.");
    }
    // 2. Find the starting pixel based on the message length knowing that  
    //    the last LSB to embed should end up in the last pixel of the image.
    int start_pixel = total_pixel - LSB_array.size();
    int index = 0;
    int starting_x = start_pixel % width;
    int starting_y = start_pixel / width;
    // 3. Iterate over the image pixels, embedding LSBs from the array.
    for (int i = starting_y; i < height; i++) {
        for (int j = (i== starting_y ? starting_x: 0); j < width; j++) {
                int pixel_value = copy_image.get_pixel(i, j);
                int lsb = pixel_value % 2; // get lsb
                pixel_value -= lsb; // make last bit 0
                pixel_value += LSB_array[index]; // make last bit lsb
                copy_image.set_pixel(i, j, pixel_value);
                index++;
        }
    }
    // 4. Return a SecretImage object constructed from the given GrayscaleImage 
    //    with the embedded message.
    SecretImage secret_image(copy_image);
    return secret_image;
}
