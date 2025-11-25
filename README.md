# 🖼️ LSB Image Steganography Project

## 📌 Overview
This project implements **LSB (Least Significant Bit) Image Steganography** using the C programming language.  
It supports both **encoding** (hiding a secret message inside a BMP image) and **decoding** (extracting the hidden message).  
The program operates on **24-bit BMP image files** and **text files**, embedding secret data by modifying only the least significant bits of pixel values.

---

## 🎯 Features
- 🔐 **Encode text data** into BMP image using LSB manipulation  
- 🔍 **Decode hidden data** from a stego image  
- 🖥️ Supports `.bmp` for image input and `.txt` for secret data  
- 📁 Generates a new stego image or decodes into a secret output file  
- 🧩 Uses a magic string `#*` to verify stego signatures  
- 🏗️ Robust error handling and argument validation  

---

## 📁 Project Structure
```
LSB_Steganography/
│
├── encode.c
├── encode.h
├── decode.c
├── decode.h
├── common.h
├── types.h
├── main.c
└── beautiful.bmp        # Original image
└── stego.bmp            # Output encoded image
└── secret.txt           # Secret message file
```

---

## ⚙️ How LSB Steganography Works
The **Least Significant Bit (LSB)** of each byte in the BMP pixel data is replaced with bits of the secret message.

Example:  
```
Original byte: 11001010  
Secret bit :          1  
Modified byte: 11001011
```

Only 1 bit change → No visible difference in the image.

---

## 🧪 Encoding Pipeline
The encoding procedure stores the following in the image:

1. **Magic string (`#*`)**  
2. **File extension size**  
3. **File extension** (e.g., `.txt`)  
4. **Secret file size**  
5. **Secret file data**

All encoded bit‑by‑bit using LSB replacement.

---

## 🧾 Command Usage

### 🔐 **Encode Data**
```
./a.out -e <source.bmp> <secret.txt> <output_stego.bmp>
```
Example:
```
./a.out -e beautiful.bmp secret.txt stego.bmp
```

If no output file is given:
```
stego.bmp is created as default
```

---

### 🔍 **Decode Data**
```
./a.out -d <stego.bmp> <outputfile(optional)>
```

Example:
```
./a.out -d stego.bmp decoded.txt
```

If no output filename is given:
```
sec.txt is created by default
```

---

## 🛠️ Key Functions

### **Encoding**
- `encode_magic_string()`  
- `encode_secret_file_extn()`  
- `encode_secret_file_size()`  
- `encode_secret_file_data()`  
- `encode_byte_to_lsb()` / `encode_int_to_lsb()`

### **Decoding**
- `decode_magic_string()`  
- `decode_secret_file_extn()`  
- `decode_secret_file_size()`  
- `decode_secret_file_data()`  
- `decode_byte_from_lsb()` / `decode_int_to_lsb()`

---

## 📌 Example Secret Message
```
My password is SECRET ;)
```

This message is stored inside **beautiful.bmp** and extracted back during decoding.

---

## 📤 Output
During encoding:
```
## Encoding Procedure Started ##
Copying Image Header
Encoding Magic String Signature
Encoding secret.txt File Extension
Encoding secret.txt File Size
Encoding secret.txt File Data
Copying Left Over Data
## Encoding Done Successfully ##
```

During decoding:
```
## Decoding Procedure Started ##
Decoding Magic String Signature
Decoding Output File Extension
Decoding File Size
Decoding File Data
## Decoding Done Successfully ##
```

---

## 📜 File Format Requirements
### **BMP Image**
- Must be **24-bit uncompressed BMP**
- Header size = 54 bytes is assumed

### **Secret File**
- Must be `.txt`

---

## 🧑‍💻 Technologies Used
- **C Programming**
- **File I/O**
- **Bitwise Operations**
- **Binary Data Manipulation**
- **Image Processing Logic**

---

## 📝 Author
Developed as part of an academic demonstration of **digital steganography using LSB technique**.

---

## 📄 License
This project is open-source and free to use for educational purposes.
