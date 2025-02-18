#include "csv.hpp"
#include "date.h"
#include <iostream>
#include <string>
#include <chrono>

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
  }
};

struct News {
  int counter;
  Node* head;
  Node* tail;

  News() : counter(0), head(nullptr), tail(nullptr) {}

  void printNewsCount() {
    cout << counter << endl;
  }

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
  }
};

int main() {
  News trueNews = News();
  auto start = chrono::high_resolution_clock::now();
  for (CSVRow &row : reader) {
    string entries[4];
    int counter = 0;
    for (CSVField &field : row) {
      entries[counter] = field.get();
      counter++;
    }
    trueNews.insertAtBack(entries[0], entries[1], entries[2], entries[3]);
  }
  auto stop = chrono::high_resolution_clock::now();
  trueNews.printNewsCount();
  auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
  cout << "Time spent constructing the linked list: " << duration.count() << " miliseconds" << endl;
}
