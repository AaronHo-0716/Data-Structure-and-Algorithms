#include "csv.hpp"
#include "date.hpp"
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace csv;
using namespace dateParsing;
using namespace chrono;

struct NewsArticle {
  string title;
  string content;
  string category;
  int month;
  int year;
  int day;
  Date date;

  NewsArticle(string t, string c, string cat, string d)
      : title(move(t)), content(move(c)), category(move(cat)), date(Date(d)) {
    this->month = date.month;
    this->year = date.year;
    this->day = date.day;
  }

  string printNews() const {
    return title + "," + content + "," + category + ", " +
           const_cast<Date &>(date).getDate();
  }
};

struct Node {
  NewsArticle data;
  Node *next;
  Node *prev;

  Node(NewsArticle d) : data(move(d)), next(nullptr), prev(nullptr) {}
};

class NewsList {
public:
  Node *head;
  Node *tail;
  int size;

  NewsList() : head(nullptr), tail(nullptr), size(0) {}

  // Destructor
  ~NewsList() {
    Node *current = head;
    while (current) {
      Node *next = current->next;
      delete current;
      current = next;
    }
  }

  // Disable copy constructor to prevent double-free
  NewsList(const NewsList &) = delete;
  NewsList &operator=(const NewsList &) = delete;

  NewsList(NewsList &&other) noexcept
      : head(other.head), tail(other.tail), size(other.size) {
    other.head = nullptr;
    other.tail = nullptr;
    other.size = 0;
  }

  NewsList &operator=(NewsList &&other) noexcept {
    if (this != &other) {
      // Clean up existing data
      Node *current = head;
      while (current) {
        Node *n = current->next;
        delete current;
        current = n;
      }
      // Move data from other
      head = other.head;
      tail = other.tail;
      size = other.size;
      // Clear other
      other.head = nullptr;
      other.tail = nullptr;
      other.size = 0;
    }
    return *this;
  }

  void insertAtBack(NewsArticle data) {
    Node *newNode = new Node(move(data));
    if (!head) {
      head = tail = newNode;
    } else {
      tail->next = newNode;
      newNode->prev = tail;
      tail = newNode;
    }
    size++;
  }

  template <typename Predicate> NewsList filter(Predicate predicate) const {
    NewsList result;
    Node *curr = head;
    while (curr) {
      if (predicate(curr->data))
        result.insertAtBack(curr->data);
      curr = curr->next;
    }
    return result;
  }

  template <typename Predicate> int countIf(Predicate predicate) const {
    int count = 0;
    Node *curr = head;
    while (curr) {
      if (predicate(curr->data))
        count++;
      curr = curr->next;
    }
    return count;
  }
};

bool isEarlier(const NewsArticle &a, const NewsArticle &b) {
  return tie(a.year) < tie(b.year);
}

Node *split(Node *head) {
  Node *fast = head, *slow = head;

  while (fast->next && fast->next->next) {
    fast = fast->next->next;
    slow = slow->next;
  }

  Node *temp = slow->next;
  slow->next = nullptr;
  return temp;
}

Node *mergeNodes(Node *first, Node *second) {
  if (!first)
    return second;

  if (!second)
    return first;

  if (isEarlier(first->data, second->data)) {
    first->next = mergeNodes(first->next, second);
    if (first->next)
      first->next->prev = first;
    first->prev = nullptr;
    return first;
  } else {
    second->next = mergeNodes(first, second->next);
    if (second->next)
      second->next->prev = second;
    second->prev = nullptr;
    return second;
  }
}

Node *mergeSortRecursive(Node *head) {
  if (!head || !head->next)
    return head;
  Node *second = split(head);
  return mergeNodes(mergeSortRecursive(head), mergeSortRecursive(second));
}

NewsList functionalMergeSort(const NewsList &original) {
  NewsList sorted;
  Node *curr = original.head;
  while (curr) {
    sorted.insertAtBack(curr->data);
    curr = curr->next;
  }
  if (sorted.head) {
    sorted.head = mergeSortRecursive(sorted.head);
    Node *t = sorted.head;
    while (t && t->next)
      t = t->next;
    sorted.tail = t;
  }
  return sorted;
}

NewsList functionalBubbleSort(const NewsList &original) {
  NewsList sorted;
  Node *curr = original.head;

  while (curr) {
    sorted.insertAtBack(curr->data);
    curr = curr->next;
  }

  if (sorted.size < 2)
    return sorted;

  bool swapped;
  do {
    swapped = false;
    Node *ptr1 = sorted.head;
    while (ptr1 && ptr1->next != nullptr) {
      if (isEarlier(ptr1->data, ptr1->next->data)) {
        swap(ptr1->data, ptr1->next->data);
        swapped = true;
      }
      ptr1 = ptr1->next;
    }
  } while (swapped);

  return sorted;
}

void clearInput(string message = "Invalid Input. Please Try Again.") {
  cout << message << endl;
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string getSubject() {
  string sub;
  while (true) {
    cout << "\nArticle Available: \n"
         << "Middle-east, US_News, left-news, Government News, politics, News, "
            "politicsNews, worldnews, Exit\n"
         << "\nEnter subject to search: ";

    if (!(cin >> ws && getline(cin, sub))) {
      clearInput();
      continue;
    }

    string lowerSub = sub;
    transform(lowerSub.begin(), lowerSub.end(), lowerSub.begin(), ::tolower);

    if (lowerSub == "exit")
      return "";
    return sub;
  }
}

void searchArticle(const NewsList &trueNews, const NewsList &fakeNews) {
  int choice, filterType;

  cout << "\nSearch which dataset: \n1. True News\n2. Fake News\n0. "
          "Quit\nChoice: ";

  if (!(cin >> choice)) {
    clearInput("Invalid input type. Please enter a number.");
    return;
  }

  if (choice == 0)
    return;
  if (choice != 1 && choice != 2) {
    cout << "Invalid option, please choose again." << endl;
    return;
  }

  const NewsList &data = (choice == 1) ? trueNews : fakeNews;
  cout << "1. Search Article by Year.\n2. Search Article by Subject.\nChoice: ";

  if (!(cin >> filterType)) {
    clearInput("Invalid input type.");
    return;
  }

  high_resolution_clock::time_point start;
  NewsList results;

  if (filterType == 1) {
    int year;
    cout << "\nEnter year to search: ";
    if (!(cin >> year)) {
      clearInput("Invalid year input.");
      return;
    }
    start = high_resolution_clock::now();

    results =
        data.filter([year](const NewsArticle &a) { return a.year == year; });
  } else if (filterType == 2) {
    string sub = getSubject();
    if (sub.empty())
      return;

    string lowerSub = sub;
    transform(lowerSub.begin(), lowerSub.end(), lowerSub.begin(), ::tolower);

    start = high_resolution_clock::now();

    results = data.filter([lowerSub](const NewsArticle &a) {
      string cat = a.category;
      transform(cat.begin(), cat.end(), cat.begin(), ::tolower);
      return cat == lowerSub;
    });
  } else {
    cout << "Invalid option." << endl;
    return;
  }

  int i = 0;
  Node *curr = results.head;
  while (curr) {
    cout << "[line " << ++i << "] title: " << curr->data.title
         << " | category: " << curr->data.category
         << " | Date: " << const_cast<Date &>(curr->data.date).getDate()
         << endl;
    curr = curr->next;
  }

  auto stop = high_resolution_clock::now();
  cout << "Time Duration: " << duration_cast<milliseconds>(stop - start).count()
       << "ms" << endl;
  cout << "Total Articles Found: " << results.size << endl;
}

float calculateMonthPercentage(const NewsList &trueN, const NewsList &fakeN,
                               int m, int y) {
  auto isPolitical = [m, y](const NewsArticle &a) {
    return a.month == m && a.year == y &&
           (a.category == "politics" || a.category == "politicsNews");
  };

  int tCount = trueN.countIf(isPolitical);
  int fCount = fakeN.countIf(isPolitical);

  return (tCount + fCount == 0)
             ? 0.0f
             : (static_cast<float>(fCount) / (tCount + fCount)) * 100.0f;
}

void printNewsPercentage(const NewsList &trueNews, const NewsList &fakeNews) {
  int filterYear;
  string months[12] = {"January",   "February", "March",    "April",
                       "May",       "June",     "July",     "August",
                       "September", "October",  "November", "December"};

  cout << "\nWelcome to Percentage of Fake Political News Articles" << endl;

  while (true) {
    cout << "Please enter the year you want to search (Type 0 to quit): ";

    if (!(cin >> filterYear)) {
      clearInput("Invalid input. Please enter a numeric year.");
      continue;
    }

    if (filterYear == 0)
      break;

    cout << "\nPercentage of Fake Political News Articles in " << filterYear
         << "\n"
         << string(70, '-') << endl;
    cout << left << setw(12) << "Month" << " | " << setw(40) << "Graph" << " | "
         << "Percentage\n"
         << string(70, '-') << endl;

    for (int i = 0; i < 12; i++) {
      float p = calculateMonthPercentage(trueNews, fakeNews, i + 1, filterYear);
      int stars = static_cast<int>(p);

      cout << left << setw(12) << months[i] << " | ";
      cout << string(stars, '*') << setw(40 - stars) << "" << " | " << fixed
           << setprecision(2) << setw(6) << p << "%" << endl;
    }
  }
}

void mostFrequentWord(const NewsList &fakeNews) {
  auto start = high_resolution_clock::now();
  NewsList govNews = fakeNews.filter(
      [](const NewsArticle &a) { return a.category == "Government News"; });

  map<string, int> counts;
  Node *curr = govNews.head;

  while (curr) {
    stringstream ss(curr->data.title);
    string w;
    while (ss >> w) {
      transform(w.begin(), w.end(), w.begin(), ::tolower);
      w.erase(remove_if(w.begin(), w.end(), ::ispunct), w.end());
      if (!w.empty())
        counts[w]++;
    }
    curr = curr->next;
  }

  vector<pair<string, int>> freqList(counts.begin(), counts.end());
  sort(freqList.begin(), freqList.end(),
       [](auto &a, auto &b) { return a.second > b.second; });

  cout << "\nTop 20 Most Frequent Words in Government topics:\n";
  cout << left << setw(4) << "No." << setw(15) << "Words" << "Count" << endl;

  for (int i = 0; i < min(20, (int)freqList.size()); i++)
    cout << left << setw(4) << (i + 1) << setw(15) << freqList[i].first
         << freqList[i].second << endl;

  cout << "\nTime Duration: "
       << duration_cast<milliseconds>(high_resolution_clock::now() - start)
              .count()
       << "ms\n";
}

NewsList sortList(bool isMergeSort, const NewsList &news,
                  const string newsType) {
  auto start = high_resolution_clock::now();
  NewsList sort =
      (isMergeSort) ? functionalMergeSort(news) : functionalBubbleSort(news);
  auto end = high_resolution_clock::now();

  cout << "Time taken to sort " << newsType
       << " news: " << duration_cast<milliseconds>(end - start).count() << "ms"
       << endl;

  return sort;
}

void sortArticle(const NewsList &trueNews, const NewsList &fakeNews) {
  int choice = 0;
  cout << "\nSort articles:\n1. Merge Sort\n2. Bubble Sort\n0. Quit\nChoice: ";
  if (!(cin >> choice)) {
    clearInput();
    return;
  }

  if (choice == 0)
    return;
  if (choice != 1 && choice != 2) {
    cout << "Invalid option." << endl;
    return;
  }

  cout << "\nSorting true news and fake news now..." << endl;

  NewsList sortedTrue = sortList(choice == 1, trueNews, "true");
  NewsList sortedFake = sortList(choice == 1, fakeNews, "fake");

  while (true) {
    int printChoice;
    cout << "\nChoose which to print: \n1. True News\n2. Fake News\n0. "
            "Back\nChoice: ";

    if (!(cin >> printChoice)) {
      clearInput();
      continue;
    }

    if (printChoice == 0)
      break;

    Node *curr = (printChoice == 1)
                     ? sortedTrue.head
                     : (printChoice == 2 ? sortedFake.head : nullptr);

    if (!curr) {
      cout << "Invalid selection." << endl;
      continue;
    }

    while (curr) {
      cout << curr->data.printNews() << endl;
      curr = curr->next;
    }
  }
}

NewsList readFile(string filename) {
  try {
    auto start = high_resolution_clock::now();
    CSVReader reader(filename);
    NewsList articles;

    for (auto &row : reader)
      articles.insertAtBack({row[0].get(), row[1].get(), row[2].get(), row[3].get()});

    cout << "Number of " << filename.substr(0, 4)
         << " news loaded: " << articles.size << endl;

    auto duration =
        duration_cast<milliseconds>(high_resolution_clock::now() - start)
            .count();

    cout << "Time spent loading " << filename.substr(0, 4)
         << " news: " << duration << "ms" << endl;
    return articles;

  } catch (const exception &e) {
    cerr << "Error reading file " << filename << ": " << e.what() << endl;
    return NewsList();
  }
}

int main() {
  NewsList trueNews = readFile("true.csv");
  NewsList fakeNews = readFile("fake.csv");

  if (trueNews.size == 0 && fakeNews.size == 0) {
    cerr << "Failed to load datasets. Exiting." << endl;
    return 1;
  }

  while (true) {
    int choice = 0;
    cout << "\n--- Main Menu ---\n";
    cout << "1. Percentage of fake news per year\n";
    cout << "2. Sort articles by year\n";
    cout << "3. Most frequent words in government fake news\n";
    cout << "4. Search articles\n";
    cout << "0. Exit\n";
    cout << "Choice: ";

    if (!(cin >> choice)) {
      clearInput("Invalid input. Please enter a number between 1 and 5.");
      continue;
    }

    if (choice == 1)
      printNewsPercentage(trueNews, fakeNews);
    else if (choice == 2)
      sortArticle(trueNews, fakeNews);
    else if (choice == 3)
      mostFrequentWord(fakeNews);
    else if (choice == 4)
      searchArticle(trueNews, fakeNews);
    else if (choice == 0)
      break;
    else
      cout << "Invalid choice." << endl;
  }
  return 0;
}
