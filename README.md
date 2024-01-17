# Simple BMP Image Processing Command-line Tool

This project is a C++ application designed for basic image processing. It includes functionalities such as flipping, inverting colors, applying Gaussian blur, converting to grayscale, and edge detection on bitmap images.

## Features

- **Flip Vertically (-fv) and Horizontally (-fh)**: Flip images vertically or horizontally.
- **Invert Colors (-i)**: Invert the colors of the image.
- **Gaussian Blur (-g)**: Apply Gaussian blur with specified kernel size and sigma value.
- **Convert to Grayscale (-bw)**: Convert images to grayscale.
- **Laplacian Edge Detection (-edge)**: Detect edges using the Laplacian method.

## Usage

Run the compiled program with the desired operations:

```./image_processor <filename> <operation> [additional args]```

For example:

```./image_processor sample.bmp -fv -g 5 1.0```

## To Do List:

- Does not work with all BMP files, just the most basic of formats (no compression)
- Edge detection needs further work
- CUDA revision... convert all functions to CUDA kernels
- Make a GUI instead of command-line
