#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <map>
#include <sstream>
#include <string>

using namespace std;

namespace dateParsing {
struct Date {
private:
  map<string, int> monthMap = {
      {"January", 1}, {"Jan", 1},       {"February", 2}, {"Feb", 2},
      {"March", 3},   {"Mar", 3},       {"April", 4},    {"Apr", 4},
      {"May", 5},     {"June", 6},      {"Jun", 6},      {"July", 7},
      {"Jul", 7},     {"August", 8},    {"Aug", 8},      {"September", 9},
      {"Sep", 9},     {"October", 10},  {"Oct", 10},     {"November", 11},
      {"Nov", 11},    {"December", 12}, {"Dec", 12}};

public:
  int day, month, year;

  Date() : day(0), month(0), year(0) {}

  Date(string dateString) {
    if (dateString.find("-") != string::npos) {
      parseDashFormat(dateString);
    } else if(dateString.find(",") != string::npos){
      parseTextFormat(dateString);
    } else {
      day = 0;
      month = 0;
      year = 0;
    }
  }

  string getDate() {
    return to_string(year) + "-" + to_string(month) + "-" + to_string(day);
  }

  void parseTextFormat(string dateString) {
    istringstream ss(dateString);
    string monthStr;

    ss >> monthStr >> day;
    ss.ignore(1);
    ss >> year;

    month = monthMap[monthStr];
  }

  void parseDashFormat(string dateString) {
    stringstream ss(dateString);
    string dayStr, monthStr, yearStr;

    getline(ss, dayStr, '-');
    getline(ss, monthStr, '-');
    getline(ss, yearStr, '-');

    day = stoi(dayStr);
    month = monthMap[monthStr];

    if (yearStr.length() == 2) {
      int yearNum = stoi(yearStr);
      year = (yearNum < 50) ? (2000 + yearNum)
                            : (1900 + yearNum);
    } else {
      year = stoi(yearStr);
    }
  }
};
} // namespace dateParsing

#endif
