#include "csv.hpp"
#include <iostream>
#include <map>
#include <sstream>
#include <string>

using namespace csv;
using namespace std;

CSVReader reader("test.csv");

map<string, int> monthMap = {
    {"January", 1},   {"February", 2}, {"March", 3},     {"April", 4},
    {"May", 5},       {"June", 6},     {"July", 7},      {"August", 8},
    {"September", 9}, {"October", 10}, {"November", 11}, {"December", 12}};

struct Date {
  int day;
  int month;
  int year;

  Date(string dateString) {
    istringstream ss(dateString);
    string monthStr;

    ss >> monthStr >> day;
    ss.ignore(1);
    ss >> year;

    month = monthMap[monthStr];
  }

  string getDate() {
    return to_string(year) + "-" + to_string(month) + "-" + to_string(day);
  }
};

struct Node {
  string title;
  string content;
  string category;

  Node *next;
  Node *prev;

  Node(int value) {}
};

int main() {
  for (CSVRow &row : reader) {
    for (CSVField &field : row) {
      cout << field.get() << endl;
    }
  }
}
