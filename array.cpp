// <GroupNo>_<TeamLeaderID>_<Member1ID>_<Member2ID>_<Member3ID>.zip
// Use APA style citations within your code as comments and also mention them in your report.
// cite the code from outside
#include "csv.hpp"
#include "date.h"
#include <iostream>
#include <chrono>

using namespace std;
using namespace csv;
using namespace dateParsing;
using namespace chrono;

struct myArray {
    private:
        string dataCleaning(string col1) {
            return (col1 == " " || (col1.empty())) ? "NULL" : col1;
        }
    
        Date parseDate(string col3) {
            return (Date(col3));
        }

    public: 
        string col[3] = {"", "", ""};
        Date col3;

        myArray() : col{"", "NULL", ""}, col3(Date("0000-00-00")) {}

        myArray (CSVRow &row) {
            string data[4];
            int colIdx = 0;

            for (CSVField &field : row) {
                data[colIdx] = field.get();
                colIdx++;
            }

            this->col[0] = data[0];
            this->col[1] = dataCleaning(data[1]);
            this->col[2] = data[2];
            this->col3 = parseDate(data[3]);
        }

        // copy construtor
        // which is called when a new myArray object is created using another existing myArray object. (myArray b = a;)
        // It copies the values from other into the new object
        myArray(const myArray &other) {
            for (int i = 0; i < 3; i++) {
                col[i] = other.col[i];
            }
            col3 = other.col3;
        }

        // copy assignment operator
        // which is used when an already existing object is assigned a new value from another existing object
        myArray& operator=(const myArray &other) {
            if (this != &other) { // ensures the object doesn't try to copy itself 
                for (int i = 0; i < 3; i++) {
                    col[i] = other.col[i];
                }
                col3 = other.col3;
            }
            return *this;
        }

        string operator[](int idx) const { 
            if (idx >= 0 && idx < 3) {  
                return col[idx];  
            } else if (idx == 3) {  
                return Date(col3).getDate();  
            } else {
                throw out_of_range("Index out of range");
            }
        }
};

myArray** readFile(string filename, int &rowCount) {
    CSVReader reader(filename);
    rowCount = 0;

    myArray** file = new myArray*[23500];

    if (!file) {
        cerr << "Memory allocation failed!" << endl;
        return nullptr;
    }
    
    for (CSVRow &row : reader) {
        file[rowCount] = new myArray(row);
        rowCount++;
    }

    // (filename == "true.csv") ? cout << "True rows: " << rowIdx << endl : cout << "False rows: " << rowIdx << endl; // debug line
    // true - 21417, false - 23481
    return file;
}

void readArray(myArray** file, int rowCount) {
    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < 4; j++) {
            cout << "data[" << i << "][" << j << "] = " << (*file[i])[j] << endl;
        }
    }
}

void percentage(myArray** trueN, myArray** fakeN) {
    int input = -1;
    float total = 0;
    int fake = 0;

        // Input validation loop
    while (1) {
        cout << "Do you want to do any filter\n"
                << "1. Month\n"
                << "2. Year\n"
                << "3. No\n"
                << "0. Exit\n"
                << ": ";

        if (!(cin >> input) || input < 0 || input > 2) {
            cout << "Invalid input. Please enter a valid integer (0, 1, 2, or 3)." << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            continue;
        }

        switch (input) {
            case 1:
                // Do filter for month
                cout << "MONTH" << endl;
                break;

            case 2:
                // Do filter for year
                cout << "Year" << endl;
                break;

            case 3:
                // total = static_cast<float>((**trueN).arraySize + (**fakeN).arraySize);
                // fake = (**fakeN).arraySize;
                break;

            default: // case 0
                break;
        }

        break;
    }
}

void filter(myArray** file) {

}

void merge(myArray** file, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Allocate temporary arrays on the heap
    myArray* Ltemp = new myArray[n1];
    myArray* Rtemp = new myArray[n2];

    // Copy data
    for (int i = 0; i < n1; i++) {
        Ltemp[i] = *file[left + i];
    } 
    for (int i = 0; i < n2; i++) {
        Rtemp[i] = *file[mid + 1 + i];
    } 

    int i = 0, j = 0, k = left;

    // Merge process
    while (i < n1 && j < n2) {        
        if (Ltemp[i].col3.year < Rtemp[j].col3.year ||
            (Ltemp[i].col3.year == Rtemp[j].col3.year && Ltemp[i].col3.month < Rtemp[j].col3.month) ||
            (Ltemp[i].col3.year == Rtemp[j].col3.year && Ltemp[i].col3.month == Rtemp[j].col3.month && Ltemp[i].col3.day < Rtemp[j].col3.day)) {
                *file[k] = Ltemp[i++];
        } else {
            *file[k] = Rtemp[j++];
        }
        k++;
    }

    // Copy remaining elements
    while (i < n1) *file[k++] = Ltemp[i++];
    while (j < n2) *file[k++] = Rtemp[j++];

    // Free heap memory
    delete[] Ltemp;
    delete[] Rtemp;
}

void mergeSort(myArray** file, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(file, left, mid);
        mergeSort(file, mid + 1, right);
        merge(file, left, mid, right);
    }
}

int main() {
    auto startF = high_resolution_clock::now();
    int rowCount = 0;
    myArray** file = readFile("fake.csv", rowCount);
    if (!file) return -1;  // Error handling

    cout << "Total rows before sorting: " << rowCount << endl;

    mergeSort(file, 0, rowCount - 1);


    // Print data
    // readArray(file, rowCount);

    cout << "Total rows after sorting: " << rowCount << endl;


    // Free memory
    for (int i = 0; i < rowCount; i++) {
        delete file[i];
    }
    delete[] file;
    auto stopF = high_resolution_clock::now();
    cout << "Fake: " << (duration_cast<milliseconds>(stopF - startF)).count() << "ms" << endl;
    return 0;
}
