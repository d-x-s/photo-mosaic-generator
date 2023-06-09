# Image Mosaic Generator

This program generates an image mosaic by mapping each pixel of a target image to a corresponding thumbnail using nearest neighbor recursion on a KD-Tree. The resulting mosaic is a scaled-up version of the target image, composed of smaller images (thumbnails) that best represent the colors of the original pixels.

## Description

The image mosaic generator program takes a target image and a directory of thumbnail images as input. It uses a KD-Tree data structure to efficiently find the nearest neighbor thumbnail for each pixel in the target image. The KD-Tree is constructed based on the average color of each thumbnail image.

The main steps involved in generating the mosaic are as follows:

1. Read the thumbnail images from the specified directory and create a map of average color to file name.
2. Build the KD-Tree using the created map of thumbnail images.
3. Read the target image, which is a smaller image.
4. For each pixel in the target image, find its nearest neighbor in the KD-Tree using the average color.
5. Retrieve the file name corresponding to the nearest neighbor thumbnail.
6. Use the pixels from the identified thumbnail image to replace the corresponding pixels in the mosaic image.
7. Write the generated mosaic image to a file.

## Requirements

- C++ compiler
- cs221util library (for PNG image manipulation)
- rgbtree.h and tileUtil.h header files

## Usage

1. Clone the repository or download the source code.
2. Compile the program using the C++ compiler. For example: `g++ main.cpp -o mosaic-generator -std=c++11`
3. Create a directory containing the thumbnail images you want to use for the mosaic.
4. Modify the program to specify the path to the thumbnail directory and the target image.
5. Run the program: `./mosaic-generator`
6. The resulting mosaic image will be saved as "mosaic.png" in the "targets" directory.

## Example

Here's an example of how the program can be used:

1. Prepare a directory called "thumbnails" containing a set of thumbnail images.
2. Ensure the thumbnail images are in a format supported by the program (e.g., PNG).
3. Modify the program to specify the path to the "thumbnails" directory and the path to the target image.
4. Compile and run the program.
5. The program will generate a mosaic image based on the target image and save it as "mosaic.png" in the "targets" directory.

## Acknowledgments

The implementation of the KD-Tree data structure is based on the `rgbtree` class provided in the source code.

## License

This project is licensed under the [MIT License](LICENSE).
