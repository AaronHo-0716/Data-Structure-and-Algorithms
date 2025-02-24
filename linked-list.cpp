// vincentlaucsb. (2018, July 2). GitHub - vincentlaucsb/csv-parser: A high-performance, fully-featured CSV parser and serializer for modern C++. GitHub. https://github.com/vincentlaucsb/csv-parser/tree/master

#include "csv.hpp"
#include "date.hpp"
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

  Node *split(Node *head) {
    Node *slow = head;
    Node *fast = head;

    while (fast->next && fast->next->next) {
      slow = slow->next;
      fast = fast->next->next;
    }

    Node *secondHalf = slow->next;
    slow->next = nullptr;
    if (secondHalf) {
      secondHalf->prev = nullptr;
    }
    return secondHalf;
  }

  Node *merge(Node *first, Node *second) {
    if (!first) {
      return second;
    }
    if (!second) {
      return first;
    }

    if (first->date.year <= second->date.year) {
      first->next = merge(first->next, second);
      if (first->next) {
        first->next->prev = first;
      }
      first->prev = nullptr;
      return first;
    } else {
      second->next = merge(first, second->next);
      if (second->next) {
        second->next->prev = second;
      }
      second->prev = nullptr;
      return second;
    }
  }

  void mergeSortByYear() {
    if (!head || !head->next) {
      return;
    }

    Node *second = split(head);

    News left;
    News right;
    left.head = head;
    right.head = second;

    if (second) {
      right.tail = tail;
    } else {
      left.tail = tail;
    }

    left.mergeSortByYear();
    right.mergeSortByYear();

    head = merge(left.head, right.head);

    tail = head;
    while (tail && tail->next) {
      tail = tail->next;
    }
  }

  void bubbleSortByYear() {
    if (!head || !head->next) {
      return;
    }

    bool swapped;
    Node *current;
    Node *lastSorted = nullptr;

    do {
      swapped = false;
      current = head;

      // Traverse until the end or the last sorted node
      while (current->next != lastSorted) {
        if (current->date.year > current->next->date.year) {
          // Swap nodes
          Node *tempNext = current->next;

          current->next = tempNext->next;
          tempNext->next = current;

          tempNext->prev = current->prev;
          current->prev = tempNext;

          if (tempNext->prev == nullptr) {
            head = tempNext;
          } else {
            tempNext->prev->next = tempNext;
          }

          if (current->next) {
            current->next->prev = current;
          }

          // Update tail if the swap involves the last node
          if (current == tail) {
            tail = tempNext;
          }

          swapped = true;

          current = tempNext;
        }

        current = current->next;
        if (!current) {
          break;
        }
      }

      lastSorted = current;
    } while (swapped);
  }
};

void printNewsAfterSorting(News trueNews, News fakeNews) {
  while (true) {
    int choice = 0;
    cout << "\nChoose which to print: " << endl;
    cout << "1. True News" << endl;
    cout << "2. Fake News" << endl;
    cout << "3. Quit" << endl;

    cin >> choice;

    if (choice == 1) {
      trueNews.traverse();
    } else if (choice == 2) {
      fakeNews.traverse();
    } else if (choice == 3) {
      break;
    } else {
      cout << "Invalid option, please choose again." << endl;
    }
  }
}

void printNewsPercentage(News *trueNews, News *fakeNews) {
  int filterYear;
  string months[12] = {"January",   "February", "March",    "April",
                       "May",       "June",     "July",     "August",
                       "September", "October",  "November", "December"};
  float percentageOfFakeNews[12] = {};

  cout << "\nWelcome to Percentage of Fake Political News Articles" << endl;
  while (true) {
    cout << "Please enter the year you want to search (Type 0 to quit): ";
    cin >> filterYear;

    if (filterYear == 0) {
      break;
    }

    for (int i = 0; i < 12; i++) {
      int trueCount = trueNews->newsPerMonthByYear(i + 1, filterYear);
      int fakeCount = fakeNews->newsPerMonthByYear(i + 1, filterYear);

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
void sortArticle(News trueNews, News fakeNews) {
  while (true) {
    int choice = 0;
    cout
        << "\nRemember to close the program and open again if you have already "
           "sorted the articles."
        << endl;
    cout << "Sort articles:" << endl;
    cout << "1. Merge Sort" << endl;
    cout << "2. Bubble Sort" << endl;
    cout << "3. Quit" << endl;

    cin >> choice;

    if (choice == 1) {
      cout << "\nSorting true news and fake news now..." << endl;
      auto startSortTrue = chrono::high_resolution_clock::now();
      trueNews.mergeSortByYear();
      auto endSortTrue = chrono::high_resolution_clock::now();

      auto startSortFake = chrono::high_resolution_clock::now();
      fakeNews.mergeSortByYear();
      auto endSortFake = chrono::high_resolution_clock::now();

      auto durationSortingTrue = chrono::duration_cast<chrono::milliseconds>(
          endSortTrue - startSortTrue);
      auto durationSortingFake = chrono::duration_cast<chrono::milliseconds>(
          endSortFake - startSortFake);

      cout << "\nTrue news and fake news sorted." << endl;
      cout << "Time taken to sort true news: " << durationSortingTrue.count()
           << "ms" << endl;
      cout << "Time taken to sort fake news: " << durationSortingFake.count()
           << "ms" << endl;

      printNewsAfterSorting(trueNews, fakeNews);

    } else if (choice == 2) {
      cout << "Sorting true news and fake news now..." << endl;
      auto startSortTrue = chrono::high_resolution_clock::now();
      trueNews.bubbleSortByYear();
      auto endSortTrue = chrono::high_resolution_clock::now();

      auto startSortFake = chrono::high_resolution_clock::now();
      fakeNews.bubbleSortByYear();
      auto endSortFake = chrono::high_resolution_clock::now();

      auto durationSortingTrue = chrono::duration_cast<chrono::milliseconds>(
          endSortTrue - startSortTrue);
      auto durationSortingFake =
        chrono::duration_cast<chrono::milliseconds>(
         endSortFake - startSortFake);

      cout << "\nTrue news and fake news sorted." << endl;
      cout << "Time taken to sort true news: " << durationSortingTrue.count()
           << "ms" << endl;
      cout << "Time taken to sort fake news: " <<
        durationSortingFake.count()
          << "ms" << endl;

      printNewsAfterSorting(trueNews, fakeNews);
    } else if (choice == 3) {
      break;
    } else {
      cout << "Invalid option, please choose again." << endl;
    }
  }
}

struct WordFrequency {
  string word;
  int frequency;
  WordFrequency *next;
  WordFrequency *prev;

  WordFrequency(string w)
      : word(w), frequency(1), next(nullptr), prev(nullptr) {}
};

struct WordFrequencyList {
  WordFrequency *head;
  WordFrequency *tail;
  int count;

  WordFrequencyList() : head(nullptr), tail(nullptr), count(0) {}

  void addOrUpdate(string w) {
    WordFrequency *current = head;
    while (current) {
      if (current->word == w) {
        current->frequency++;
        return;
      }
      current = current->next;
    }
    // Not found, add new node
    WordFrequency *newNode = new WordFrequency(w);
    if (!head) {
      head = tail = newNode;
    } else {
      tail->next = newNode;
      newNode->prev = tail;
      tail = newNode;
    }
    count++;
  }

  // Bubble sort by frequency (descending)
  void bubbleSortByFrequency() {
    if (!head || !head->next)
      return;

    bool swapped;
    WordFrequency *current;
    WordFrequency *lastSorted = nullptr;

    do {
      swapped = false;
      current = head;

      while (current->next != lastSorted) {
        if (current->frequency < current->next->frequency) {
          // Swap nodes
          WordFrequency *tempNext = current->next;

          current->next = tempNext->next;
          tempNext->next = current;

          tempNext->prev = current->prev;
          current->prev = tempNext;

          if (tempNext->prev == nullptr) {
            head = tempNext;
          } else {
            tempNext->prev->next = tempNext;
          }

          if (current->next) {
            current->next->prev = current;
          }

          if (current == tail) {
            tail = tempNext;
          }

          swapped = true;
          current = tempNext;
        }
        current = current->next;
        if (!current)
          break;
      }
      lastSorted = current;
    } while (swapped);
  }

  // Clean up memory
  ~WordFrequencyList() {
    WordFrequency *current = head;
    while (current) {
      WordFrequency *next = current->next;
      delete current;
      current = next;
    }
  }
};

void mostFrequentWord(News *news) {
  WordFrequencyList wordFreqList;
  string w;

  auto start = chrono::high_resolution_clock::now();

  Node *currentNews = news->head;
  while (currentNews != nullptr) {
    if (currentNews->category == "Government News") {
      string text = currentNews->title;
      transform(text.begin(), text.end(), text.begin(), ::tolower);

      // Remove punctuation
      text.erase(remove_if(text.begin(), text.end(),
                           [](char c) { return ispunct(c); }),
                 text.end());

      istringstream ss(text);
      while (ss >> w) {
        wordFreqList.addOrUpdate(w);
      }
    }
    currentNews = currentNews->next;
  }

  wordFreqList.bubbleSortByFrequency();

  // Output top 20
  cout << "\nTop 20 Most Frequent Words in Government topics:\n";
  WordFrequency *current = wordFreqList.head;
  for (int i = 0; i < min(20, wordFreqList.count) && current; i++) {
    cout << i + 1 << ". " << current->word << " (Count: " << current->frequency
         << ")\n";
    current = current->next;
  }

  auto stop = chrono::high_resolution_clock::now();
  cout << "Time Duration: "
       << (chrono::duration_cast<chrono::milliseconds>(stop - start)).count()
       << "ms" << endl;
}

void searchArticle(News *news, bool byYear) {
  int count = 0;
  int year = 0;
  string subject = "";

  if (byYear) {
    cout << "\nEnter year to search: ";
    cin >> year;
  } else {
    cout << "\nEnter subject (eg. politicsNews, worldnews) to search (or "
            "'exit' to quit): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, subject);
    if (subject == "exit")
      return;
    transform(subject.begin(), subject.end(), subject.begin(), ::tolower);
  }

  auto start = chrono::high_resolution_clock::now();
  Node *current = news->head;
  while (current != nullptr) {
    string temp = current->category;
    transform(temp.begin(), temp.end(), temp.begin(), ::tolower);

    if ((byYear && current->date.year == year) ||
        (!byYear && temp == subject)) {
      count++;
      cout << "[line " << count << "] title: " << current->title
           << " | category: " << current->category
           << " | Date: " << current->date.getDate() << endl;
    }
    current = current->next;
  }

  auto stop = chrono::high_resolution_clock::now();
  cout << "Time Duration: "
       << (chrono::duration_cast<chrono::milliseconds>(stop - start)).count()
       << "ms" << endl;
  cout << "Total Articles Found: " << count << endl;
}

void searchArticleMenu(News *trueNews, News *fakeNews) {
  int choice = 0;
  int filter = 0;
  while (true) {
    cout << "\nSearch which dataset: " << endl;
    cout << "1. True News" << endl;
    cout << "2. Fake News" << endl;
    cout << "3. Quit" << endl;

    cin >> choice;

    if (choice == 3) {
      break;
    }

    if (choice != 1 && choice != 2) {
      cout << "Invalid option, please choose again." << endl;
      continue;
    }

    cout << "Please choose one option: " << endl;
    cout << "1. Search Article by Year." << endl;
    cout << "2. Search Article by Subject." << endl;

    cin >> filter;
    if (filter != 1 && filter != 2) {
      cout << "Invalid option, please choose again." << endl;
      continue;
    }

    if (choice == 1) {
      if (filter == 1) {
        searchArticle(trueNews, true);
      } else {
        searchArticle(trueNews, false);
      }

    } else if (choice == 2) {
      if (filter == 1) {
        searchArticle(fakeNews, true);
      } else {
        searchArticle(fakeNews, false);
      }
    }
  }
}

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
    cout << "\nPlease choose one option: " << endl;
    cout << "1. Percentage of fake news pew year." << endl;
    cout << "2. Sort the news article by year." << endl;
    cout << "3. Most frequent words in goverment fake news." << endl;
    cout << "4. Search article." << endl;
    cout << "5. Exit " << endl;
    cin >> choice;

    if (choice == 1) {
      printNewsPercentage(&trueNews, &fakeNews);
    } else if (choice == 2) {
      sortArticle(trueNews, fakeNews);
    } else if (choice == 3) {
      mostFrequentWord(&fakeNews);
    } else if (choice == 4) {
      searchArticleMenu(&trueNews, &fakeNews);
    } else if (choice == 5) {
      break;
    }
  }
}
