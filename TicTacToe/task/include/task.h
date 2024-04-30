#ifndef TICTACTOE_TASK_H
#define TICTACTOE_TASK_H

#include "unordered_map"
#include "functional"

typedef std::function<bool(void)> taskFunc;

class Task {
private:
    std::unordered_map<int, taskFunc> tasks;
public:
    int createTask(const taskFunc &func);

    void executeTasks();

    void clearTasks();
};


#endif //TICTACTOE_TASK_H
