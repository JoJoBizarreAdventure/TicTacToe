#include "task.h"

int Task::createTask(const taskFunc &func) {
    int index = static_cast<int>(tasks.size());
    tasks.emplace(index, func);
    return index;
}

void Task::executeTasks() {
    if (tasks.empty())
        return;

    std::vector<int> toRemoved;
    for (const auto &kv: tasks) {
        auto index = kv.first;
        auto func = kv.second;

        if (func()) {
            toRemoved.push_back(index);
        }
    }

    for (const auto &index: toRemoved) {
        tasks.erase(index);
    }
}

void Task::clearTasks() {
    tasks.clear();
}

