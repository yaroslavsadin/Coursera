#include <iostream>
#include <string>
#include <tuple>
#include <map>
#include <unordered_map>

using namespace std;

#ifdef LOCAL_TEST
// Перечислимый тип для статуса задачи
enum class TaskStatus {
  NEW,          // новая
  IN_PROGRESS,  // в разработке
  TESTING,      // на тестировании
  DONE          // завершена
};
#endif

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
public:
  // Получить статистику по статусам задач конкретного разработчика
  const TasksInfo& GetPersonTasksInfo(const string& person) const {
    return person_to_tasks_.at(person);
  }
  
  // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
  void AddNewTask(const string& person) {
    person_to_tasks_[person][TaskStatus::NEW]++;
  }
  
  // Обновить статусы по данному количеству задач конкретного разработчика,
  // подробности см. ниже
  tuple<TasksInfo, TasksInfo> PerformPersonTasks(
      const string& person, int task_count) {
        TasksInfo updated;
        TasksInfo current_;
        TasksInfo& current = person_to_tasks_[person];
        for(auto& [task,count] : current) {
          while(count && task_count) {
          if(task == TaskStatus::DONE) break;
          count--;
            updated[
              static_cast<TaskStatus>(
                static_cast<int>(task) + 1
                )
            ]++;
            task_count--;
          }
        }
        CleanUp(current);
        for(const auto& [task,count] : current) {
          if(task != TaskStatus::DONE) {  
            current_[task] = count;
          }
        }
        auto result = make_tuple(updated, current_);
        for(auto& [task,count] : updated) {
          current[task] += count;
        }
        return result;
      }
private:
  unordered_map<string,TasksInfo> person_to_tasks_;  
  void CleanUp(TasksInfo& current) {
    for(int i = static_cast<int>(TaskStatus::DONE); i >= 0; i--) {
      if(current[static_cast<TaskStatus>(i)] == 0) {
        current.erase(static_cast<TaskStatus>(i));
      }
    }
  }
};

#ifdef LOCAL_TEST

int main() {
  TeamTasks dut;
  dut.AddNewTask("Alice");
  dut.AddNewTask("Alice");
  dut.AddNewTask("Alice");
  dut.AddNewTask("Alice");
  dut.AddNewTask("Alice");

  dut.PerformPersonTasks("Alice",5);
  dut.PerformPersonTasks("Alice",5);
  dut.PerformPersonTasks("Alice",1);

  dut.AddNewTask("Alice");
  dut.AddNewTask("Alice");
  dut.AddNewTask("Alice");
  dut.AddNewTask("Alice");
  dut.AddNewTask("Alice");

  dut.PerformPersonTasks("Alice",2);
  dut.PerformPersonTasks("Alice",4);
}

#endif