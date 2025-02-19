# clear_vision
# ClearVision

ClearVision is a C++11 command-line tool for processing grayscale images with various filtering techniques, image arithmetic, and steganography operations.

## Features
- **Filtering:** Apply mean filter, Gaussian smoothing, and unsharp masking.
- **Image Arithmetic:** Add and subtract images.
- **Comparison:** Check if two images are identical.
- **Steganography:** Disguise images, reveal hidden images, and perform LSB-based encryption and decryption.
  
## Usage
Run the `clearvision` executable with the desired operation:
```sh
./clearvision <operation> <args>
```
### Example Commands
- Apply a mean filter:
  ```sh
  ./clearvision mean image.png 3
  ```
- Apply Gaussian smoothing:
  ```sh
  ./clearvision gauss image.png 5 1.2
  ```
- Encrypt a message into an image:
  ```sh
  ./clearvision enc image.png "Hidden message"
  ```
- Decrypt a message from an image:
  ```sh
  ./clearvision dec image.png 14
  ```
