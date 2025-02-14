
#include "csv.hpp"
#include <iostream>

using namespace csv;
using namespace std;

string (*readFile(string filename))[4] { 
    CSVReader reader(filename);
    string (*file)[4] = new string[23600][4]; // head allcation of an array
    int rowIdx = 0;

    for (CSVRow &row : reader) {
        int colIdx = 0;

        for (CSVField &field : row) {
            file[rowIdx][colIdx] = (field.get() == " " || (field.get()).empty()) ? "NULL" : field.get();
            colIdx++;
        }
        rowIdx++;
    }
    (filename == "true.csv") ? cout << "True rows: " << rowIdx << endl : cout << "False rows: " << rowIdx << endl; // debug line
    // true - 21417, false - 23481
    return file;
}

int main() {
    string (*fakeN)[4] = readFile("fake.csv");
    string (*trueN)[4] = readFile("true.csv"); 
}