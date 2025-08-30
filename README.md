# contact-management-system

# 📒 Contact Book Management System

A simple yet feature-rich **Contact Book Management System** built in **C++**, designed to help users manage their personal contacts and groups efficiently.

## 🚀 Features

* 🔐 **User Authentication**

  * Sign up with username and password
  * Secure login system

* 👥 **Contact Management**

  * Add new contacts (with duplicate detection & overwrite confirmation)
  * View all contacts (sorted by name)
  * Search contacts (supports partial name matches)
  * Update contact phone numbers
  * Delete contacts

* 📂 **Group Management**

  * Create groups (with optional default contact)
  * View groups (only names or with members)
  * Add / Remove contacts from groups
  * Prevent duplicate members in groups

* ✉️ **Messaging**

  * Send messages to entire groups (simulated via console output)

* 📁 **File Handling**

  * Contacts stored in text files (`File_<username>.txt`)
  * Groups stored in text files (`groups_<username>.txt`)
  * Ensures persistence across program runs

## 🛠️ Technologies Used

* **Language:** C++
* **Concepts Applied:**

  * File Handling
  * Structures (`struct`)
  * Dynamic Memory Allocation
  * String Handling & Validation
  * User Input / Menu-driven System

## 📸 Preview
![contact](https://github.com/user-attachments/assets/25cef328-a587-4a68-b402-0d29f525163a)

## ▶️ How to Run

1. Clone this repository:

   ```bash
   git clone https://github.com/<your-username>/contact-book-management.git
   cd contact-book-management
   ```

2. Compile the program:

   ```bash
   g++ Group4.cpp -o contactbook
   ```

3. Run the executable:

   ```bash
   ./contactbook
   ```
## 👨‍💻 Author

Developed by **Zoha Shabbir** ✨
