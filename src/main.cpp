#include <iostream>
#include <nlohmann/json.hpp>

const char *TASKER_VERSION = "0.1.0";

struct Task {
    int id;
    std::string desc;
    bool done;
};

void println(const char *str) { std::cout << str << std::endl; }

void print_help() {
    println("Tasker++ - A better minimalist tasks manager");
    println("Usage: tasker [command] [options]\n");
    println("Commands:");
    println("  add [description]      Add a new task");
    println("  list                   List all tasks");
    println("  done [task_id]         Mark a task as done");
    println("  remove [task_id]       Delete a task");
    println("  help                   Show this help message");
    println("  version                Show version information");
}

enum class Command { ADD, LIST, DONE, DELETE, HELP, VERSION, UNKNOWN };

Command stringToEnum(const std::string &command) {
    if (command == "add")
        return Command::ADD;
    if (command == "list")
        return Command::LIST;
    if (command == "done")
        return Command::DONE;
    if (command == "delete")
        return Command::DELETE;
    if (command == "help")
        return Command::HELP;
    if (command == "version")
        return Command::VERSION;
    return Command::UNKNOWN;
}

void add_command() {
    println("Add");
}

void list_command() {
    println("List");
}

void done_command() {
    println("Done");
}

void delete_command() {
    println("Delete");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_help();
        return 1;
    }

    Command command = stringToEnum(argv[1]);

    switch (command) {
    case Command::ADD:
        add_command();
        break;
    case Command::LIST:
        list_command();
        break;
    case Command::DONE:
        done_command();
        break;
    case Command::DELETE:
        delete_command();
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
