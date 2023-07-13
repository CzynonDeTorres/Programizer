#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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

// node initialization
user *head = NULL;

void add_user(string user_input, string pass_input){
    struct user *new_node = new user;

    if(head==NULL){
        new_node->id = 0;
        new_node->name = user_input;
        new_node->pass = pass_input;
        new_node->next = head;
        head = new_node;
    }
    else{
        new_node->id = head->id + 1;
        new_node->name = user_input;
        new_node->pass = pass_input;
        new_node->next = head;
        head = new_node;
    }

    ofstream user_file("database/users.txt", ios::app);
    user_file << head->id << "," << user_input << "," << pass_input << endl;
    user_file.close();

}

void insert_user(int n, string user_input, string pass_input){
    struct user *new_node = new user;
    new_node->id = n;
    new_node->name = user_input;
    new_node->pass = pass_input;
    new_node->next = head;
    head = new_node;
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

void regis(){ // register is a keyword ?
    bool loop = true;
    string username_input, password_input;

    do {
        cout << "Register for a new account." << endl << endl;

        cout << "Enter username: ";
        cin >> username_input;
        if(search_user(head, username_input)==true){
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
    bool loop = true;
    string username_input, password_input;

    do {
        cout << "Enter username: ";
        cin >> username_input;
        if(search_user(head, username_input)==false){
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
}

int main(){
    open_user_list();
    enter();
    // login (user or guest)
    // options
        // view to-do
            // back
            // finish item
        // add to-do
        // delete to-do
}