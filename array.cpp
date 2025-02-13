
#include "csv.hpp"
#include <iostream>

using namespace csv;
using namespace std;

string (*readFake())[4] { 
    CSVReader reader("fake.csv");
    string (*fakeN)[4] = new string[23500][4]; // head allcation of an array
    int rowIdx = 0;

    for (CSVRow &row : reader) {
        int colIdx = 0;

        for (CSVField &field : row) {
            fakeN[rowIdx][colIdx] = (field.get() == " " || (field.get()).empty()) ? "NULL" : field.get();
            colIdx++;
        }
        rowIdx++;
    }
    cout << "Fake rows: " << rowIdx << endl; // 23481
    return fakeN;
}

string (*readTrue())[4] { 
    CSVReader reader("true.csv");
    string (*trueN)[4] = new string[23600][4]; // head allcation of an array
    int rowIdx = 0;

    for (CSVRow &row : reader) {
        int colIdx = 0;

        for (CSVField &field : row) {
            trueN[rowIdx][colIdx] = (field.get() == " " || (field.get()).empty()) ? "NULL" : field.get();
            colIdx++;
        }
        rowIdx++;
    }
    cout << "True rows: " << rowIdx << endl; // 21417
    return trueN;
}

int main() {
    string (*fakeN)[4] = readFake();
    string (*trueN)[4] = readTrue(); 

}