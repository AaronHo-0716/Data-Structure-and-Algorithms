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
        rowIdx++;
    }
    
    (filename == "true.csv") ? cout << "True rows: " << rowIdx << endl : cout << "False rows: " << rowIdx << endl; // debug line
    // true - 21417, false - 23481
    return file;
}

void readArray(myArray** file) {
    for (int i = 0; i < 23500; i++) {
        for (int j = 0; j < 4; j++) {
            cout << "data[" << i << "][" << j << "] = " << file[i]->operator[](j) << endl;
        }
    }
}

int main() {
    
    auto startF = high_resolution_clock::now();
    myArray** fakeN = readFile("fake.csv");
    auto stopF = high_resolution_clock::now();

    cout << "FAke: " << (duration_cast<milliseconds>(stopF - startF)).count() << "ms" << endl;

    auto startT = high_resolution_clock::now();
    myArray** trueN = readFile("true.csv"); 
    auto stopT = high_resolution_clock::now();

    cout << "True: " << (duration_cast<milliseconds>(stopT - startT)).count() << "ms" << endl;



    // myArray** testN = readFile("test.csv"); 
    // readArray(testN);
}

/*
string (*readFile(string filename))[4] { 
    CSVReader reader(filename);
    string (*file)[4] = new string[23500][4]; // head allcation of an array
    int rowIdx = 0;

    for (CSVRow &row : reader) {
        int colIdx = 0;
        string data;

        for (CSVField &field : row) {
            data = field.get();
            
            file[rowIdx][colIdx] = (data == " " || (data.empty())) ? "NULL" : data;
            
            if (colIdx == 3) { // change the format of the date
                file[rowIdx][3] = dateParsing::Date(data).getDate();
            }
            colIdx++;
        }
        rowIdx++;
    }

    (filename == "true.csv") ? cout << "True rows: " << rowIdx << endl : cout << "False rows: " << rowIdx << endl; // debug line
    // true - 21417, false - 23481
    return file;
}

void readArray(string (*news)[4]) {
    for (int i = 0; i < 23500; i++) {
        for (int j = 0; j < 4; j++) {
            cout << "data[" << i << "][" << j << "] = " << news[i][j] << std::endl;
        }
    }
}
*/
// sort
// year-based sorting is fully demonstrated.