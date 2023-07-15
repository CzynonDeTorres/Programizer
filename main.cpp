#include <iostream>
#include <fstream> // file stream
#include <string> // string
#include <sstream>
#include <stdio.h> // scanf
#include <algorithm> // find
#include <queue> // queue

using namespace std;

string current_user, current_pass;
int current_user_id;

// nodes
struct user{
    int id;
    string name;
    string pass;
    string line_detail;
    user* next;
};

struct todo{
    int userid;
    string name;
    string date_dl;
    string time_dl; // current available time method is a bit confusing so we just have to suffer
    int priority;
        // 1 - most important
        // 2 - mid
        // 3 - least important
    bool status; // done or not done
    string category;
    todo* next;
};

// node initialization
user *userhead = NULL;
todo *todohead = NULL;

void add_user(string user_input, string pass_input){
    struct user *new_node = new user;

    if(userhead==NULL){
        new_node->id = 0;
        new_node->name = user_input;
        new_node->pass = pass_input;
        new_node->next = userhead;
        userhead = new_node;
    }
    else{
        new_node->id = userhead->id + 1;
        new_node->name = user_input;
        new_node->pass = pass_input;
        new_node->next = userhead;
        userhead = new_node;
    }

    ofstream user_file("database/users.txt", ios::app);
    user_file << userhead->id << "," << user_input << "," << pass_input << endl;
    user_file.close();

}

void insert_user(int n, string user_input, string pass_input){
    struct user *new_node = new user;
    new_node->id = n;
    new_node->name = user_input;
    new_node->pass = pass_input;
    new_node->next = userhead;
    userhead = new_node;
}

void open_user_list(){
    ifstream user_file;
    string line,deets,user,pass;
    int id;
    user_file.open("database/users.txt");
    if (user_file.is_open()){
        while (getline(user_file, line)){
            stringstream refline(line);
            int i=0;
            while(getline(refline, deets, ',')){
                if(i==0)
                    id=stoi(deets);
                else if(i==1)
                    user=deets;
                else if(i==2)
                    pass=deets;
                i++;
            }
            insert_user(id,user,pass);
        }
    }
    else ofstream user_file("database/users.txt");

    user_file.close();
}

bool search_user(struct user* head, string name){
    if (head == NULL) return false;
    if (head->name == name) {
        current_user_id = head->id;
        current_user = head->name;
        current_pass = head->pass;
        return true;
    }

    return search_user(head->next, name);

}

void view_td(){
    todo* temp = todohead;

    if (temp == nullptr) {
        cout << "List is empty." << endl;
        return;
    }

    cout << "List: ";
    while (temp != nullptr) {
        cout << temp->name << " "
             << temp->date_dl << " "
             << temp->time_dl << " "
             << temp->status << " "
             << temp->priority << " "
             << temp->category << " ";
        temp = temp->next;
    }
    cout << endl;

}

void show_categories(queue<string> q) {
    while (!q.empty()) {
        cout << q.front() << endl;
        q.pop();
    }
  cout << endl;
}

string categoriesList()
{
    int chooseCateg;
    string userTaskCateg;
    bool loop_initialzer = true;
    string newCateg;
    queue<string> categories;
    categories.push("Work"); // defaults
    categories.push("Personal");
    categories.push("Fitness and Health");
    categories.push("Academics");
    categories.push("Music");

    do
    {
        cout << "\n   1: Choose Task Category\n   2: Add New\n   Enter Here: ";
        cin >> chooseCateg;

        switch (chooseCateg)
        {
        case 1:
            system("cls");
            cout << "\nTask Categories:\n";
            show_categories(categories);
            cout << "\n\n   Enter Task Category: ";
            cin >> userTaskCateg;
            {
                queue<string> tempQueue = categories; // Create a temporary queue for searching
                bool categoryFound = false;
                while (!tempQueue.empty())
                {
                    string existingCateg = tempQueue.front();
                    tempQueue.pop();
                    if (existingCateg == userTaskCateg)
                    {
                        categoryFound = true;
                        break;
                    }
                }
                if (categoryFound)
                {
                    return userTaskCateg;
                }
                else
                {
                    cout << "Category Not Found!\n";
                }
            }
            break;

        case 2:
            cout << "\nNew Category: ";
            cin >> newCateg;
            categories.push(newCateg);
            break;
        }
    } while (loop_initialzer);
}

int priorityLevel()
{
    int priorityTask;
    bool task_loop = true;
    do
    {
        cout << "\n\tChoose Priority Level: \n"
             << "\t  1: Critical/Important\n"
             << "\t  2: Not Priority Today\n"
             << "\t  3: Low Importance\n"
             << "\n\t   Enter Here: ";

        cin >> priorityTask;

        if (priorityTask != 1 && priorityTask != 2 && priorityTask != 3)
        {
            system("cls");
            cout << "Invalid Input";
        }

        else
        {
            task_loop = false;
            switch (priorityTask)
            {
            case 1:
                return 1; // feel free to modify the terms
                break;

            case 2:
                return 2;
                break;

            case 3:
                return 3;
                break;
            }
        }

    } while (task_loop);
}

bool verify_date(string wholedate){
    int month, date, year;
    int thirty[] = {4,6,9,11};
    int thirtyone[] = {1,3,5,7,8,10,12};
    stringstream refline(wholedate);
    string deets;

    int i=0;
    while(getline(refline, deets, '/')){
        if(i==0)
            month=stoi(deets);
        else if(i==1)
            date=stoi(deets);
        else if(i==2)
            year=stoi(deets);
        i++;
    }

    if ((month<1) || (month>12)) return false;
    if (find(thirty, thirty + 4, month) != thirty + 4) // if month has thirty days
        if ((date<1) || (date>30)) return false;
    if (month==2&&(year%4==0)) // if month is feb and is in a leap year
        if ((date<1) || (date>29)) return false;
    else if (month==2) // if month is feb on any other year
        if ((date<1) || (date>28)) return false;
    if (find(thirtyone, thirtyone + 4, month) != thirtyone + 4)
        if ((date<1) || (date>31)) return false;
    return true;
}

bool verify_time(string time){
    int hour, minute;
    stringstream refline(time);
    string deets;

    int i=0;
    while(getline(refline, deets, ':')){
        if(i==0)
            hour=stoi(deets);
        else if(i==1)
            minute=stoi(deets);
        i++;
    }

    if ((hour<0) || (hour>23)) return false;
    if ((minute<0) || (minute>59)) return false;
    return true;
}

auto get_deadline(){
    bool date_loop = true;
    struct vals {string d, t;};
    string date, time;

    do{
        cout << "What is the date of the deadline (MM/DD/YY)? ";
        cin >> date;
        if (!verify_date(date))
            cout << "Error reading date\n";
        else
        {
            cout << "What is the time of the deadline (HH:MM 24H format)? ";
            cin >> time;
            if (!verify_time(time))
                cout << "Error reading time\n";
            else{
                date_loop = false;
            }
        }

    } while (date_loop);
    return vals{date, time};
}

void add_td(){
    string name;
    system("cls");
    // [date],[time],[prio],[done or not done],[category],[name]
    cout << "What are you planning on doing? ";
    getline(cin, name);

    auto [date, time] = get_deadline();
    int taskPriority = priorityLevel();
    string taskCategory = categoriesList();

    struct todo *new_node = new todo;
    string filename = "database/lists/" + to_string(current_user_id) + ".txt";

    new_node->userid = current_user_id;
    new_node->name = name;
    new_node->date_dl = date;
    new_node->time_dl = time;
    new_node->priority = taskPriority;
    new_node->status = false;
    new_node->category = taskCategory;
    todohead = new_node;

    ofstream user_file(filename, ios::app);
    user_file << todohead->name << ","
              << todohead->date_dl << ","
              << todohead->time_dl << ","
              << todohead->priority << ","
              << todohead->status << ","
              << todohead->category << endl;
    user_file.close();

}

void complete_td(){
    system("cls");


}

void edit_td(){
    system("cls");

}

void delete_td(){
    system("cls");

}

void options(){
    system("cls");
    int choice;

    view_td();

    cout << "What do you want to-do?" << endl
         << "[1] Add a to-do." << endl
         << "[2] Complete a to-do." << endl
         << "[3] Edit a to-do." << endl
         << "[4] Delete a to-do." << endl
         << "Choice: ";

    switch (choice){
        case 1:
            add_td();
            break;
        case 2:
            complete_td();
            break;
        case 3:
            edit_td();
            break;
        case 4:
            delete_td();
            break;
        default:
            options();
            break;
    }
}

void regis(){ // register is a keyword ?
    system("cls");
    bool loop = true;
    string username_input, password_input;

    do {
        cout << "Register for a new account." << endl << endl;

        cout << "Enter username: ";
        cin >> username_input;
        if(search_user(userhead, username_input)==true){
            cout << "Username exists, please try again. ";
            system("pause");
            system("cls");
        }
        else {
            cout << "Enter password: ";
            cin >> password_input;
            add_user(username_input, password_input);
            loop = false;
        }

    } while(loop);
}

void login(){
    system("cls");
    bool loop = true;
    string username_input, password_input;

    do {
        cout << "Enter username: ";
        cin >> username_input;
        if(search_user(userhead, username_input)==false){
            cout << "User isn't in database, please try again.";
            system("pause");
            system("cls");
            break;
        }
        cout << "Enter password: ";
        cin >> password_input;
        if (password_input != current_pass){
            cout << "Password is incorrect, please try again.";
            system("pause");
            system("cls");
            break;
        }
        loop = false;
    } while (loop);
}

void enter(){
    system("cls");
    int option;

    cout << "Welcome to XXX" << endl << endl;
    cout << "[0] Enter as a guest." << endl
         << "[1] Login as existing user." << endl
         << "[2] Register." << endl
         << "Choice: ";
    cin >> option;

    switch (option){
        case 0:
            // guestmode();
            break;
        case 1:
            system("cls");
            login();
            break;
        case 2:
            system("cls");
            regis();
            break;
        case 3:
            cout << "Entered option isn't listed. Please try again. " << endl;
            system("pause");
            system("cls");
            enter();
            break;
    }

    options();
}

int main(){
    add_td();
    // open_user_list();
    // enter();
    // [X] register and login
    // [ ] guest mode
    // [ ] display
        // [ ] - unsorted (default)
        // [ ] - by date
        // [ ] - by priority
        // [ ] - by category
            // [ ] add to-do
            // [ ] complete to-do
            // [ ] edit to-do
            // [ ] delete to-do
}