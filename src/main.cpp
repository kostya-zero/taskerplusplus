#include <iostream>
#include <nlohmann/json.hpp>

#include "store.h"

const char *TASKER_VERSION = "0.1.0";

void println(const char *str) { std::cout << str << std::endl; }

void print_help() {
    println("Tasker++ - A better minimalist tasks manager");
    println("Usage: tasker [command] [options]\n");
    println("Commands:");
    println("  add [description]      Add a new task");
    println("  list                   List all tasks");
    println("  done [task_id]         Mark a task as done");
    println("  remove [task_id]       Remove a task");
    println("  help                   Show this help message");
    println("  version                Show version information");
}

enum class Command { ADD, LIST, DONE, REMOVE, HELP, VERSION, UNKNOWN };

Command stringToEnum(const std::string &command) {
    if (command == "add")
        return Command::ADD;
    if (command == "list")
        return Command::LIST;
    if (command == "done")
        return Command::DONE;
    if (command == "remove")
        return Command::REMOVE;
    if (command == "help")
        return Command::HELP;
    if (command == "version")
        return Command::VERSION;
    return Command::UNKNOWN;
}

void add_command(const std::string &desc) {
    const std::string store_path = get_store_path();
    try {
        std::vector<Task> tasks = load_tasks(store_path);
        int next_id = 0;
        for (const auto &task : tasks) {
            if (task.id > next_id) {
                next_id = task.id;
            }
        }
        Task task;
        task.id = next_id + 1;
        task.desc = desc;
        task.done = false;

        tasks.push_back(task);

        try {
            save_tasks(store_path, tasks);
            println("Added.");
        } catch (const std::runtime_error &e2) {
            std::cout << "Failed to save tasks: " << e2.what() << std::endl;
            exit(1);
        }
    } catch (const std::runtime_error &e1) {
        std::cout << "Failed to read tasks: " << e1.what() << std::endl;
        exit(1);
    }
}

void list_command() {
    const std::string store_path = get_store_path();
    try {
        std::vector<Task> tasks = load_tasks(store_path);
        if (tasks.empty()) {
            println("No tasks at the moment.");
            return;
        }

        // Sort tasks by id
        std::sort(tasks.begin(), tasks.end(), [](const Task &a, const Task &b) {
            return a.id < b.id;
        });

        for (const auto &task: tasks) {
            std::cout << "[" << (task.done ? "x" : " ") << "] "
                    << task.id << ": " << task.desc << std::endl;
        }
    } catch (const std::runtime_error &e) {
        std::cout << "Failed to read tasks: " << e.what() << std::endl;
        exit(1);
    }
}

void remove_command() {
    println("Remove");
}

void done_command() {
    println("Done");
}

int main(const int argc, char *argv[]) {
    if (argc < 2) {
        print_help();
        return 1;
    }

    const Command command = stringToEnum(argv[1]);

    switch (command) {
        case Command::ADD:
            if (argc < 3) {
                println("Description is required.");
                return 1;
            }
            if (argc > 3) {
                println("Too many arguments");
                return 1;
            }

            {
                const auto desc = std::string(argv[2]);
                add_command(desc);
            }
            break;
        case Command::LIST:
            list_command();
            break;
        case Command::DONE:
            done_command();
            break;
        case Command::REMOVE:
            remove_command();
            break;
        case Command::HELP:
            print_help();
            break;
        case Command::VERSION:
            println(TASKER_VERSION);
            break;
        case Command::UNKNOWN:
            println("Unknown command. Use `tasker --help` for help.");
            break;
    }

    return 0;
}
