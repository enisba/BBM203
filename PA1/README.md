# 🖼️ Enhanced Image Processing for Defense Applications using C++

This C++ project focuses on advanced **image processing techniques** tailored for **defense applications**. It implements various filtering and enhancement methods to improve image quality, extract hidden data, and optimize computational efficiency.

---

## 🚀 Features & Implementations

### 🎛️ Noise Reduction Filters
📌 **Mean & Gaussian Filters** remove unwanted noise while preserving important image details.

✅ **Before & After Example:**  
![Noise Reduction](https://github.com/user-attachments/assets/0f8a3555-fed8-4fac-84f9-f13a82ca6a26)

---

### 🔍 Sharpening Technique
📌 **Unsharp Masking** enhances image clarity and highlights critical features.

✅ **Example:**  
![Sharpening](https://github.com/user-attachments/assets/e81fb020-c272-47ca-8d37-9c87d71b2f06)

---

### 🛡️ LSB Steganography (Message Hiding)
📌 A technique for securely **embedding and extracting** hidden messages within images.

✅ **Steganography Demo:**  
![Steganography](https://github.com/user-attachments/assets/b34b0b28-9160-4e97-a72e-752fe7992ff4)

---

## ⚡ Optimizations & Data Handling

To ensure **high efficiency and computational performance**, this project leverages:

✔️ **Operator Overloading** – Simplifies operations and improves code readability.  
✔️ **Dynamic Memory Management** – Efficiently allocates and deallocates resources for large images.  
✔️ **Triangular Matrix Storage** – Reduces memory overhead while maintaining accuracy.  

✅ **Example:**  
![Memory Optimization](https://github.com/user-attachments/assets/9c2f324b-e207-4331-940a-ccfff8eaa943)

✅ **Matrix Representation:**  
![Triangular Matrix](https://github.com/user-attachments/assets/714ec196-be35-4f18-ada2-d9aafac84cc0)
![image](https://github.com/user-attachments/assets/bbe162b7-cbae-4fd0-9344-0e83a6b3899f)

---

## 📂 Installation & Usage

```sh
# Clone the repository
git clone https://github.com/enisba/BBM203/PA1.git

# Compile the C++ code (Example using g++)
g++ -o image_processor main.cpp image_filters.cpp -O2

# Run the program
./image_processor
