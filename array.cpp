// <GroupNo>_<TeamLeaderID>_<Member1ID>_<Member2ID>_<Member3ID>.zip
// Use APA style citations within your code as comments and also mention them in your report.
// cite the code from outside
#include "csv.hpp"
// #include "date/tz.h"
#include <iostream>
#include <regex>

using namespace csv;
using namespace std;

string (*readFile(string filename))[4] { 
    CSVReader reader(filename);
    string (*file)[4] = new string[23600][4]; // head allcation of an array
    int rowIdx = 0;

    for (CSVRow &row : reader) {
        int colIdx = 0;
        string data;

        for (CSVField &field : row) {
            data = field.get();
            file[rowIdx][colIdx] = (data == " " || (data.empty())) ? "NULL" : data;
            
            if (colIdx == 3) { // change the format of the date
                data.pop_back();
                file[rowIdx][colIdx] = regex_replace(data, regex(", | "), "/");
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
    for (int i = 0; i < 26000; i++) {
        for (int j = 0; j < 4; j++) {
            cout << "data[" << i << "][" << j << "] = " << news[i][j] << std::endl;
        }
    }
}

int main() {
    string (*fakeN)[4] = readFile("fake.csv");
    string (*trueN)[4] = readFile("true.csv"); 
    readArray(trueN);

}

// sort
// year-based sorting is fully demonstrated.