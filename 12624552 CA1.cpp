#include <iostream>
#include <string>
using namespace std;

void readInt(int &x, int lo, int hi)
{
    cin >> x;
    while (cin.fail() || x < lo || x > hi)
    {
        cin.clear();
        cin.ignore(100, '\n');
        cout << "Invalid! Enter between " << lo << " and " << hi << ": ";
        cin >> x;
    }
}

void readFloat(float &x, float lo, float hi)
{
    cin >> x;
    while (cin.fail() || x < lo || x > hi)
    {
        cin.clear();
        cin.ignore(100, '\n');
        cout << "Invalid! Enter between " << lo << " and " << hi << ": ";
        cin >> x;
    }
}

class Typing
{
    int id;
    string name;
    int words;
    float mins;
    float acc;
    char grade;
    static int cnt;

public:
    void getData(int id);
    void display();
    void setGrade();
    int getId();
    float getWpm();
    static int getCnt();
    friend void showBest(Typing *p, int n);
};

int Typing::cnt = 0;

inline int Typing::getId()
{
    return id;
}

inline float Typing::getWpm()
{
    return words / mins;
}

int Typing::getCnt()
{
    return cnt;
}

void Typing::getData(int id)
{
    this->id = id;

    cout << "Enter name: ";
    cin.ignore();
    getline(cin, this->name);
    while (this->name.length() == 0)
    {
        cout << "Name cannot be empty. Enter name: ";
        getline(cin, this->name);
    }

    cout << "Enter words typed (1-5000): ";
    readInt(this->words, 1, 5000);

    cout << "Enter time in minutes (0.5-60): ";
    readFloat(this->mins, 0.5, 60);

    cout << "Enter accuracy % (0-100): ";
    readFloat(this->acc, 0, 100);

    setGrade();
    cnt++;
}

void Typing::setGrade()
{
    float w = getWpm();
    if (w >= 60 && acc >= 95)
        grade = 'A';
    else if (w >= 40 && acc >= 90)
        grade = 'B';
    else if (w >= 25 && acc >= 80)
        grade = 'C';
    else
        grade = 'D';
}

void Typing::display()
{
    cout << id << "\t" << name << "\t" << words << "\t" << mins << "\t"
         << getWpm() << "\t" << acc << "\t" << grade << endl;
}

void showBest(Typing *p, int n)
{
    int i, pos = 0;
    float w, best = 0, total = 0;

    if (n == 0)
        cout << "No records yet" << endl;
    else
    {
        for (i = 0; i < n; i++)
        {
            w = (p + i)->words / (p + i)->mins;
            total = total + w;
            if (w > best)
            {
                best = w;
                pos = i;
            }
        }
        cout << "Best typist: " << (p + pos)->name << " (ID " << (p + pos)->id
             << ") with " << best << " WPM" << endl;
        cout << "Average speed: " << total / n << " WPM" << endl;
    }
}

int main()
{
    Typing arr[50];
    Typing *ptr = arr;
    int choice = 0, id = 0, i = 0, n = 0, flag = 0, found = 0;

    do
    {
        cout << "\n--- Typing Speed Practice and Analyser ---" << endl;
        cout << "1. Add record" << endl;
        cout << "2. Display all records" << endl;
        cout << "3. Search by ID" << endl;
        cout << "4. Best typist and average speed" << endl;
        cout << "5. Total records" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter choice: ";
        readInt(choice, 0, 5);

        switch (choice)
        {
        case 1:
            n = Typing::getCnt();
            if (n >= 50)
                cout << "Array full" << endl;
            else
            {
                cout << "Enter ID (1-9999): ";
                readInt(id, 1, 9999);

                flag = 0;
                for (i = 0; i < n; i++)
                {
                    if (arr[i].getId() == id)
                        flag = 1;
                }

                if (flag == 1)
                    cout << "Error: ID already exists!" << endl;
                else
                {
                    arr[n].getData(id);
                    cout << "Record added" << endl;
                }
            }
            break;

        case 2:
            n = Typing::getCnt();
            if (n == 0)
                cout << "No records to show" << endl;
            else
            {
                cout << "ID\tName\tWords\tMins\tWPM\tAcc\tGrade" << endl;
                ptr = arr;
                for (i = 0; i < n; i++)
                {
                    ptr->display();
                    ptr++;
                }
            }
            break;

        case 3:
            n = Typing::getCnt();
            cout << "Enter ID to search: ";
            readInt(id, 1, 9999);
            found = 0;
            ptr = arr;
            for (i = 0; i < n; i++)
            {
                if (ptr->getId() == id)
                {
                    cout << "ID\tName\tWords\tMins\tWPM\tAcc\tGrade" << endl;
                    ptr->display();
                    found = 1;
                }
                ptr++;
            }
            if (found == 0)
                cout << "Record not found" << endl;
            break;

        case 4:
            showBest(arr, Typing::getCnt());
            break;

        case 5:
            cout << "Total records: " << Typing::getCnt() << endl;
            break;

        case 0:
            cout << "Thank you, bye!" << endl;
            break;
        }
    } while (choice != 0);

    return 0;
}
