#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

class Task {
public:
    string title;
    string description;
    string deadline;
    bool isCompleted;

    Task(string t = "", string d = "", string dl = "", bool c = false)
        : title(t), description(d), deadline(dl), isCompleted(c) {
    }
};

class TaskManager {
private:
    Task* tasks;       // tablica dynamiczna
    int taskCount;     // ilosc zadan
    int capacity;      // max ilosc zadan

    void resize() {
        int newCapacity = capacity * 2;
        Task* newTasks = new Task[newCapacity];
        for (int i = 0; i < taskCount; i++) {
            newTasks[i] = tasks[i];
        }
        delete[] tasks;
        tasks = newTasks;
        capacity = newCapacity;
    }

public:
    TaskManager(int initialCapacity = 10)
        : taskCount(0), capacity(initialCapacity) {
        tasks = new Task[capacity];
    }

    ~TaskManager() {
        delete[] tasks;
    }

    void addTask(const string& title, const string& description, const string& deadline) {
        if (taskCount == capacity) {

            resize();
        }

        tasks[taskCount++] = Task(title, description, deadline, false);
    }

    void displayTasks() const {
        cout << "#   | Name                 | Description                    | Deadline     | Status\n";
        cout << "----|----------------------|--------------------------------|--------------|----------\n";
        for (int i = 0; i < taskCount; i++) {
            cout << setw(3) << i + 1 << " | " << setw(20) << tasks[i].title
                << " | " << setw(30) << tasks[i].description
                << " | " << setw(12) << tasks[i].deadline
                << " | " << (tasks[i].isCompleted ? "Complete" : "Not implemented") << "\n";
        }
    }

    void markTaskAsCompleted(int index) {
        if (index >= 0 && index < taskCount) {
            tasks[index].isCompleted = true;
        }
        else {
            cout << "Incorrect task index!\n";
        }
    }

    void markTaskANcompleted(int index) {
        if (index >= 0 && index < taskCount) {
            tasks[index].isCompleted = false;
        }
        else {
            cout << "Incorrect task index!\n";
        }
    }

    void editTask(int index, const string& newTitle, const string& newDescription, const string& newDeadline) {
        if (index >= 0 && index < taskCount) {
            tasks[index].title = newTitle;
            tasks[index].description = newDescription;
            tasks[index].deadline = newDeadline;
        }
        else {
            cout << "Incorrect task index!\n";
        }
    }

    void sortByDeadline() {
        sort(tasks, tasks + taskCount, [](const Task& a, const Task& b) {
            return a.deadline < b.deadline;
            });
    }

    void saveToFile(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            for (int i = 0; i < taskCount; i++) {
                file << tasks[i].title << "|" << tasks[i].description << "|"
                    << tasks[i].deadline << "|" << tasks[i].isCompleted << "\n";
            }
            file.close();
        }
        else {
            cout << "Could not open file for writing!\n";
        }
    }

    void deleteTaskByIndex(int index) {
        if (index < 0 || index >= taskCount) {
            cout << "Invalid task index!\n";
            return;
        }

        for (int i = index; i < taskCount - 1; i++) {
            tasks[i] = tasks[i + 1];
        }

        taskCount--; 
        cout << "Task deleted successfully.\n";
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);

        if (file.is_open()) {
            taskCount = 0;
            string line;

            while (getline(file, line)) {
                size_t pos1 = line.find('|');

                size_t pos2 = line.find('|', pos1 + 1);

                size_t pos3 = line.find('|', pos2 + 1);

                string title = line.substr(0, pos1);

                string description = line.substr(pos1 + 1, pos2 - pos1 - 1);

                string deadline = line.substr(pos2 + 1, pos3 - pos2 - 1);

                bool isCompleted = stoi(line.substr(pos3 + 1));

                addTask(title, description, deadline);
                tasks[taskCount - 1].isCompleted = isCompleted;
            }
            file.close();
        }
        else {
            cout << "Could not open file for reading!\n";
        }
    }
};

// MAIN
int main() {
    setlocale(LC_CTYPE, "EN");
    TaskManager manager;

    manager.loadFromFile("tasks.txt");

    int choice;
    do {
        cout << "\nTask menedger\n";

        cout << "1. Add task\n";

        cout << "2. Show all tasks\n";

        cout << "3. Mark task as completed\n";

        cout << "4. Edit task\n";

        cout << "5. Sort tasks by deadline\n";

        cout << "6. Save task to file\n";

        cout << "7. Delete task\n";

        cout << "8. Mark task as not implemented\n";

        cout << "0. Exite\n";

        cout << "Your choice: ";

        cin >> choice;

        switch (choice) {

        case 1: {

            cin.ignore();
            string title, description, deadline;
            cout << "Enter a name: ";

            getline(cin, title);
            cout << "Enter a description: ";

            getline(cin, description);
            cout << "Enter a deadline (YYYY-MM-DD): ";

            getline(cin, deadline);

            manager.addTask(title, description, deadline);

            break;

        }

        case 2:

            manager.displayTasks();
            break;

        case 3: {

            int index;
            cout << "Enter the task number: ";
            cin >> index;
            manager.markTaskAsCompleted(index - 1);
            break;

        }
        case 4: {

            int index;
            cin.ignore();

            string newTitle, newDescription, newDeadline;

            cout << "Enter the task number: ";
            cin >> index;
            cin.ignore();

            cout << "New name: ";
            getline(cin, newTitle);

            cout << "New description: ";
            getline(cin, newDescription);

            cout << "New deadline (YYYY-MM-DD): ";
            getline(cin, newDeadline);

            manager.editTask(index - 1, newTitle, newDescription, newDeadline);

            break;

        }
        case 5:

            manager.sortByDeadline();
            cout << "Tasks sorted by deadline.\n";
            break;

        case 6:

            manager.saveToFile("tasks.txt");
            cout << "Task saved to file.\n";
            break;

        case 7:

            int index;
            cout << "Enter the task number to delete: ";
            cin >> index;
            manager.deleteTaskByIndex(index - 1); 
            break;

        case 8:

            int index1;
            cout << "Enter the task number: ";
            cin >> index1;
            manager.markTaskANcompleted(index1 - 1);
            break;


        case 0:

            manager.saveToFile("tasks.txt");
            cout << "Goodbye!\n";
            break;

        default:

            cout << "Wrong choise!\n";
        }

    } while (choice != 0);
    return 0;
}
