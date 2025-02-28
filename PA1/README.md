# Image Processing for Defense Applications (C++)

This C++ project enhances images and secures hidden data for defense applications. It includes noise reduction, sharpening, and steganography techniques.

---

## Features

### Noise Reduction
- Mean & Gaussian Filters remove unwanted noise while preserving image details.
- **Before & After Example:**  
  <p align="center">
    <img src="https://github.com/user-attachments/assets/0f8a3555-fed8-4fac-84f9-f13a82ca6a26" width="45%"> 
  </p>

### Image Sharpening
- Unsharp Masking enhances clarity and highlights critical features.
- **Example:**  
  <p align="center">
    <img src="https://github.com/user-attachments/assets/e81fb020-c272-47ca-8d37-9c87d71b2f06" width="45%">
  </p>

### Hidden Message Embedding (LSB Steganography)
- Securely embed and extract messages within images.
- **Steganography Demo:**  
  <p align="center">
    <img src="https://github.com/user-attachments/assets/b34b0b28-9160-4e97-a72e-752fe7992ff4" width="45%">
  </p>

### Performance Optimizations
- Efficient Memory Management for handling large images.
- Operator Overloading for simplified calculations.
- Triangular Matrix Storage to reduce memory usage.
- **Memory Optimization Example:**  
  <p align="center">
    <img src="https://github.com/user-attachments/assets/9c2f324b-e207-4331-940a-ccfff8eaa943" width="45%">
  </p>
- **Matrix Representation:**  
  <p align="center">
    <img src="https://github.com/user-attachments/assets/714ec196-be35-4f18-ada2-d9aafac84cc0" width="45%">
    <img src="https://github.com/user-attachments/assets/bbe162b7-cbae-4fd0-9344-0e83a6b3899f" width="45%">
  </p>

---

## Installation & Usage

```sh
# Clone the repository
git clone https://github.com/enisba/BBM203/PA1.git

# Compile the code
g++ -o image_processor main.cpp image_filters.cpp -O2

# Run the program
./image_processor
```

Simple, optimized, and powerful image processing.
