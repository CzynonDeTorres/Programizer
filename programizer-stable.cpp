#include <iostream>
#include <fstream> // file stream
#include <string>  // string
#include <sstream>
#include <stdio.h>   // scanf
#include <algorithm> // find
#include <queue>     // queue
#include <iomanip>   // setw

using namespace std;

string current_user, current_pass;
int current_user_id, line_count = 0;

// widths
int id_width = 6,
    todo_width = 8,
    date_width = 9,
    time_width = 9,
    status_width = 12,
    prio_width = 12,
    categ_width = 12;

// nodes
struct user
{
    int id;
    string name;
    string pass;
    user *next;
};

struct todo
{
    int id;
    string name;
    string date_dl;
    string time_dl; // current available time method is a bit confusing so we just have to suffer
    int priority;
    // 1 - most important
    // 2 - mid
    // 3 - least important
    bool status; // done or not done
    string category;
    todo *next;
};

// node initialization
user *userhead = NULL;
todo *todohead = NULL;

void add_user(string user_input, string pass_input)
{
    struct user *new_node = new user;

    if (userhead == NULL)
        new_node->id = 0;
    else
        new_node->id = userhead->id + 1;
    new_node->name = user_input;
    new_node->pass = pass_input;
    new_node->next = userhead;
    userhead = new_node;

    current_user_id = userhead->id;

    ofstream user_file("database/users.txt", ios::app);
    if (user_file.is_open())
        user_file << userhead->id << "," << user_input << "," << pass_input << endl;
    user_file.close();
}

void insert_user(int n, string user_input, string pass_input)
{
    struct user *new_node = new user;
    new_node->id = n;
    new_node->name = user_input;
    new_node->pass = pass_input;
    new_node->next = userhead;
    userhead = new_node;
}

void open_user_list()
{
    ifstream user_file;
    string line, deets, user, pass;
    int id;
    user_file.open("database/users.txt");
    if (user_file.is_open())
    {
        while (getline(user_file, line))
        {
            stringstream refline(line);
            int i = 0;
            while (getline(refline, deets, ','))
            {
                if (i == 0)
                    id = stoi(deets);
                else if (i == 1)
                    user = deets;
                else if (i == 2)
                    pass = deets;
                i++;
            }
            insert_user(id, user, pass);
        }
    }
    else
        ofstream user_file("database/users.txt");

    user_file.close();
}

bool search_user(struct user *head, string name)
{
    if (head == NULL)
        return false;
    if (head->name == name)
    {
        current_user_id = head->id;
        current_user = head->name;
        current_pass = head->pass;
        return true;
    }

    return search_user(head->next, name);
}

void insert_todo(int n, string name, string date, string time, int prio, bool status, string categ)
{
    struct todo *new_node = new todo;
    new_node->id = n;
    new_node->name = name;
    new_node->date_dl = date;
    new_node->time_dl = time;
    new_node->priority = prio;
    new_node->status = status;
    new_node->category = categ;
    new_node->next = todohead;
    todohead = new_node;
}

void fix_td_width()
{
    struct todo *temp = todohead;

    if (temp == nullptr)
        return;
    while (temp != nullptr)
    {
        if (temp->name.length() + 4 > todo_width)
            todo_width = temp->name.length() + 4;
        if (temp->category.length() + 4 > categ_width)
            categ_width = temp->category.length() + 4;
        temp = temp->next;
    }
}

void initialize_td()
{
    ifstream user_file;
    string line, deets, name, date, time, categ;
    int id, prio;
    bool status;
    string filename = "database/lists/" + to_string(current_user_id) + ".txt";
    user_file.open(filename);
    if (user_file.is_open())
    {
        while (getline(user_file, line))
        {
            stringstream refline(line);
            int i = 0;
            while (getline(refline, deets, ','))
            {
                if (i == 0)
                    id = stoi(deets);
                else if (i == 1)
                    name = deets;
                else if (i == 2)
                    date = deets;
                else if (i == 3)
                    time = deets;
                else if (i == 4)
                    prio = stoi(deets);
                else if (i == 5)
                {
                    if (deets == "0")
                        status = false;
                    if (deets == "1")
                        status = true;
                }
                else if (i == 6)
                    categ = deets;
                i++;
            }
            insert_todo(id, name, date, time, prio, status, categ);
            line_count++;
        }
    }
    else
        ofstream user_file(filename);

    user_file.close();
}

void view_td()
{
    fix_td_width();

    todo *temp = new todo;
    temp = todohead;

    if (temp == nullptr)
    {
        cout << "List is empty." << endl;
    }
    else
        cout << left << setw(id_width) << "ID"
             << left << setw(todo_width) << "Name"
             << left << setw(date_width) << "Deadline:"
             << left << setw(time_width) << ""
             << left << setw(prio_width) << "Priority"
             << left << setw(status_width) << "Status"
             << left << setw(categ_width) << "Category" << endl;

    while (temp != nullptr)
    {
        string status;
        if (temp->status == false)
            status = "Not done";
        if (temp->status == true)
            status = "Done";
        cout << left << setw(id_width) << temp->id
             << left << setw(todo_width) << temp->name
             << left << setw(date_width) << temp->date_dl
             << left << setw(time_width) << temp->time_dl
             << left << setw(prio_width) << temp->priority
             << left << setw(status_width) << status
             << left << setw(categ_width) << temp->category << endl;
        temp = temp->next;
    }
    cout << endl;
}

void complete_todo(struct todo *head, int id)
{
    while (head != NULL)
    {
        if (head->id == id)
        {
            head->status = true;
        }
        head = head->next;
    }
}

void show_categories(queue<string> q)
{
    while (!q.empty())
    {
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
            getline(cin >> ws, userTaskCateg);
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
            return (priorityTask == 1 ? 1 : (priorityTask == 2 ? 2 : 3)); // para mas maikli hsskjas
        }

    } while (task_loop);
}

bool verify_date(string wholedate)
{
    int month, date, year;
    int thirty[] = {4, 6, 9, 11};
    int thirtyone[] = {1, 3, 5, 7, 8, 10, 12};
    stringstream refline(wholedate);
    string deets;

    int i = 0;
    while (getline(refline, deets, '/'))
    {
        if (i == 0)
            month = stoi(deets);
        else if (i == 1)
            date = stoi(deets);
        else if (i == 2)
            year = stoi(deets);
        i++;
    }

    if ((month < 1) || (month > 12))
        return false;
    if (find(thirty, thirty + 4, month) != thirty + 4) // if month has thirty days
        if ((date < 1) || (date > 30))
            return false;
    if (month == 2 && (year % 4 == 0)) // if month is feb and is in a leap year
        if ((date < 1) || (date > 29))
            return false;
        else if (month == 2) // if month is feb on any other year
            if ((date < 1) || (date > 28))
                return false;
    if (find(thirtyone, thirtyone + 4, month) != thirtyone + 4)
        if ((date < 1) || (date > 31))
            return false;
    return true;
}

bool verify_time(string time)
{
    int hour, minute;
    stringstream refline(time);
    string deets;

    int i = 0;
    while (getline(refline, deets, ':'))
    {
        if (i == 0)
            hour = stoi(deets);
        else if (i == 1)
            minute = stoi(deets);
        i++;
    }

    if ((hour < 0) || (hour > 23))
        return false;
    if ((minute < 0) || (minute > 59))
        return false;
    return true;
}

auto get_deadline()
{
    bool date_loop = true;
    struct vals
    {
        string d, t;
    };
    string date, time;

    do
    {
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
            else
            {
                date_loop = false;
            }
        }

    } while (date_loop);
    return vals{date, time};
}

void add_td()
{
    string name;
    system("cls");
    cout << "What are you planning on doing? ";
    getline(cin >> ws, name);

    auto [input_date, input_time] = get_deadline();
    int taskPriority = priorityLevel();
    string taskCategory = categoriesList();

    struct todo *new_node = new todo;
    string filename = "database/lists/" + to_string(current_user_id) + ".txt";

    if (todohead == NULL)
        new_node->id = 0;
    else
        new_node->id = todohead->id + 1;
    new_node->name = name;
    new_node->date_dl = input_date;
    new_node->time_dl = input_time;
    new_node->priority = taskPriority;
    new_node->status = false;
    new_node->category = taskCategory;
    new_node->next = todohead;
    todohead = new_node;

    ofstream user_file(filename, ios::app);
    if (user_file.is_open())
        user_file << todohead->id << ","
                  << todohead->name << ","
                  << todohead->date_dl << ","
                  << todohead->time_dl << ","
                  << todohead->priority << ","
                  << todohead->status << ","
                  << todohead->category << endl;
    user_file.close();
}

void complete_td()
{
    system("cls");
    view_td();

    bool loop = true;
    int id;

    cout << "What has been done? ";
    cin >> id;
    complete_todo(todohead, id);
}

void edit_td()
{
    system("cls");
    view_td();

    bool loop = true;
    int id, property, new_prio;
    string new_name, new_date, new_time, new_categ;
    struct todo *temp = new todo;
    temp = todohead;
    do
    {
        cout << "Which item do you want to edit? ";
        cin >> id;
        cout << "What do you want to edit? " << endl
             << "[1] Name" << endl
             << "[2] Date of deadline" << endl
             << "[3] Time of deadline" << endl
             << "[4] Priority" << endl
             << "[5] Category" << endl;
        cin >> property;
        switch (property)
        {
        case 1:
            cout << "What should be the new name? ";
            getline(cin >> ws, new_name);
            while (temp != nullptr)
            {
                if (temp->id == id)
                    temp->name = new_name;

                temp = temp->next;
            }
            loop = false;
            break;
        case 2:
            cout << "What should be the new date of deadline? ";
            cin >> new_date;
            if (verify_date(new_date))
                while (temp != nullptr)
                {
                    if (temp->id == id)
                        temp->date_dl = new_date;

                    temp = temp->next;
                }
            loop = false;
            break;
        case 3:
            cout << "What should be the new time of deadline? ";
            cin >> new_time;
            if (verify_time(new_time))
                while (temp != nullptr)
                {
                    if (temp->id == id)
                        temp->time_dl = new_time;

                    temp = temp->next;
                }
            loop = false;
            break;
        case 4:
            new_prio = priorityLevel();
            while (temp != nullptr)
            {
                if (temp->id == id)
                    temp->priority = new_prio;

                temp = temp->next;
            }
            loop = false;
            break;
        case 5:
            new_categ = categoriesList();
            while (temp != nullptr)
            {
                if (temp->id == id)
                    temp->category = new_categ;
                temp = temp->next;
            }
            loop = false;
            break;
        default:
            cout << "Not in the choices. ";
        }
    } while (loop);
}

void delete_td(struct todo *head)
{
    system("cls");
    todo *curr = head, *previous = NULL;
    int del_id;

    view_td();

    cout << "What do you want to delete? ";
    cin >> del_id;

    if (!head)
        return;
    if (head->id == del_id)
    {
        curr = head->next;
        delete head;
        head = curr;
    }
    else
    {
        curr = head;
        while (curr != NULL && curr->id != del_id)
        {
            previous = curr;
            curr = curr->next;
        }
        previous->next = curr->next;
        delete curr;
    }
    view_td();
    view_td();
    system("cls");
}

void exit()
{
    int i = 0;
    struct todo *temp = todohead;
    string filename = "database/lists/" + to_string(current_user_id) + ".txt";
    if (current_user_id == -1)
    {
        ofstream user_file(filename);
        user_file << "";
        user_file.close();
    }
    else
        while (temp != NULL)
        {
            if (i == 0)
            {
                ofstream user_file(filename);
                user_file << i << ","
                          << temp->name << ","
                          << temp->date_dl << ","
                          << temp->time_dl << ","
                          << temp->priority << ","
                          << temp->status << ","
                          << temp->category << endl;
                user_file.close();
            }
            else
            {
                ofstream user_file(filename, ios::app);
                user_file << i << ","
                          << temp->name << ","
                          << temp->date_dl << ","
                          << temp->time_dl << ","
                          << temp->priority << ","
                          << temp->status << ","
                          << temp->category << endl;
                user_file.close();
            }
            temp = temp->next;
            i++;
        }
    system("cls");
    cout << "Thank you for using this program!";
    return;
}

void options()
{
    char choice;
    system("cls");
    view_td();

    cout << "What do you want to-do?" << endl
         << "[1] Add a to-do." << endl
         << "[2] Complete a to-do." << endl
         << "[3] Edit a to-do." << endl
         << "[4] Delete a to-do." << endl
         << "[5] Exit." << endl
         << "Choice: ";

    cin >> choice;

    switch (choice)
    {
    case '1':
        add_td();
        options();
        break;
    case '2':
        complete_td();
        options();
        break;
    case '3':
        edit_td();
        options();
        break;
    case '4':
        delete_td(todohead);
        options();
        break;
    case '5':
        exit();
        break;
    default:
        options();
        break;
    }
}

void regis()
{ // register is a keyword apparently
    system("cls");
    bool loop = true;
    string username_input, password_input;

    do
    {
        cout << "Register for a new account." << endl
             << endl;

        cout << "Enter username: ";
        cin >> username_input;
        if (search_user(userhead, username_input) == true)
        {
            cout << "Username exists, please try again. ";
            system("pause");
            system("cls");
        }
        else
        {
            cout << "Enter password: ";
            cin >> password_input;
            add_user(username_input, password_input);
        }

    } while (loop);
}

void login()
{
    system("cls");
    bool loop = true;
    string username_input, password_input;

    do
    {
        cout << "Enter username: ";
        cin >> username_input;
        if (search_user(userhead, username_input) == false)
        {
            cout << "User isn't in database, please try again.";
            system("pause");
            system("cls");
        }
        else
        {
            cout << "Enter password: ";
            cin >> password_input;
            if (password_input != current_pass)
            {
                cout << "Password is incorrect, please try again.";
                system("pause");
                system("cls");
                break;
            }
            loop = false;

            initialize_td();
            options();
        }
    } while (loop);
}

void guestmode()
{
    current_user_id = -1;

    initialize_td();
    options();
}

void enter()
{
    system("cls");
    char option;

    cout << "Welcome to Programizer" << endl
         << endl;
    cout << "[0] Enter as a guest." << endl
         << "[1] Login as existing user." << endl
         << "[2] Register." << endl
         << "Choice: ";
    cin >> option;

    switch (option)
    {
    case '0':
        guestmode();
        break;
    case '1':
        system("cls");
        login();
        break;
    case '2':
        system("cls");
        regis();
        break;
    default:
        cout << "Entered option isn't listed. Please try again. " << endl;
        system("pause");
        system("cls");
        enter();
        break;
    }
}

int main()
{
    open_user_list();
    enter();
    return 0;
}
