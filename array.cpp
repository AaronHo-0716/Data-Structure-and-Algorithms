// <GroupNo>_<TeamLeaderID>_<Member1ID>_<Member2ID>_<Member3ID>.zip
// Use APA style citations within your code as comments and also mention them in your report.
// cite the code from outside
#include "csv.hpp"
#include "date.h"
#include <iostream>
#include <chrono>
#include <variant>
#include <algorithm>

using namespace std;
using namespace csv;
using namespace dateParsing;
using namespace chrono;

struct myArray {
    private:  
        Date parseDate(string col3) {
            return (Date(col3));
        }

    public: 
        string col0, col1, col2;
        Date col3;

        myArray() : col0(""), col1(""), col2(""), col3(Date("0000-00-00")) {}

        myArray (CSVRow &row) {
            string data[4];
            int colIdx = 0;

            for (CSVField &field : row) {
                data[colIdx] = field.get();
                colIdx++;
            }

            this->col0 = data[0];
            this->col1 = data[1];
            this->col2 = data[2];
            this->col3 = parseDate(data[3]);
        }

        using ReturnType = variant<string, Date>;
        ReturnType operator[](int idx) const { 
            switch (idx) {
                case 0: return col0; break;
                case 1: return col1; break;
                case 2: return col2; break;
                case 3: return col3; break;
                
                default: throw out_of_range("Index out of range");
            }
        }

        // copy assignment operator
        // which is used when an already existing object is assigned a new value from another existing object
        myArray& operator=(const myArray &other) {
            if (this != &other) { // ensures the object doesn't try to copy itself 
                col0 = other.col0;
                col1 = other.col1;
                col2 = other.col2;
                col3 = other.col3;
            }
            return *this;
        }

        // // copy construtor
        // // which is called when a new myArray object is created using another existing myArray object. (myArray b = a;)
        // // It copies the values from other into the new object
        // myArray(const myArray &other) {
        //     for (int i = 0; i < 3; i++) {
        //         col[i] = other.col[i];
        //     }
        //     col3 = other.col3;
        // }

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
    string data;
    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < 4; j++) {
            data = (j == 3) ? get<Date>((*file[i])[j]).getDate() : get<string>((*file[i])[j]);
            cout << "data[" << i << "][" << j << "] = " << data << endl;
        }
    }
}

void merge(myArray** file, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Allocate temporary arrays on the heap
    myArray* Ltemp = new myArray[n1];
    myArray* Rtemp = new myArray[n2];

    // Copy data
    for (int i = 0; i < n1; i++) Ltemp[i] = *file[left + i];
    for (int i = 0; i < n2; i++) Rtemp[i] = *file[mid + 1 + i]; 

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

void percentage(myArray** trueN, myArray** fakeN, int trueRow, int fakeRow) {
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

        if (!(cin >> input) || input < 0 || input > 3) {
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
                total = static_cast<float>(trueRow + fakeRow);
                fake = fakeRow;
                cout << "Percentage: " << (fake / total) * 100 << "%" << endl;
                break;

            default: // case 0
                break;
        }

        break;
    }     
}

void filterByYear() {
    int input; 

    while (1) {
        cout << "Do you want to do any filter\n"
                << "1. Month\n"
                << "2. Year\n"
                << "3. No\n"
                << "0. Exit\n"
                << ": ";

        if (!(cin >> input) || input < 0 || input > 3) {
            cout << "Invalid input. Please enter a valid integer (0, 1, 2, or 3)." << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            continue;
        }
        break;
    }  
}

int* findUnique(myArray** file, int rowCount, int sizeY) {
    int* arr = new int[sizeY];
    int count = 0;

    for (int i = 0; i < rowCount; i++) {
        // Use find to check if file[i]->col3.year already exists in arr
        if (file[i]->col3.year != 0 && find(arr, arr + count, file[i]->col3.year) == arr + count) {
            arr[count++] = file[i]->col3.year;  
        }
    }
    return arr;
}

string* findUniqueSubject(myArray** file, int rowCount, int size) {
    string* arr = new string[size];
    int count = 0;

    for (int i = 0; i < rowCount; i++) {
        if (find(arr, arr + count, file[i]->col2) == arr + count) {
            arr[count++] = file[i]->col2; 
        }
    }
    return arr;
}

void printNewsPercentage() {

}

void sortArticle() {
    cout << "Sort articles:" << endl;
    cout << "1. Merge Sort" << endl;
    cout << "2. Bubble Sort" << endl;
}

void mostFrequentWord() {}

void searchArticle() {}

void menu() {
    while (1) {
        int choice = 0;
        cout << "Please choose one option: " << endl;
        cout << "1. Percentage of fake news pew year." << endl;
        cout << "2. Sort the news article by year." << endl;
        cout << "3. Most frequent words in goverment fake news." << endl;
        cout << "4. Search article." << endl;
        cout << "5. Exit" << endl;

        cin >> choice;

        switch (choice) {
            case 1: printNewsPercentage(); break;
            case 2: sortArticle(); break;
            case 3: mostFrequentWord(); break;
            case 4: searchArticle(); break;
            case 5: break;
            default: break;
        }
    }
}

int main() {
    // fake.csv
    auto startF = high_resolution_clock::now();
    int rowCount = 0;

    myArray** fakeN = readFile("fake.csv", rowCount);
    if (!fakeN) return -1;  // Error handling
    
    // action here
    // mergeSort(fakeN, 0, rowCount - 1);

    // int sizeY = (rowCount > 23000) ? 6 : 2;
    // int* yearY = findUnique(fakeN, rowCount, sizeY);
    // for (int i = 0; i < sizeY; i++) {
    //     cout << year[i] << endl;
    // }

    // int sizeS = (rowCount > 23000) ? 6 : 2;
    // string* subject = findUniqueSubject(fakeN, rowCount, sizeS);
    // for (int i = 0; i < sizeS; i++) {
    //     cout << subject[i] << endl;
    // }

    for (int i = 0; i < rowCount; i++) delete fakeN[i]; // Free memory
    delete[] fakeN;

    auto stopF = high_resolution_clock::now();
    cout << "Fake: " << (duration_cast<milliseconds>(stopF - startF)).count() << "ms" << endl;
    



    // true.csv
    auto startT = high_resolution_clock::now();
    int rowCount2 = 0;
    myArray** trueN = readFile("true.csv", rowCount2);
    if (!trueN) return -1;  // Error handling

    percentage(trueN, fakeN, rowCount2, rowCount);

    for (int i = 0; i < rowCount; i++) delete trueN[i]; // delete memory
    delete[] trueN;

    auto stopT = high_resolution_clock::now();
    cout << "True: " << (duration_cast<milliseconds>(stopT - startT)).count() << "ms" << endl;

    return 0;
}
