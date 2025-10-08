#ifndef TASKERPLUSPLUS_STORE_H
#define TASKERPLUSPLUS_STORE_H
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <tl/expected.hpp>

using result = tl::expected<tl::monostate, std::string>;
using json = nlohmann::json;

struct Task {
    int id;
    std::string desc;
    bool done;
};

class Tasks {
  private:
    std::vector<Task> tasks;

  public:
    Tasks();

    result load_tasks(const std::string &filename);

    result save_tasks(const std::string &filename);

    std::vector<Task> get_tasks();

    void add(const std::string &desc);

    result check(const int &id);

    result remove(const int &id);
};

void to_json(json &j, const Task &t);

void from_json(const json &j, Task &t);

std::string get_home_directory();

std::string get_store_path();

#endif // TASKERPLUSPLUS_STORE_H
