#include <iostream>

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
    println("  delete [task_id]       Delete a task");
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

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_help();
        return 1;
    }

    Command command = stringToEnum(argv[0]);

    switch (command) {
    case Command::ADD:
        println("Add");
        break;
    case Command::LIST:
        println("List");
        break;
    case Command::DONE:
        println("Done");
        break;
    case Command::DELETE:
        println("Delete");
        break;
    case Command::HELP:
        println("Help");
        break;
    case Command::VERSION:
        println("Version");
        break;
    case Command::UNKNOWN:
        println("Unknown");
        break;
    }

    return 0;
}
