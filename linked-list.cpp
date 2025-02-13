#include "csv.hpp"
#include <iostream>

using namespace csv;
using namespace std;

CSVReader reader("test.csv");

int main() {
  for (CSVRow &row : reader) {
    for (CSVField &field : row) {
      cout << field.get() << endl;
    }
  }
}
