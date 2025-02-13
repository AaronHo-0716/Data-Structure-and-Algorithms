
#include "csv.hpp"
#include <iostream>

using namespace csv;
using namespace std;

CSVReader reader("fake.csv");

int main() {
    string (*fake)[4] = new string[30000][4]; // head allcation of an array
    int rowIdx = 0;

    for (CSVRow &row : reader) {
        int colIdx = 0;

        for (CSVField &field : row) {
            fake[rowIdx][colIdx] = (field.get() == " " || (field.get()).empty()) ? "NULL" : field.get();
            colIdx++;
        }
        rowIdx++;
    }
    cout << fake[12215][1] << endl;
    cout << endl;
    cout << fake[12216][1] << endl;
    cout << endl;
    cout << fake[12217][1] << endl;
    return 0;
}