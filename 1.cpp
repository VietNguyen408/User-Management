#include<iostream>
#include<cstring>
#include<string>
using namespace std;
const int LEN = 50;

//Definition for the class Person
class Person{
protected:
    char first_name[LEN], surname[LEN], phone_number[LEN];
public:
    Person();
    Person(char[], char[], char[]);
    char* to_string();
    Person* search(char *search_key){
        return (strcmp(search_key,this->surname) == 0  || 
                strcmp(search_key,this->first_name)==0 ||
                strcmp(search_key,this->phone_number)==0) ? this : NULL;}
};

Person::Person(){
    strcpy(first_name, "");
	strcpy(surname,"");
	strcpy(phone_number,"");
}

Person::Person(char first_name[], char surname[], char phone_number[]){
    strcpy(this->first_name, first_name);
    strcpy(this->surname, surname);
    strcpy(this->phone_number,phone_number);
}

char* Person::to_string(){
	int actual_size = sizeof(first_name) + sizeof(surname) + sizeof(phone_number);
	char* object_data = new char[actual_size + 1];
	object_data[actual_size] = '\0';
	actual_size = snprintf(object_data,LEN,"%s %s %s",this->first_name, this->surname, this->phone_number);
	return object_data;
}


//Definition for class User
class User : public Person{
	char username[LEN], password[LEN];
    char permissions[4];
public:
    //constructor of the class User
	User();
	User(char[], char[], char[]);

    char *to_string();
    void display_info();
    bool compare(User *user);
    User* search(char* search_key){return (Person::search(search_key)) ? this : NULL;}
	friend class Admin;
};

User::User(){
    strcpy(username,"");
	strcpy(password, "");
    strcpy(permissions,"000");
}

User::User(char first_name[], char surname[], char phone_number[]) : Person(first_name,surname,phone_number ){
    strcpy(username,"");
	strcpy(password, "");
    strcpy(permissions,"000");
}

char* User::to_string(){
	char* person_to_string = Person::to_string();
    int actual_size = sizeof(person_to_string) + sizeof(username) + sizeof(permissions);
	char* object_data = new char[actual_size + 1];
	object_data[actual_size] = '\0';
	actual_size = snprintf(object_data,LEN,"%s %s %s",person_to_string, this->username,this->permissions);
	return object_data;
}

void User::display_info() {
	cout << this->to_string();
}

bool User::compare(User *user) {
	return (strcmp(user->phone_number, this->phone_number) == 0);
}


//This is definition for the class Admin
class Admin : public Person{
    char username[LEN], password[LEN];
public:
    Admin();
    Admin(char[], char[]);
    bool authenticate();
	void set_username(User*, char[]);
	void set_password(User*, char[]);
	void set_permission(User*, char[]);
    void remove_permission(User*, char[]);
};

Admin::Admin(){
    strcpy(username,"");
    strcpy(password,"");
}

Admin::Admin(char username[],char password[]){
    strcpy(this->username,username);
    strcpy(this->password,password);
}

bool Admin::authenticate() {
	char username[LEN], password[LEN];
	cout << "Enter admin's username: ";
	cin >> ws >> username;
	cout << "Enter admin's password: ";
	cin >> ws >> password;
	return (strcmp(username, this->username) == 0 &&
		    strcmp(password, this->password) == 0);
}

void Admin::set_username(User *user, char new_username[]) {
	if (this->authenticate())
		strcpy(user->username, new_username);
	else
		cout << "Permission denied.\n";
}

void Admin::set_password(User *user, char new_passwd[]) {
	if (this->authenticate())
		strcpy(user->password, new_passwd);
	else 
		cout << "Permission denied.\n";
}

void Admin::set_permission(User *user, char p[]){
    if(this->authenticate()){
        for(int i=0;i<strlen(p);++i){
            switch(p[i]) {
	            case 'r':
		            user->permissions[0] = 'r';
		            cout << "Permission granted.\n";
		            break;
	            case 'w':
		            user->permissions[1] = 'w';
		            cout << "Permission granted.\n";
		            break;
	            case 'x':
		            user->permissions[2] = 'x';
		            cout << "Permission granted.\n";
		            break;
	            default:
		            cout << "Only r, w and x are excepted.\n";
		            break;
	        }
        }
    }else{
        cout << "Permission denied";
    }
}

void Admin::remove_permission(User* user,char p[]){
    if(this->authenticate()){
        for(int i=0;i<strlen(p);++i){
            switch(p[i]) {
	            case 'r':
		            user->permissions[0] = '0';
		            cout << "Successfully remove read permission.\n";
		            break;
	            case 'w':
		            user->permissions[1] = '0';
		            cout << "Successfully remove write permission.\n";
		            break;
	            case 'x':
		            user->permissions[2] = '0';
		            cout << "Successfully remove execute permission.\n";
		            break;
	            default:
		            cout << "Only r, w and x are excepted.\n";
		            break;
	        }
        }
    }else{
        cout << "Permission denied";
    }
}

//function in loop
void insertUser(User*, int, Admin*);
User* expandList(User*, int);
void findUser(User*, char[], int);
void printList(User*, int);



void loop(User *userList, Admin *admin, int size){
	while(true){
    char username[LEN];
	char permission[LEN];
	cout << "There are " << size << " user(s) in the database." << endl;
	cout << "-------List of options-------" << endl;
  	cout << "0: quit\n";
    cout << "1: add new user information\n";
    cout << "2: search based on user's first name\n";
    cout << "3: update user's username and password\n";
	cout << "4: set permission for users\n"; 
    cout << "5: remove permission for users\n";
    cout << "6: print the user list\n";
    cout << "Enter your option(0->6): ";

  	int option;
  	cin >> option; 
	cout << endl;
    
  	switch(option){
  	        case 1:
				size++;
				userList = expandList(userList, size);
				insertUser(userList, size, admin);
				break;

			case 2:
                User* temp;
                char search_key[LEN]; 
				cout << "Enter the first name you want to search for: ";
				cin >> ws; 
				cin.getline(search_key, LEN);
                for(int i=0;i<size;++i){
      				temp = userList[i].search(search_key);
    				if (temp != NULL) break;				
				}
			//------------------------------------------------------------
			if (temp == NULL) {
				cout << "Cannot find in the database" << endl ;
			}else{
			cout << "User's info: " << temp->to_string() << endl << endl;
			}
    		break;

			case 3:
                char first_name_update[LEN],username_update[LEN],password_update[LEN];
    		    cout << "Enter user's first name to be updated: "; cin >> first_name_update;
    		    for (int i = 0; i<size; i++){
    			    if (userList[i].search(first_name_update) != NULL){
                        cout << "Enter new username: "; cin >> username_update;
                        cout << "Enter new password: "; cin >> password_update;
    				    admin->set_username(&userList[i],username_update);
                        admin->set_password(&userList[i],password_update);
    				    break;
				    }
			    }
    		    break;

			case 4:
    		    cout << "Enter user's first name to be updated: "; cin >> first_name_update;
                for (int i = 0; i<size; i++){
    			    if (userList[i].search(first_name_update) != NULL){
                        cout << "Enter the permission you want to add (r/w/x only): ";
				        cin >> ws; 
				        cin.getline(permission, LEN);
				        admin->set_permission(&userList[i],permission);
                        cout << endl;
    				    break;
				    }
			    }
				break;

            case 5:
    		    cout << "Enter user's first name to be updated: "; cin >> first_name_update;
                for (int i = 0; i<size; i++){
    			    if (userList[i].search(first_name_update) != NULL){
                        cout << "Enter the permission you want to remove (r/w/x only): ";
				        cin >> ws; 
				        cin.getline(permission, LEN);
				        admin->remove_permission(&userList[i],permission);
                        cout << endl;
    				    break;
				    }
			    }
				break;

			case 6:
				printList(userList, size);
                cout << endl;
				break;

			case 0:
				if (size > 0) delete [] userList;
				delete admin;
				cout << "Hyvasti";
				return;
			
			default:
				cout << "Only 0 to 6 are accepted.\n";
				break;
		}
	}
}


int main(){
    char name[LEN], pass[LEN];
	cout << "Enter admin username: ";
	cin >> ws >> name;
	cout << "Enter admin password: ";
	cin >> ws >> pass;
	cout << "\n\n";

	Admin *a = new Admin(name, pass);
    User *u;

  	loop(u,a,0);
}


User* expandList(User* list ,int size) {
    User *newList = new User[size];
    for (int i = 0; i < size - 1; i++) {
        newList[i] = list[i];
    }
    return newList;
}


void insertUser(User* list, int size, Admin *admin) {
    char fname[LEN], lname[LEN], phone[LEN];
	char username[LEN], pass[LEN];

    cout << "First name: ";
    cin >> ws; cin.getline(fname, LEN);
	cout << "Last name: ";
    cin >> ws; cin.getline(lname, LEN);
	cout << "Phone number: ";
    cin >> ws; cin.getline(phone, LEN);
	cout << "Username: ";
    cin >> ws; cin.getline(username, LEN);
	cout << "Password: ";
    cin >> ws; cin.getline(pass, LEN);
	cout << "\n\n";

	User *tmpUser = new User(fname, lname, phone);
    list[size-1] = *tmpUser;

	admin->set_username(&list[size-1], username);
	admin->set_password(&list[size-1], pass);

	cout << "\nUser added to the list:\n";
	list[size-1].display_info();
    cout << endl;
}


void findUser(User* list, char first_name[], int size) {
	if (!size) {
		cout << "The list is currently empty.\n";
		return;
	}
    for (int i = 0; i < size; i++) {
        User *x = list[i].search(first_name);
        if (x != NULL) {
            x->display_info();
            return;
        }
    }
    cout << "There is no such User that matches the given first name.\n";
}


void printList(User *list, int size) {
	if (!size) {
		cout << "The list is currently empty.\n";
		return;
	}
    for (int i = 0; i < size; i++) {
        cout <<"____________________\n";
        list[i].display_info();
        cout << endl;
    }
}