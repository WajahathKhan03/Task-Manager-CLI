#include<stdio.h>
#include<stdlib.h>
#include <string>
#include<vector>
#include <iostream>
#include<ctime>
#include<unordered_map>
#include<fstream>
using namespace std;

 
class Task {
public:
    int id;
    string title;
    string status;
    string createdAt;
    string updatedAt;

    Task() : id(0), title("unknown"), status("unknown"), createdAt("unknown"), updatedAt("unknown") {}

    Task(int id, string title, string status) {
        this->id = id;
        this->title = title;
        this->status = status;
        time_t now = time(nullptr);
        createdAt = string(ctime(&now));
        updatedAt = createdAt;
    }
};

 
class D : public Task {
public:
    unordered_map<int, Task> dataBase;

    // Add tasks to database
    void addTask(int i, string til, string stat) {
        Task t(i, til, stat);
        dataBase[i] = t;
    }

    // Update tasks in the database using the ID
    void updateTask(int i, string state) {
        if (dataBase.find(i) != dataBase.end()) { // Check if task exists
            dataBase[i].status = state;
            time_t now = time(nullptr);
            dataBase[i].updatedAt = string(ctime(&now));
        } else {
            cout << "Task ID " << i << " not found!\n";
        }
    }

    // Converts Task object to a string
    string toFileString(Task k) {
        return to_string(k.id) + "|" + k.title + "|" + k.status + "|" + k.createdAt + "\n";
    }
    //list all the tasks
    void loadTasksFromFile() {
    ifstream file("tasks.txt");
    if (!file) {
        cout << "No existing tasks found.\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        vector<string> parts;
        string word = "";
        
        // Split by '|'
        for (char c : line) {
            if (c == '|') {
                parts.push_back(word);
                word = "";
            } else {
                word += c;
            }
        }
        parts.push_back(word); 

         for(int i=0;i<parts.size();i++){
            if(i==1){
                cout<<parts[i]<<"|";
            }
            else {
                cout<<parts[i]<<"|";
            }
         }
         cout<<"\n";
    }

    file.close();
}
//list all the tasks that are of same type 
   void loadExTaskFromFile(string type){
      ifstream file("tasks.txt");
      if(!file){
        std::cout<<"no tasks entered\n";
        return;
      }
      string in;
      while (getline(file, in)) {
        vector<string> storedData;
        string word = "";
        // Split by '|'
        for (char c : in) {
            if (c == '|') {
                storedData.push_back(word);
                word = "";
            } else {
                word += c;
            }
        }
        if(word!=""){
        storedData.push_back(word); 
        }
        bool likeExist=false;
            for(auto x:storedData){
                if(x==type){
                 likeExist=true;
                }
            }
        if(likeExist){
            for(auto x: storedData){
                std::cout<<x<<"|";
            }
        }
         std::cout<<"\n";
    }
    file.close();
   }
};

 
void loadDataToFile(D &t) {
    if (t.dataBase.empty()) {  
        return;
    }

    ofstream file("tasks.txt");
    for (auto it = t.dataBase.begin(); it != t.dataBase.end(); it++) {
        file << t.toFileString(it->second);
    }
    file.close();
}
int stringToInt(string s) {
    if (s.empty()) throw invalid_argument("Empty string");

    int num = 0, sign = 1;
    int i = 0;

  
    if (s[0] == '-') {
        sign = -1;
        i = 1; 
    }

    for (; i < s.length(); i++) {
        if (s[i] < '0' || s[i] > '9') {
            throw invalid_argument("Invalid number format");
        }
        num = num * 10 + (s[i] - '0');
    }

    return sign * num;
}

int main()
{
    D myTask;
    while (true)
    {
        vector<string> argv;
        string input;
        getline(cin, input);

        string word = "";
        for (char c : input)
        {
            if (c == ' ')
            {
                if (!word.empty())
                {
                    argv.push_back(word);
                    word = ""; // Reset word
                }
            }
            else
            {
                word += c;
            }
        }
        if (!word.empty())
        { // Push last word
            argv.push_back(word);
        }
        if (argv[0] == "task")
        {
            if (argv[1] == "exit")
            {
                break;
            }
            else
            {
                if (argv[1] == "add")
                {
                    int id = stoi(argv[2]);
                    std::cout << "adding task ...\n";
                    myTask.addTask(id, argv[3], argv[4]);
                }
                else if (argv[1] == "update")
                {
                    myTask.updateTask(stringToInt(argv[2]), argv[3]);
                }
                else if (argv.size() == 3 && argv[1] == "list")
                {
                    if (argv[2] == "todo" || argv[2] == "pending" || argv[2] == "done")
                    {
                        myTask.loadExTaskFromFile(argv[2]);
                    }
                }
                else if (argv.size() == 2 && argv[1] == "list")
                {
                    myTask.loadTasksFromFile();
                }
            }
        }
    }
    loadDataToFile(myTask);
    std::cout << "saved tasks\n";
    return 0;
}
