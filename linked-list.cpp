#include "csv.hpp"
#include "date.h"
#include <iostream>
#include <string>

using namespace csv;
using namespace std;
using namespace dateParsing;

CSVReader reader("test.csv");

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
  }
};

struct News {
  int counter;
  Node* head;
  Node* tail;

  News() : counter(0), head(nullptr), tail(nullptr) {}

  void insertAtBack(string title, string content, string category, string date) {
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
    cout << counter << endl;
  }
};

int main() {
  News testNews = News();
  for (CSVRow &row : reader) {
    string entries[4];
    int counter = 0;
    for (CSVField &field : row) {
      entries[counter] = field.get();
      counter++;
    }
    testNews.insertAtBack(entries[0], entries[1], entries[2], entries[3]);
  }
}
