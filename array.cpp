#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

void readFile() {
    string line;
    ifstream trueNews("true.csv");
    ifstream fakeNews("fake.csv");
    vector<vector<string>> data;

    if (!trueNews.is_open()) cout << "Error opening 'true.scv' !" << endl;
    if (!fakeNews.is_open()) cout << "Error opening 'fake.scv' !" << endl;
    
    // Read the file line by line
    while (getline(trueNews, line)) {
        stringstream ss(line);
        string cell;
        vector<string> row;

        // Split the line by commas
        while (getline(ss, cell, '\t')) {
            row.push_back(cell);
        }

        // Add the row to the data
        data.push_back(row);
    }
    trueNews.close();

    while (getline(fakeNews, line)) {
        stringstream ss(line);
        string cell;
        vector<string> row;

        while (getline(ss, cell, '\t')) {
            row.push_back(cell);
        }

        data.push_back(row);
    }
    fakeNews.close();
}

int main() {
/*
    // Print the contents of the 2D vector
    for (const auto& row : data) {
        for (const auto& cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
*/
    return 0;
}
