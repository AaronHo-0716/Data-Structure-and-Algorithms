# News Analysis System - Data Structures & Algorithms

This project implements a news processing and analysis system designed to demonstrate the implementation of the same logic across different **Programming Paradigms** and **Data Structures**.

## 📂 File Organization

To understand the project, please refer to the specific files below:

| File Name | Data Structure | Paradigm | Description |
|:--- |:--- |:--- |:--- |
| `linked-list.cpp` | **Doubly Linked List** | **Imperative (IP)** | Focuses on explicit state changes and manual pointer management. |
| `array1D.cpp` | **1D Array** | **Imperative (IP)** | Implements the same features using contiguous memory (Array) instead of nodes. |
| `linkedListV2.cpp` | **Doubly Linked List** | **Functional (FP)** | Focuses on immutability, lambdas, and higher-order functions. |

### 2. Supporting Files (Unchanged)
These files provide the infrastructure for both versions and remain consistent:
*   **Headers:**
    *   `csv.hpp`: Handles CSV parsing and data extraction.
    *   `date.hpp`: Manages date formatting.
*   **Data Sources:**
    *   `true.csv`: Dataset containing verified news articles.
    *   `fake.csv`: Dataset containing flagged or misleading news articles.

---

## 🏗 Data Structure Comparison

### Linked List Versions (`linked-list.cpp` & `linkedListV2.cpp`)
*   **Structure:** Uses individual `Node` objects connected by `next` and `prev` pointers.
*   **Memory:** Non-contiguous memory allocation.
*   **Performance:** Efficient for insertions and deletions, but requires sequential traversal to access elements.

### Array 1D Version (`array1D.cpp`)
*   **Structure:** Uses a **1D Array** to store `NewsArticle` objects.
*   **Memory:** Contiguous memory allocation, which is more "cache-friendly."
*   **Performance:** Allows for **Random Access** (accessing any element instantly via index). Sorting algorithms (like Bubble Sort or Quick Sort) interact differently with arrays compared to pointers in a linked list.

---

## 🛠 Programming Paradigms Explained

### Imperative Style
*   **Mutable State:** Directly modifies data structures in place.
*   **Control Flow:** Uses traditional `while` and `for` loops.
*   **Logic:** Tells the computer exactly *how* to update the memory step-by-step.

### Functional Style
*   **Immutability:** Instead of changing a list, it creates a new one (e.g., `functionalMergeSort` returns a brand new sorted list).
*   **Higher-Order Functions:** Uses `.filter()` and `.countIf()` methods that accept **Lambdas** as arguments.
*   **Pure Functions:** Functions are deterministic; they don't rely on or change global variables.

---

## 🚀 Key Features
1.  **Fake News Analysis:** Calculates the percentage of fake political articles per month.
2.  **Sorting:** Organize articles chronologically using Merge Sort or Bubble Sort.
3.  **Frequency Analysis:** Identifies the top 20 most frequent words in "Government News" titles.
4.  **Search & Filter:** Search datasets by specific years or categories.

---

## 💻 How to Build and Run

### Prerequisites
*   A C++ compiler (GCC 7.0+ or Clang)
*   Standard: **C++17** or higher

### Compiling
To run the **Linked List (Functional)** version:
```bash
g++ -std=c++17 linkedListV2.cpp -o fp
./fp
```

To run the **Linked List (Imperative)** version:
```bash
g++ -std=c++17 linked-list.cpp -o fp
./fp
```

To run the **Array 1D** version:
```bash
g++ -std=c++17 array1D.cpp -o array
./array
```

## ⚠️ Robustness & Error Handling
*  **Memory Management**: The Linked List versions use custom Destructors to prevent memory leaks, while the Array version leverages RAII for automatic memory management.
*  **Move Semantics**: (FP version) Efficiently transfers data between functions without expensive deep copies.
