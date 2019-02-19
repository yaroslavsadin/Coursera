#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
#include <tuple>
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
        auto& person_list = person_tasks.at(person);

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
    int CountTaskNum(string person) {
        int res = 0;
        for(const auto& [ts,num] : person_tasks.at(person)) {
            res += num;
        }
        return res;
    }
    map<string,TasksInfo> person_tasks;
};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
  cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
      ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
      ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
      ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
}

string TaskStatusToString(TaskStatus t) {
    switch(t) {
    case TaskStatus::NEW: 
        return "NEW";
        break;
    case TaskStatus::IN_PROGRESS: 
        return "IN_PROGRESS";
        break;
    case TaskStatus::TESTING: 
        return "TESTING";
        break;
    default:
        return "DONE";
    }
}

void PrintTasksInfo(const TasksInfo& modified,const TasksInfo& untouched) {
    cout << "[";
    if(modified.size()) {
        cout << "{";
        for(const auto& [status,number] : modified) {
            cout << TaskStatusToString(status) << ": " <<  number << " ";
        }
        cout << "}";
    }
    if(untouched.size()) {
        cout << ", {";
        for(const auto& [status,number] : untouched) {
            cout << TaskStatusToString(status) << ": " <<  number << " ";
        }
        cout << "}";
    }
    cout << "]" << endl;
}

int main() {
  TeamTasks tasks;
  TasksInfo updated_tasks, untouched_tasks;
  {
    tasks.AddNewTasks("Ivan", 5);
    cout << "Ivan's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

    tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ivan", 5);
    PrintTasksInfo(updated_tasks, untouched_tasks);

    tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ivan", 5);
    PrintTasksInfo(updated_tasks, untouched_tasks);

    tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ivan", 1);
    PrintTasksInfo(updated_tasks, untouched_tasks);

    tasks.AddNewTasks("Ivan", 5);
    cout << "Ivan's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

    tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ivan", 2);
    PrintTasksInfo(updated_tasks, untouched_tasks);

    tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ivan", 4);
    PrintTasksInfo(updated_tasks, untouched_tasks);

    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));
  }
  return 0;
}
