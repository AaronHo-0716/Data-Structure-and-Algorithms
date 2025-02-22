#include "csv.hpp"
#include "date.h"
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

using namespace csv;
using namespace std;
using namespace dateParsing;

CSVReader reader("true.csv");

struct Node {
  string title;
  string content;
  string category;
  Date date;

  Node *next;
  Node *prev;

  Node(string title, string content, string category, string date) {
    this->title = title;
    this->content = content;
    this->category = category;
    this->date = Date(date);

    next = nullptr;
    prev = nullptr;
  }

  string printNews() {
    return title + "," + content + "," + category + "," + date.getDate();
  }
};

struct News {
  int counter;
  Node *head;
  Node *tail;

  News() : counter(0), head(nullptr), tail(nullptr) {}

  void printNewsCount() { cout << counter << endl; }

  void insertAtBack(string title, string content, string category,
                    string date) {
    Node *node = new Node(title, content, category, date);

    if (head == nullptr) {
      head = node;
      tail = node;
    } else {
      node->prev = tail;
      tail->next = node;
      tail = node;
    }
    counter++;
  }

  void traverse() {
    Node *current = head;
    if (current == nullptr) {
      cout << "News is empty" << endl;
    } else {
      while (current != nullptr) {
        cout << current->printNews() << endl;
        current = current->next;
      }
    }
  }

  int newsPerMonthByYear(int month, int year) {
    Node *current = head;
    int counter = 0;
    if (current == nullptr) {
      cout << "News is empty" << endl;
    } else {
      while (current != nullptr) {
        if (current->date.month == month && current->date.year == year &&
            (current->category == "politicsNews" ||
             current->category == "politics")) {
          counter++;
        }
        current = current->next;
      }
    }

    return counter;
  }
};

void printNewsPercentage(News trueNews, News fakeNews) {
  int filterYear;
  string months[12] = {"January",   "February", "March",    "April",
                       "May",       "June",     "July",     "August",
                       "September", "October",  "November", "December"};
  float percentageOfFakeNews[12] = {};

  cout << "Welcome to Percentage of Fake Political News Articles" << endl;
  while (true) {
    cout << "Please enter the year you want to search (Type 0 to quit): ";
    cin >> filterYear;

    if (filterYear == 0) {
      break;
    }

    for (int i = 0; i < 12; i++) {
      int trueCount = trueNews.newsPerMonthByYear(i + 1, filterYear);
      int fakeCount = fakeNews.newsPerMonthByYear(i + 1, filterYear);

      if (trueCount + fakeCount == 0) {
        percentageOfFakeNews[i] = 0; // Avoid division by zero
      } else {
        percentageOfFakeNews[i] =
            (static_cast<float>(fakeCount) / (trueCount + fakeCount)) * 100;
      }
    }

    cout << "\nPercentage of Fake Political News Articles in " << filterYear
         << "\n";
    cout << "---------------------------------------------------------\n";
    cout << left << setw(12) << "Month" << " | " << setw(20) << "Graph" << " | "
         << "Percentage\n";
    cout << "---------------------------------------------------------\n";

    for (int i = 0; i < 12; i++) {
      cout << left << setw(12) << months[i] << " | "; // Align month names
      int stars = static_cast<int>(
          percentageOfFakeNews[i]); // Convert percentage to integer for stars
      for (int j = 0; j < stars; j++) {
        cout << "*";
      }
      cout << setw(20 - stars) << "" << " | " << fixed << setprecision(2)
           << setw(6) << percentageOfFakeNews[i] << "%" << endl;
    }
  }
}
void sortArticle() {
  while (true) {
    int choice = 0;
    cout << "Sort articles:" << endl;
    cout << "1. Merge Sort" << endl;
    cout << "2. Bubble Sort" << endl;
    cout << "3. Quit" << endl;

    cin >> choice;

    if (choice == 1) {
    } else if (choice == 2) {
    } else if (choice == 3) {
      break;
    }
  }
}
void mostFrequentWord() {}
void searchArticle() {}

int main() {
  News trueNews = News();
  News fakeNews = News();

  auto startOfReadingTrue = chrono::high_resolution_clock::now();

  for (CSVRow &row : reader) {
    string entries[4];
    int counter = 0;
    for (CSVField &field : row) {
      entries[counter] = field.get();
      counter++;
    }
    trueNews.insertAtBack(entries[0], entries[1], entries[2], entries[3]);
  }
  auto stopOfReadingTrue = chrono::high_resolution_clock::now();
  cout << "Numbers of true news loaded: " << endl;
  trueNews.printNewsCount();

  auto durationReadingTrue = chrono::duration_cast<chrono::milliseconds>(
      stopOfReadingTrue - startOfReadingTrue);
  cout << "Time spent loading true news: " << durationReadingTrue.count()
       << " miliseconds\n"
       << endl;

  CSVReader reader("fake.csv");
  auto startOfReadingFake = chrono::high_resolution_clock::now();

  for (CSVRow &row : reader) {
    string entries[4];
    int counter = 0;
    for (CSVField &field : row) {
      entries[counter] = field.get();
      counter++;
    }
    fakeNews.insertAtBack(entries[0], entries[1], entries[2], entries[3]);
  }
  auto stopOfReadingFake = chrono::high_resolution_clock::now();
  cout << "Numbers of fake news loaded: " << endl;
  fakeNews.printNewsCount();

  auto durationReadingFake = chrono::duration_cast<chrono::milliseconds>(
      stopOfReadingFake - startOfReadingFake);
  cout << "Time spent loading fake news: " << durationReadingFake.count()
       << " miliseconds\n"
       << endl;

  while (true) {
    int choice = 0;
    cout << "Please choose one option: " << endl;
    cout << "1. Percentage of fake news pew year." << endl;
    cout << "2. Sort the news article by year." << endl;
    cout << "3. Most frequent words in goverment fake news." << endl;
    cout << "4. Search article." << endl;
    cout << "5. Exit " << endl;
    cin >> choice;

    if (choice == 1) {
      printNewsPercentage(trueNews, fakeNews);
    } else if (choice == 2) {
      sortArticle();
    } else if (choice == 3) {
    } else if (choice == 4) {
    } else if (choice == 5) {
      break;
    }
  }
}
