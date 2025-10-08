#include "store.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <tl/expected.hpp>

using result = tl::expected<tl::monostate, std::string>;
using json = nlohmann::json;

Tasks::Tasks() : tasks() {}

result Tasks::load_tasks(const std::string &filename) {
    json j;
    std::ifstream in(filename);
    if (!in.good()) {
        return tl::monostate();
    }
    if (!in) {
        return tl::unexpected("failed to open file for reading");
    }
    try {
        in >> j;
    } catch (const json::parse_error &e) {
        return tl::unexpected("json parse failed");
    }
    std::vector<Task> loaded;
    try {
        loaded = j.get<std::vector<Task>>();
    } catch (const json::parse_error &e) {
        return tl::unexpected("failed to convert data from JSON.");
    }
    this->tasks = loaded;
    return tl::monostate();
}

result Tasks::save_tasks(const std::string &filename) {
    json j = tasks;
    std::ofstream out(filename);
    if (!out) {
        return tl::unexpected("failed to open file for writing");
    }
    out << j.dump(4);
    return tl::monostate();
}

std::vector<Task> Tasks::get_tasks() { return this->tasks; }

void Tasks::add(const std::string &desc) {
    int next_id = 0;
    for (const auto &task : this->tasks) {
        if (task.id > next_id) {
            next_id = task.id;
        }
    }
    Task task;
    task.id = next_id + 1;
    task.desc = desc;
    task.done = false;

    this->tasks.push_back(task);
}

result Tasks::check(const int &id) {
    std::optional<size_t> task_index;
    for (size_t i = 0; i < this->tasks.size(); ++i) {
        if (this->tasks[i].id == id) {
            task_index = i;
        }
    }
    if (!task_index.has_value()) {
        return tl::unexpected("task not found");
    }

    this->tasks[task_index.value()].done =
        !this->tasks[task_index.value()].done;

    return tl::monostate();
}

result Tasks::remove(const int &id) {
    std::optional<size_t> task_index;
    for (size_t i = 0; i < this->tasks.size(); ++i) {
        if (this->tasks[i].id == id) {
            task_index = i;
        }
    }
    if (!task_index.has_value()) {
        return tl::unexpected("no tasks found");
    }

    this->tasks.erase(this->tasks.begin() + task_index.value());

    return tl::monostate();
}

void to_json(json &j, const Task &t) {
    j = json{{"id", t.id}, {"desc", t.desc}, {"done", t.done}};
}

void from_json(const json &j, Task &t) {
    j.at("id").get_to(t.id);
    j.at("desc").get_to(t.desc);
    j.at("done").get_to(t.done);
}

std::string get_home_directory() {
#if defined(_WIN32)
    const char *home = std::getenv("USERPROFILE");
    if (!home) {
        home = std::getenv("HOME");
    }
#else
    const char *home = std::getenv("HOME")
#endif
    if (home) {
        return {home};
    }
    return {};
}

std::string get_store_path() {
    std::string home = get_home_directory();
    if (home.empty()) {
        return {};
    }
#if defined(_WIN32)
    if (home.back() != '\\')
        home += '\\';
#else
    if (home.back() != '/')
        home += '/';
#endif
    return home + "tasker_store.json";
}
