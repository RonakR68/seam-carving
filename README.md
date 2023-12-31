# Seam Carving with OpenCV

## Overview

This C++ program demonstrates image resizing using the Seam Carving algorithm with the OpenCV library. Seam carving is a content-aware image resizing technique that preserves important features in an image while resizing it.

## Prerequisites

Before running the code, ensure that you have the following installed:

- C++ compiler
- OpenCV library: Follow instructions in opencv_installation_ubuntu.pdf for opencv installation and configuration for running c++ program in ubuntu system.

## How to Run

1. Clone the repository to your local machine:

   ```bash
   git clone https://github.com/RonakR68/seam-carving.git
   
2. Navigate to the project directory:
 
   ```bash
   cd seam-carving
   cd src
   
3. Compile the code using a C++ compiler:

   ```bash
   g++ -o seamcarving seamcarving.cpp -std=c++11 -lopencv_core -lopencv_imgcodecs -lopencv_highgui
   
4. Run the compiled executable:

   ```bash
   ./seamcarving

5. Follow the on-screen prompts to enter the input image path, output image path, and desired output dimensions.

## Output

You can find output image at the location specified during program execution. You can also find sample input and output images used for testing of this program in Input and Output folders respectively.




