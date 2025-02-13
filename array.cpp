#include "csv.hpp"
#include <iostream>

using namespace csv;
using namespace std;

CSVReader reader("fake.csv");

int main() {
    int count = 0;

    for (CSVRow &row : reader) {
        for (CSVField &field : row) {
            if ((field.get()) == " " || field.get().empty())
                cout << row.to_json_array() << endl;

        }
    }
    cout << "Count: " << count << endl;
}
