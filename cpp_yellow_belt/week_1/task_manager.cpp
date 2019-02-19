#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
#include <tuple>
#include <sstream>
using namespace std;
#if 1
// Перечислимый тип для статуса задачи
enum class TaskStatus {
  NEW,          // новая
  IN_PROGRESS,  // в разработке
  TESTING,      // на тестировании
  DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;
#endif
// Can use static_cast<int> instead of this function
// return static_cast<TaskStatus>(static_cast<int>(task_status) + 1);
// Will return TaskStatus object "saturated" to DONE
TaskStatus TaskStatusNext(TaskStatus t) {
    switch(t) {
    case TaskStatus::NEW: 
        return TaskStatus::IN_PROGRESS;
        break;
    case TaskStatus::IN_PROGRESS: 
        return TaskStatus::TESTING;
        break;
    case TaskStatus::TESTING: 
        return TaskStatus::DONE;
        break;
    default:
        throw(logic_error("Nothing after DONE"));
    }
}

TaskStatus TaskStatusPrev(TaskStatus t) {
    switch(t) {
    case TaskStatus::IN_PROGRESS: 
        return TaskStatus::NEW;
        break;
    case TaskStatus::TESTING: 
        return TaskStatus::IN_PROGRESS;
        break;
    case TaskStatus::DONE: 
        return TaskStatus::TESTING;
        break;
    default:
        throw(logic_error("Nothing before NEW"));
    }
}

class TeamTasks {
public:
  // Получить статистику по статусам задач конкретного разработчика
  const TasksInfo& GetPersonTasksInfo(const string& person) const {
      return person_tasks.at(person);
  }
  
  // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
  void AddNewTask(const string& person) {
      person_tasks[person][TaskStatus::NEW]++;
  }
  void AddNewTasks(const string& person, int num) {
      for (int i = 0; i < num; ++i) {
        AddNewTask(person);
    }
  }
  
  // Обновить статусы по данному количеству задач конкретного разработчика,
  // подробности см. ниже
  tuple<TasksInfo, TasksInfo> PerformPersonTasks(
      const string& person, int task_count) {
        TasksInfo modified; TasksInfo untouched;
        auto& person_list = person_tasks[person];

        task_count = min(CountTaskNum(person),task_count);
        for(const auto& [task_status,task_num] : person_list) {
            if(task_status == TaskStatus::DONE) { break; }
            // TODO: if task_count is a big number it should be DONE
            if(task_count < task_num) {
                untouched[task_status] = task_num - task_count;
            }
            // TODO: hande zero task_num of previos task count DONE
            while(task_num && task_count && (modified[TaskStatusNext(task_status)] < task_num)) {
                modified[TaskStatusNext(task_status)]++;
                task_count--;
            }
        }

        for(const auto& [key,value] : modified) {
            person_list[key] += value;
            person_list[TaskStatusPrev(key)] -= value;
        }

        return make_tuple(modified,untouched);
      }

private:
    int CountTaskNum(string person) const {
        int res = 0;
        for(const auto& [ts,num] : person_tasks.at(person)) {
            res += num;
        }
        return res;
    }
    map<string,TasksInfo> person_tasks;
};

string TaskStatusToString(TaskStatus t) {
    switch(t) {
    case TaskStatus::NEW: 
        return "\"NEW\"";
        break;
    case TaskStatus::IN_PROGRESS: 
        return "\"IN_PROGRESS\"";
        break;
    case TaskStatus::TESTING: 
        return "\"TESTING\"";
        break;
    default:
        return "\"DONE\"";
    }
}

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(const TasksInfo& tasks_info) {
    cout << "{";
    if(tasks_info.size()) {
        int i = 0;
        for(const auto& [status,number] : tasks_info) {
            cout << TaskStatusToString(status) << ": " <<  number;
            if(i++ + 1 != tasks_info.size()) {
                cout << ", ";
            }
        }
    }
    cout << "}";
}

void ExecuteCommand(const string& command, TeamTasks& tasks) {
    stringstream ss{command};
    string opcode;
    ss >> opcode;
    TasksInfo updated_tasks, untouched_tasks;
    if(opcode == "AddNewTasks") {
        string name; int num;
        ss >> name >> num;
        tasks.AddNewTasks(name,num);
        cout << "[]" << endl;
    } else if (opcode == "PerformPersonTasks") {
        string name; int num;
        ss >> name >> num;
        tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks(name, num);
        cout << "["; PrintTasksInfo(updated_tasks); cout << ", ";
        PrintTasksInfo(untouched_tasks); cout << "]" << endl;
    } else if(opcode == "GetPersonTasksInfo") {
        string name;
        ss >> name;
        PrintTasksInfo(tasks.GetPersonTasksInfo(name));
        cout << endl;
    } else {
        throw(runtime_error("Unknown command: "+opcode));
    }
}

int main() {
    TeamTasks tasks;

    string command;
    while (getline(cin, command)) {
        string feedback;
        try {  
            ExecuteCommand(command, tasks);
        } catch(exception& e) {
            cout << e.what() << endl;
            return -1;
        }
    }

  return 0;
}
