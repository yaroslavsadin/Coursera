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

// Перечислимый тип для статуса задачи
enum class TaskStatus {
  NEW,          // новая
  IN_PROGRESS,  // в разработке
  TESTING,      // на тестировании
  DONE          // завершена
};

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

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

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
  
  // Обновить статусы по данному количеству задач конкретного разработчика,
  // подробности см. ниже
  tuple<TasksInfo, TasksInfo> PerformPersonTasks(
      const string& person, int task_count) {
        TasksInfo modified;
        TasksInfo untouched = person_tasks.at(person);

        for(const auto& [key,value] : person_tasks.at(person)) {
            if(key == TaskStatus::DONE) { break; }
            while(task_count-- && (++modified[TaskStatusNext(key)] < value)) {}
        }
        for(const auto& [key,value] : modified) {
            auto& person_list = person_tasks.at(person);
            person_list[key] += value;
            person_list[TaskStatusPrev(key)] -= value;
        }

        return make_tuple(modified,untouched);
      }

private:
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

int main() {
  TeamTasks tasks;
  tasks.AddNewTask("Ilia");
  for (int i = 0; i < 3; ++i) {
    tasks.AddNewTask("Ivan");
  }
  cout << "Ilia's tasks: ";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
  cout << "Ivan's tasks: ";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));
  
  TasksInfo updated_tasks, untouched_tasks;
  
  tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ivan", 2);
  cout << "Updated Ivan's tasks: ";
  PrintTasksInfo(updated_tasks);
  cout << "Untouched Ivan's tasks: ";
  PrintTasksInfo(untouched_tasks);
  
  tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ivan", 2);
  cout << "Updated Ivan's tasks: ";
  PrintTasksInfo(updated_tasks);
  cout << "Untouched Ivan's tasks: ";
  PrintTasksInfo(untouched_tasks);

  return 0;
}
