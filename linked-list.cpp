#include "csv.hpp"
#include "date.h"
#include <chrono>
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
};

int main() {
  News trueNews = News();
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
  trueNews.printNewsCount();
  auto durationReadingTrue = chrono::duration_cast<chrono::milliseconds>(
      stopOfReadingTrue - startOfReadingTrue);
  cout << "Time spent constructing the linked list: "
       << durationReadingTrue.count() << " miliseconds" << endl;

  auto startOfPrintingTrue = chrono::high_resolution_clock::now();
  trueNews.traverse();
  auto stopOfPrintingTrue = chrono::high_resolution_clock::now();
  auto durationPrintingTrue = chrono::duration_cast<chrono::milliseconds>(
        stopOfPrintingTrue - startOfPrintingTrue);
  cout << "Time spent printing all the true news: "
       << durationReadingTrue.count() << " miliseconds" << endl;
}
