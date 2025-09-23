#ifndef TASKERPLUSPLUS_STORE_H
#define TASKERPLUSPLUS_STORE_H
#include <string>
#include <nlohmann/json_fwd.hpp>

using json = nlohmann::json;

struct Task {
    int id;
    std::string desc;
    bool done;
};

void to_json(json& j, const Task& t);

void from_json(const json& j, Task& t);

std::string get_home_directory();

std::string get_store_path();

void save_tasks(const std::string& filename, const std::vector<Task>& tasks);

std::vector<Task> load_tasks(const std::string& filename);



#endif //TASKERPLUSPLUS_STORE_H