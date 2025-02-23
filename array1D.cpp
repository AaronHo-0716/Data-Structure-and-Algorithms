// <GroupNo>_<TeamLeaderID>_<Member1ID>_<Member2ID>_<Member3ID>.zip
// Use APA style citations within your code as comments and also mention them in your report.
// cite the code from outside

// C++ program to remove white spaces
// from string using the function
// boost::trim function

#include "csv.hpp"
#include "date.hpp"
#include <iostream>
#include <chrono>
#include <variant>
#include <algorithm>
#include <iomanip>
#include <sstream>
// #include <string>

using namespace std;
using namespace csv;
using namespace dateParsing;
using namespace chrono;

struct myArray {
    private:  
        Date parseDate(string d) {
            return (Date(d));
        }

        string trim(string str) {
            str.erase(0, str.find_first_not_of(" \t\n\r"));
            str.erase(str.find_last_not_of(" \t\n\r") + 1);  
            return str;
        }

    public: 
        string title, text, category;
        Date publicationDate;

        myArray() : title(""), text(""), category(""), publicationDate(Date("0000-00-00")) {}

        myArray(string c0, string c1, string c2, string c3)
            : title(c0), text(c1), category(trim(c2)), publicationDate(parseDate(c3)) {}

        using ReturnType = variant<string, Date>;
        ReturnType operator[](int idx) const { 
            switch (idx) {
                case 0: return title; break;
                case 1: return text; break;
                case 2: return category; break;
                case 3: return publicationDate; break; 
                default: throw out_of_range("Index out of range");
            }
        }
};

struct WordFrequency {
    string word;
    int frequency;

    WordFrequency() : word(""), frequency(0) {}

    WordFrequency(string word, int f)
        : word(word), frequency(f) {}

    using ReturnType = variant<string, int>;
    ReturnType operator[](int idx) const { 
        switch (idx) {
            case 0: return word; break;
            case 1: return frequency; break;            
            default: throw out_of_range("Index out of range");
        }
    }
};

myArray* readFile(string filename, int &rowCount) {
    CSVReader reader(filename);
    rowCount = 0;

    myArray* file = new myArray[23500]; 

    if (!file) {
        cerr << "Memory allocation failed!" << endl;
        return nullptr;
    }

    auto start = high_resolution_clock::now();
    for (CSVRow &row : reader) {
        string data[4];
        int colIdx = 0;

        for (CSVField &field : row) {
            data[colIdx] = field.get();
            colIdx++;
        }

        file[rowCount] = myArray(data[0], data[1], data[2], data[3]);
        rowCount++;
    }

    cout << "Total " << ((filename == "true.csv") ? "True" : "Fake") << " Articles: " << rowCount + 1 << endl;
    auto stop = high_resolution_clock::now();
    cout << "Time Duration: " << (duration_cast<milliseconds>(stop - start)).count() << "ms" << endl;
    // cout << "Total Fake Articles: " << fakeNrowCount << endl; // 23481
    // cout << "Total True Articles: " << trueNrowCount << endl; // 21417

    return file;
}

void readArray(myArray* file, int rowCount) {
    string data;
    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < 4; j++) {
            if (j != 1) {
                data = (j == 3) ? get<Date>(file[i][j]).getDate() : get<string>(file[i][j]);
                cout << "line " << i + 1 << " col " << j << ": " << data << endl;
            }
        }
    }
}

// merge Sort
bool compareWordFrequency(const WordFrequency& a, const WordFrequency& b) {
    return a.frequency > b.frequency; // Descending order
}

bool compareMyArray(const myArray& a, const myArray& b) {
    return tie(a.publicationDate.year, a.publicationDate.month, a.publicationDate.day) < tie(b.publicationDate.year, b.publicationDate.month, b.publicationDate.day);
}

template <typename T, typename Compare>
void merge(T* file, int left, int mid, int right, Compare comp) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    T* Ltemp = new T[n1];
    T* Rtemp = new T[n2];

    for (int i = 0; i < n1; i++) Ltemp[i] = file[left + i];
    for (int i = 0; i < n2; i++) Rtemp[i] = file[mid + 1 + i];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        file[k++] = (comp(Ltemp[i], Rtemp[j])) ? Ltemp[i++] : Rtemp[j++];
    }

    while (i < n1) file[k++] = Ltemp[i++];
    while (j < n2) file[k++] = Rtemp[j++];

    delete[] Ltemp;
    delete[] Rtemp;
}

// **mergeSort function using a template**
template <typename T, typename Compare>
void mergeSort(T* file, int left, int right, Compare comp) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(file, left, mid, comp);
        mergeSort(file, mid + 1, right, comp);
        merge(file, left, mid, right, comp);
    }
}

// bubble sort
template <typename Compare>
void bubbleSort(myArray* file, int rowCount, Compare comp) {
    bool swapped;

    for (int i = 0; i < (rowCount - 1); i++) {
        swapped = false;
        for (int j = 0; j < (rowCount - 1 - i); j++) {
            if (comp(file[j + 1], file[j])) {
                swap(file[j], file[j + 1]);
                swapped = true;
            }
        }
        // If no two elements were swapped, then break
        if (!swapped)
            break;
    }
}

template <typename Compare>
int partition(myArray* file, int low, int high, Compare comp) {

    // Selecting last element as the pivot
    myArray pivot = file[high];

    // Index of elemment just before the last element
    // It is used for swapping
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {

        // If current element is smaller than or
        // equal to pivot
        if (comp(file[j], pivot)) {
            i++;
            swap(file[i], file[j]);
        }
    }

    // Put pivot to its position
    swap(file[i + 1], file[high]);

    // Return the point of partition
    return (i + 1);
}

template <typename Compare>
void quickSort(myArray* file, int low, int high, Compare comp) {
    if (low < high) {

        // pi is Partitioning Index, arr[p] is now at
        // right place
        int pi = partition(file, low, high, comp);

        // Separately sort elements before and after the
        // Partition Index pi
        quickSort(file, low, pi - 1, comp);
        quickSort(file, pi + 1, high, comp);
    }
}

// percentage
int counter(myArray* file, int rowCount, int year, int month) {
    int count = 0;
    for (int i = 0; i < rowCount; i++) {
        if (file[i].publicationDate.year == year && file[i].publicationDate.month == month && 
            (file[i].category == "politicsNews" || file[i].category == "politics") ) {
            count++;
        }
    }
    return count;
}

// int counter(myArray* file, int rowCount, int year) {
//     int count = 0;
//     for (int i = 0; i < rowCount; i++) {
//         if (file[i].publicationDate.year == year && 
//             (file[i].category == "politicsNews" || file[i].category == "politics")) {
//             count++;
//         }
//     }
//     return count;
// }

int getYearInput() {
    int input;
    while (true) {
        cout << "Enter year: ";
        if (!(cin >> input) || input < 2015 || input > 2018) {
            cout << "Invalid input. Please enter a valid year (2015 to 2018)." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        return input;
    }  
}

// Print percentage of fake news in graphical format
void printPercentageGraphic(myArray* trueN, myArray* fakeN, int trueRow, int fakeRow, int input) {
    float eachmonth[12], total = 0;

    string reverseMonthMap[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

    // int fYear = counter(fakeN, fakeRow, input);
    // int tYear = counter(trueN, trueRow, input);

    for (int i = 0; i < 12; i++) {
        // int f = counter(fakeN, fakeRow, input, i + 1);  
        // eachmonth[i] = f / static_cast<float>(fYear + tYear) * 100;
        int f = counter(fakeN, fakeRow, input, i + 1);  
        int t = counter(trueN, trueRow, input, i + 1);  
        eachmonth[i] = (f == 0 || t == 0) ? 0 : f / static_cast<float>(f + t) * 100;

        total += eachmonth[i];
    }

    cout << "\nPercentage of Fake Politics News Articles in " << input << endl;
    for (int i = 0; i < 12; i++) {
        cout << left;
        cout << setw(12) << reverseMonthMap[i] << ": ";  

        cout << setw(2) << "" ;
        for (int j = 0; j < static_cast<int>(eachmonth[i]); j++) 
            cout << "*";

        cout << setw(110 - static_cast<int>(eachmonth[i])) << ""  
            << right << setw(6)  
            << fixed << setprecision(2) << eachmonth[i] << "%" << endl;
    }
    cout << "Total Percentage of Fake Politics News Articles in Year " << input << " is " << total / 12 << "%." << endl;
    cout << "Note: Each '*' represents 1% of fake news.\n";
}

// Menu for selecting news percentage filtering options
void NewsPercentageMenu(myArray* trueN, myArray* fakeN, int trueRow, int fakeRow) {
    int input = -1, year = 2015;

    while (true) {
        cout << "\nDo you want to apply any filter?\n"
                << "1. Year\n"
                << "2. No Filter (All Years)\n"
                << "0. Exit\n"
                << "Please enter yuor choice: ";

        if (!(cin >> input) || input < 0 || input > 2) {
            cout << "Invalid input. Please enter a valid option (0, 1, or 2).\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (input) {
            case 1:
                printPercentageGraphic(trueN, fakeN, trueRow, fakeRow, getYearInput());
                break;
            case 2:
                for (int i = 0; i < 4; i++) 
                    printPercentageGraphic(trueN, fakeN, trueRow, fakeRow, year++);
                break;
            default:
                return;  // Exit the function if 0 is entered
        }
        break;
    }
}


// most frequency
void mostFrequentWord(myArray* file, int rowCount) {
    const int size = 50000;
    WordFrequency* wordFreq = new WordFrequency[size];
    int uniqueCount = 0;
    string w;

    auto start = high_resolution_clock::now();

    for (int i = 0; i < rowCount; i++) {
        if (file[i].category == "Government News") {
            string text = file[i].title;
            transform(text.begin(), text.end(), text.begin(), ::tolower); 

            // Remove punctuation
            text.erase(remove_if(text.begin(), text.end(), [](char c) {
                return ispunct(c);
            }), text.end());

            istringstream ss(text);
    
            while (ss >> w) {
                bool found = false;
                for (int j = 0; j < uniqueCount; j++) {
                    if (w == wordFreq[j].word) {
                        wordFreq[j].frequency++;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    wordFreq[uniqueCount].word = w;
                    wordFreq[uniqueCount].frequency = 1;
                    uniqueCount++;
                }
            }
        }
    }

    mergeSort(wordFreq, 0, uniqueCount - 1, compareWordFrequency); 

    cout << "\nTop 20 Most Frequent Words in Government topics:\n";
    for (int i = 0; i < min(20, uniqueCount); i++) {
        cout << i + 1 << ". " << wordFreq[i].word << " (Count: " << wordFreq[i].frequency << ")\n";
    }

    auto stop = high_resolution_clock::now();
    cout << "Time Duration: " << (duration_cast<milliseconds>(stop - start)).count() << "ms" << endl;

    delete[] wordFreq;
}

char fakeORtrue() {
    while (1) {
        int choice = 0;
        cout << "\nYou want to serach/sort in\n" 
            << "1. Fake Mews\n"
            << "2. True News\n"
            << "3. Exit\n"
            << "Please enter: ";

        if (!(cin >> choice) || choice < 1 || choice > 3) {
            cout << "Invalid input. Please enter a valid number from 1 - 3." << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            continue;
        }

        switch (choice) {
            case 1: return 'F';
            case 2: return 'T';
            default: return '-';
        }
    }
}

bool promoptPrint() {
    int choice = 0;
    while (1) {
        cout << "\nDo you want to print the sorted article\n"
            << "1. Yes\n"
            << "2. No\n"
            << "Please enter: ";

        if (!(cin >> choice) || choice < 1 || choice > 2) {
            cout << "Invalid input. Please enter a valid number from 1 - 2." << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            continue;
        }
        return (choice == 1) ? true : false;
    }
}

void sortArticle(myArray* file, int rowCount) {
    while(1) {
        int choice = 0;
        cout << "\nSort articles:" << endl;
        cout << "1. Merge Sort" << endl;
        cout << "2. Bubble Sort" << endl;
        cout << "3. Quick Sort" << endl;
        cout << "4. Without Sorting" << endl;
        cout << "5. Exit" << endl;
        cout << "Please enter your choice: ";

        if (!(cin >> choice) || choice < 1 || choice > 5) {
            cout << "Invalid input. Please enter a valid number from 1 - 5." << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            continue;
        }
        myArray* temp = new myArray[rowCount];
        for (int i = 0; i < rowCount; i++) temp[i] = file[i];

        auto start = high_resolution_clock::now();
        switch (choice) {
            case 1: mergeSort(temp, 0, rowCount - 1, compareMyArray); break;
            case 2: bubbleSort(temp, rowCount, compareMyArray); break; 
            case 3: quickSort(temp, 0, rowCount - 1, compareMyArray); break; 
            default: break;
        }
        if (choice != 5) {
            auto stop = high_resolution_clock::now();
            cout << "Time Duration: " << (duration_cast<milliseconds>(stop - start)).count() << "ms" << endl;
            if (promoptPrint()) readArray(temp, rowCount);
        }
        delete[] temp;
        break;
    }
}

string getInput() {
    string input;

    while (true) {
        cout << "\nArticle Available: \n"
            << "Middle-east\n"
            << "US_News\n"
            << "left-news\n" 
            << "Government News\n"
            << "politics\n" 
            << "News\n" 
            << "politicsNews\n"
            << "worldnews\n"
            << "Exit\n"
            << "Enter article subject: ";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore leftover input

        if (!getline(cin, input) || input.empty()) {  // Checks if input is empty
            cout << "Invalid input. Please enter a valid subject.\n";
            continue;
        }
        transform(input.begin(), input.end(), input.begin(), ::tolower); 
        return input;
    }
}

void searchArticle(myArray* file, int rowCount, bool byYear) {
    int count = 0;
    int year = 2000;
    string subject = "";

    if (byYear) {
        year = getYearInput();
    } else {
        subject = getInput();
        if (subject == "exit") return;
        transform(subject.begin(), subject.end(), subject.begin(), ::tolower); 
    }

    auto start = high_resolution_clock::now();
    for (int i = 0; i < rowCount; i++) {
        string temp = file[i].category;
        transform(temp.begin(), temp.end(), temp.begin(), ::tolower); 

        if (file[i].publicationDate.year == year || temp == subject) {
            count++;
            cout << "[line " << count << "] title: " << file[i].title << " | category: " << file[i].category << " | Date: " << (file[i].publicationDate).getDate() << endl;
        }
    }
    auto stop = high_resolution_clock::now();
    cout << "Time Duration: " << (duration_cast<milliseconds>(stop - start)).count() << "ms" << endl;
    cout << "Total Article Found: " << count << endl;
}

// 6.	Search the articles based on specific criteria, such as category, publication year
void searchArticleMenu(myArray* file, int rowCount) {
    while (1) {
        int choice = 0;
        cout << "\nPlease choose one option: " << endl;
        cout << "1. Search Article by Year." << endl;
        cout << "2. Search Article by Subject." << endl;
        cout << "3. Exit" << endl;
        cout << ": ";

        if (!(cin >> choice) || choice < 1 || choice > 3) {
            cout << "Invalid input. Please enter a valid number from 1 - 3." << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            continue;
        }

        switch (choice) {
            case 1: searchArticle(file, rowCount, true); break;
            case 2: searchArticle(file, rowCount, false); break; // bubble sort
            default: break;
        }
        
        break;
    }
}

void menu(myArray* trueN, myArray* fakeN, int trueRow, int fakeRow) {
    char fORt;
    while (1) {
        int choice = 0;
        cout << "\nPlease choose one option: " << endl;
        cout << "1. Percentage of fake news per year." << endl;
        cout << "2. Sort the news article by year." << endl;
        cout << "3. Most frequent words in goverment fake news." << endl;
        cout << "4. Search article." << endl;
        cout << "5. Exit" << endl;
        cout << ": ";

        if (!(cin >> choice) || choice < 1 || choice > 5) {
            cout << "Invalid input. Please enter a valid number from 1 - 5." << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            continue;
        }

        switch (choice) {
            case 1: NewsPercentageMenu(trueN, fakeN, trueRow, fakeRow); break;
            case 2: 
                fORt = fakeORtrue();
                if (fORt == 'F') sortArticle(fakeN, fakeRow); 
                else if (fORt == 'T') sortArticle(trueN, trueRow); 
                break;
            case 3: mostFrequentWord(fakeN, fakeRow); break;
            case 4: 
                fORt = fakeORtrue();
                if (fORt == 'F') searchArticleMenu(fakeN, fakeRow); 
                else if (fORt == 'T') searchArticleMenu(trueN, trueRow); 
                break;
            default: break;
        }

        if (choice == 5) break;
    }
}


int main() {
    int trueNrowCount, fakeNrowCount;
    
    auto start = high_resolution_clock::now();
    myArray* fakeN = readFile("fake.csv", fakeNrowCount);
    myArray* trueN = readFile("true.csv", trueNrowCount);
    auto stop = high_resolution_clock::now();

    menu(trueN, fakeN, trueNrowCount, fakeNrowCount);

    delete[] trueN; // Free allocated memory
    delete[] fakeN; // Free allocated memory

    return 0;
}