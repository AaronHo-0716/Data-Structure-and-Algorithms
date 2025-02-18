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
            // cout << "(Debug line 19)" << (Date(col3)).getDate() << endl;
            return (Date(col3));
        }

    public: 
        string col[3];
        Date col3;
        int arraySize = 0;

        myArray() : col{ "", "NULL", "" }, col3(Date("")) {}

        myArray (CSVRow &row) {
            string data[4];
            int rowIdx;

            for (CSVField &field : row) {
                data[rowIdx] = field.get();
                rowIdx++;
            }

            this->col[0] = data[0];
            this->col[1] = dataCleaning(data[1]);
            this->col[2] = data[2];
            this->col3 = parseDate(data[3]);
        }

        string operator[](int idx) {
            if (idx >= 0 && idx < 3) {  
                return col[idx];  
            } else if (idx == 3) {  
                return col3.getDate(); 
            } else {
                throw out_of_range("Index out of range");  // Throw an exception for invalid indices
            }
        }
};

myArray** readFile(string filename) {
    CSVReader reader(filename);

    myArray** file = new myArray*[23500];
    int rowIdx = 0;

    for (CSVRow &row : reader) {
        file[rowIdx] = new myArray(row);
        file[rowIdx] -> arraySize = rowIdx;
        rowIdx++;
    }
    (*file) -> arraySize = rowIdx;
    
    // (filename == "true.csv") ? cout << "True rows: " << rowIdx << endl : cout << "False rows: " << rowIdx << endl; // debug line
    // true - 21417, false - 23481
    return file;
}

void readArray(myArray** file) {
    cout << "Debug: HAHAHHAAHAHAH" << endl;
    for (int i = 0; i < (**file).arraySize ; i++) {
        for (int j = 0; j < 4; j++) {
            cout << "data[" << i << "][" << j << "] = " << file[i]->operator[](j) << endl;
        }
    }
}

/*
void percentage(myArray** trueN, myArray** fakeN) {
    int total = ((**trueN).arraySize + (**fakeN).arraySize);

    float percentage = (static_cast<float>((**fakeN).arraySize) / total) * 100;
    
    
    cout << percentage << "%" << endl;
}
*/

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
                total = static_cast<float>((**trueN).arraySize + (**fakeN).arraySize);
                fake = (**fakeN).arraySize;
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
    const int n1 = mid - left + 1;
    const int n2 = right - mid;

    // temporary arrays
    myArray** L = new myArray*[n1];
    myArray** R = new myArray*[n2];


    for (int i = 0; i < n1; i++) {
        L[i] = file[left + i];
    }

    for (int i = 0; i < n2; i++) {
        R[i] = file[mid + 1 + i];
    }

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {        
        // First compare the years
        if (L[i]->col3.year < R[j]->col3.year) {
            file[k] = L[i++];
        } else if (L[i]->col3.year > R[j]->col3.year) {
            file[k] = R[j++];
        } else { 
            // if year are same, then compare month
            if (L[i]->col3.month < R[j]->col3.month) {
                file[k] = L[i++];
            } else if (L[i]->col3.month > R[j]->col3.month) {
                file[k] = R[j++];
            } else {
                // if year and month are same, compare day
                if (L[i]->col3.day < R[j]->col3.day) {
                    file[k] = L[i++];
                } else {
                    file[k] = R[j++];
                }
            }
        }
        k++;
    }

    // Copy remaining elements
    while (i < n1) file[k++] = L[i++];
    while (j < n2) file[k++] = R[j++];

    delete[] L;
    delete[] R;

    // for (int i = 0; i < 6; i++) {
    //     cout << file[i]->col3.getDate() << endl;
    // }
    
}


void mergeSort(myArray** file, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(file, left, mid);
        mergeSort(file, mid + 1, right);
        merge(file, left, mid, right);
    }
    cout << "Debug 208: Next" << endl;
}


int main() {
    
    // auto startF = high_resolution_clock::now();
    // myArray** fakeN = readFile("fake.csv");
    // auto stopF = high_resolution_clock::now();

    // cout << "Fake: " << (duration_cast<milliseconds>(stopF - startF)).count() << "ms" << endl;

    // auto startT = high_resolution_clock::now();
    // myArray** trueN = readFile("true.csv"); 
    // auto stopT = high_resolution_clock::now();

    // cout << "True: " << (duration_cast<milliseconds>(stopT - startT)).count() << "ms" << endl;

    // percentage(trueN, fakeN);

    auto startTest = high_resolution_clock::now();
    myArray** testN = readFile("test.csv"); 
    auto stopTest = high_resolution_clock::now();
    
    const int size = (**testN).arraySize;
    mergeSort(testN, 0, size - 1);
    (**testN).arraySize++;

    readArray(testN);
    
    cout << "Test: " << (duration_cast<milliseconds>(stopTest - startTest)).count() << "ms" << endl;
    // readArray(testN);
}


// sort
// year-based sorting is fully demonstrated.