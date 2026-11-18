#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
using namespace std;

class Practice {
    int id;
    char name[30];
    char level[10];
    int words;
    int chars;
    int seconds;
    int wrong;
    float wpm;
    float accuracy;
    char grade[10];

public:
    static int total;


    Practice() {
        id = 0;
        strcpy(name, "none");
        strcpy(level, "none");
        words = 0;
        chars = 0;
        seconds = 0;
        wrong = 0;
        wpm = 0;
        accuracy = 0;
        strcpy(grade, "none");
    }


    Practice(int i, const char n[], const char l[], int w, int c, int s, int wr) {
        id = i;
        strcpy(name, n);
        strcpy(level, l);
        setData(w, c, s, wr);
    }


    Practice(const Practice &p) {
        id = p.id;
        strcpy(name, p.name);
        strcpy(level, p.level);
        words = p.words;
        chars = p.chars;
        seconds = p.seconds;
        wrong = p.wrong;
        wpm = p.wpm;
        accuracy = p.accuracy;
        strcpy(grade, p.grade);
    }

    ~Practice() {

    }

    void setData(int w, int c, int s, int wr) {
        words = w;
        chars = c;
        seconds = s;
        wrong = wr;
        if (wrong > chars)
            wrong = chars;
        wpm = 0;
        accuracy = 0;
        if (seconds > 0)
            wpm = words * 60.0 / seconds;
        if (chars > 0)
            accuracy = (chars - wrong) * 100.0 / chars;

        if (wpm >= 40 && accuracy >= 95)
            strcpy(grade, "Good");
        else if (wpm >= 25 && accuracy >= 85)
            strcpy(grade, "Average");
        else
            strcpy(grade, "Slow");
    }

    int getId() {
        return id;
    }

    char *getLevel() {
        return level;
    }

    inline void display() {
        cout << id << "\t" << name << "\t" << level << "\t" << words << "\t"
             << seconds << "\t" << wrong << "\t" << wpm << "\t" << accuracy
             << "\t" << grade << "\n";
    }
};

int Practice::total = 0;

void typeTest(const char level[], int &words, int &chars, int &seconds, int &wrong) {
    char text[200];
    char typed[200];
    int i, n;

    if (strcmp(level, "Easy") == 0)
        strcpy(text, "the quick brown fox jumps over the lazy dog");
    else if (strcmp(level, "Medium") == 0)
        strcpy(text, "Practice makes a person perfect, so type a little every single day.");
    else
        strcpy(text, "C++ uses classes, objects & pointers; always close your files (fstream) after use!");

    words = 1;
    for (i = 0; text[i] != '\0'; i++)
        if (text[i] == ' ')
            words++;
    chars = strlen(text);

    cin.ignore();
    cout << "\nType this sentence exactly:\n\n" << text << "\n\n";
    cout << "Press Enter to start, then type and press Enter again.\n";
    cin.getline(typed, 200);

    time_t start = time(0);
    cin.getline(typed, 200);
    time_t end = time(0);

    seconds = end - start;
    if (seconds < 1)
        seconds = 1;


    wrong = 0;
    n = strlen(typed);
    for (i = 0; i < chars && i < n; i++)
        if (typed[i] != text[i])
            wrong++;
    if (n > chars)
        wrong = wrong + (n - chars);
    else
        wrong = wrong + (chars - n);
}

int countRecords() {
    fstream file("./data/practice.dat", ios::in | ios::binary);
    if (!file)
        return 0;
    file.seekg(0, ios::end);
    int size = file.tellg();
    file.close();
    return size / sizeof(Practice);
}

void createRecord() {
    char name[30];
    char level[10];
    int lv, words, chars, seconds, wrong;

    cin.ignore();
    cout << "Enter user name: ";
    cin.getline(name, 30);
    cout << "Level (1 Easy, 2 Medium, 3 Hard): ";
    cin >> lv;
    if (lv == 1)
        strcpy(level, "Easy");
    else if (lv == 2)
        strcpy(level, "Medium");
    else
        strcpy(level, "Hard");

    typeTest(level, words, chars, seconds, wrong);

    Practice p(Practice::total + 1, name, level, words, chars, seconds, wrong);
    Practice backup = p;

    fstream file("./data/practice.dat", ios::out | ios::binary | ios::app);
    if (!file) {
        cout << "Cannot open file. Make a folder named data first.\n";
        return;
    }
    file.write((char *)&backup, sizeof(backup));
    file.close();

    Practice::total++;
    cout << "\nResult saved with ID " << backup.getId() << "\n";
    cout << "ID\tName\tLevel\tWords\tSec\tWrong\tWPM\tAcc%\tGrade\n";
    backup.display();
}

void readRecords() {
    Practice p;
    fstream file("./data/practice.dat", ios::in | ios::binary);
    if (!file) {
        cout << "No records yet.\n";
        return;
    }
    cout << "ID\tName\tLevel\tWords\tSec\tWrong\tWPM\tAcc%\tGrade\n";
    while (file.read((char *)&p, sizeof(p))) {
        p.display();
    }
    file.close();
}

void searchRecord() {
    int id;
    Practice p;
    cout << "Enter ID to search: ";
    cin >> id;

    if (id < 1 || id > Practice::total) {
        cout << "Record not found.\n";
        return;
    }

    fstream file("./data/practice.dat", ios::in | ios::binary);
    file.seekg((id - 1) * sizeof(p), ios::beg);
    file.read((char *)&p, sizeof(p));
    file.close();

    cout << "ID\tName\tLevel\tWords\tSec\tWrong\tWPM\tAcc%\tGrade\n";
    p.display();
}

void updateRecord() {
    int id, words, chars, seconds, wrong;
    Practice p;
    cout << "Enter ID to update (retake the test): ";
    cin >> id;

    if (id < 1 || id > Practice::total) {
        cout << "Record not found.\n";
        return;
    }

    fstream file("./data/practice.dat", ios::in | ios::out | ios::binary);
    file.seekg((id - 1) * sizeof(p), ios::beg);
    file.read((char *)&p, sizeof(p));
    cout << "Old record:\n";
    p.display();

    typeTest(p.getLevel(), words, chars, seconds, wrong);
    p.setData(words, chars, seconds, wrong);

    file.seekp((id - 1) * sizeof(p), ios::beg);
    file.write((char *)&p, sizeof(p));
    file.close();

    cout << "\nRecord updated. New record:\n";
    p.display();
}

int main() {
    int choice;
    Practice::total = countRecords();

    do {
        cout << "\n--- Typing Speed Practice and Analyser ---\n";
        cout << "Records in file: " << Practice::total << "\n";
        cout << "1. Create (new practice)\n2. Read all\n3. Search\n4. Update\n5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1: createRecord(); break;
        case 2: readRecords(); break;
        case 3: searchRecord(); break;
        case 4: updateRecord(); break;
        case 5: cout << "Bye\n"; break;
        default: cout << "Wrong choice\n";
        }
    } while (choice != 5);

    return 0;
}
