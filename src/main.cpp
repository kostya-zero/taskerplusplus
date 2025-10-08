#include <iostream>
#include <nlohmann/json.hpp>
#include <tl/expected.hpp>
#ifdef WIN32
#include <windows.h>
#endif

#include "store.h"

const char *TASKER_VERSION = "0.1.0";

void println(const char *str) { std::cout << str << std::endl; }

void print_success(const char *str) {
    std::cout << " \x1b[92m\xE2\x9C\x93\x1b[0m " << str << std::endl;
}

void eprintln(const char *str) { std::cerr << " Error: " << str << std::endl; }

void eprintln(const std::string &str) {
    std::cerr << " \x1b[91mError:\x1b[0m " << str << std::endl;
}

void print_help() {
    println("Tasker++ - A better minimalist tasks manager");
    println("Usage: tasker [command] [options]\n");
    println("Commands:");
    println("  add [description]      Add a new task");
    println("  list                   List all tasks");
    println("  check [task_id]        Mark a task as finished or not finished");
    println("  remove [task_id]       Remove a task");
    println("  help                   Show this help message");
    println("  version                Show version information");
}

enum class Command { ADD, LIST, CHECK, REMOVE, HELP, VERSION, UNKNOWN };

Command stringToEnum(const std::string &command) {
    if (command == "add")
        return Command::ADD;
    if (command == "list")
        return Command::LIST;
    if (command == "check")
        return Command::CHECK;
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
    Tasks tasks = Tasks();
    auto load_result = tasks.load_tasks(store_path);
    if (!load_result) {
        eprintln(load_result.error());
        exit(1);
    }

    tasks.add(desc);

    auto save_result = tasks.save_tasks(store_path);
    if (!save_result) {
        eprintln(save_result.error());
        exit(1);
    }

    print_success("Added.");
}

void list_command() {
    const std::string store_path = get_store_path();
    Tasks store = Tasks();
    auto load_result = store.load_tasks(store_path);
    if (!load_result) {
        eprintln(load_result.error());
        exit(1);
    }

    auto tasks = store.get_tasks();

    if (tasks.empty()) {
        println("No tasks at the moment.");
        return;
    }

    // Sort tasks by id
    std::sort(tasks.begin(), tasks.end(),
              [](const Task &a, const Task &b) { return a.id < b.id; });

    for (const auto &task : tasks) {
        std::cout << "[" << (task.done ? "x" : " ") << "] " << task.id << ": "
                  << task.desc << std::endl;
    }
}

void remove_command(const int &id) {
    const std::string store_path = get_store_path();
    Tasks tasks = Tasks();
    auto load_result = tasks.load_tasks(store_path);
    if (!load_result) {
        eprintln(load_result.error());
        exit(1);
    }

    auto remove_result = tasks.remove(id);
    if (!remove_result) {
        eprintln(remove_result.error());
        exit(1);
    }

    auto save_result = tasks.save_tasks(store_path);
    if (!save_result) {
        eprintln(save_result.error());
        exit(1);
    }
    print_success("Removed.");
}

void check_command(const int &id) {
    const std::string store_path = get_store_path();
    Tasks tasks = Tasks();
    auto load_result = tasks.load_tasks(store_path);
    if (!load_result) {
        eprintln(load_result.error());
        exit(1);
    }

    auto check_result = tasks.check(id);
    if (!check_result) {
        eprintln(check_result.error());
        exit(1);
    }

    auto save_result = tasks.save_tasks(store_path);
    if (!save_result) {
        eprintln(save_result.error());
        exit(1);
    }
    print_success("Checked.");
}

int main(const int argc, char *argv[]) {
    // Required to properly print unicode
#ifdef WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    if (argc < 2) {
        print_help();
        return 1;
    }

    const Command command = stringToEnum(argv[1]);
    ;
    switch (command) {
    case Command::ADD:
        if (argc < 3) {
            eprintln("Description is required.");
            return 1;
        }
        if (argc > 3) {
            eprintln("Too many arguments");
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
    case Command::CHECK:
        if (argc < 3) {
            eprintln("ID is required.");
            return 1;
        }
        if (argc > 3) {
            eprintln("Too many arguments");
            return 1;
        }

        {
            try {
                const auto id_string = std::string(argv[2]);
                const auto id = std::stoi(id_string);
                check_command(id);
            } catch (const std::invalid_argument &e) {
                eprintln("ID is not a number.");
                return 1;
            } catch (const std::out_of_range &e) {
                eprintln("ID is out of range.");
                return 1;
            }
        }
        break;
    case Command::REMOVE:
        if (argc < 3) {
            eprintln("ID is required.");
            return 1;
        }
        if (argc > 3) {
            eprintln("Too many arguments");
            return 1;
        }

        {
            try {
                const auto id_string = std::string(argv[2]);
                const auto id = std::stoi(id_string);
                remove_command(id);
            } catch (const std::invalid_argument &e) {
                eprintln("ID is not a number.");
                return 1;
            } catch (const std::out_of_range &e) {
                eprintln("ID is out of range.");
                return 1;
            }
        }
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
