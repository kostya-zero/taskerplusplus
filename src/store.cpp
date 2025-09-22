#include "store.h"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void to_json(json &j, const Task &t) {
    j = json{{"id", t.id}, {"desc", t.desc}, {"done", t.done}};
}

void from_json(const json &j, Task &t) {
    j.at("id").get_to(t.id);
    j.at("desc").get_to(t.desc);
    j.at("done").get_to(t.done);
}

void save_tasks(const std::string &filename, const std::vector<Task> &tasks) {
    json j = tasks;
    std::ofstream out(filename);
    if (!out) {
        throw std::runtime_error("failed to open file for writing");
    }
    out << j.dump(4);
}

std::vector<Task> load_tasks(const std::string &filename) {
    json j;
    std::ifstream in(filename);
    if (!in) {
        throw std::runtime_error("failed to open file for reading");
    }
    try {
        in >> j;
    } catch (const json::parse_error& e) {
        throw std::runtime_error("json parse failed");
    }
    std::vector<Task> loaded;
    try {
        loaded = j.get<std::vector<Task>>();
    } catch (const json::parse_error& e) {
        throw std::runtime_error("JSON type a error when converting.");
    }
    return loaded;
}