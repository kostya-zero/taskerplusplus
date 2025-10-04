#include <iostream>
#include <nlohmann/json.hpp>

#include "store.h"

const char *TASKER_VERSION = "0.1.0";

void println(const char *str) { std::cout << str << std::endl; }

void eprintln(const char *str) { std::cerr << str << std::endl; }

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
      std::cerr << "Failed to save tasks: " << e2.what() << std::endl;
      exit(1);
    }
  } catch (const std::runtime_error &e1) {
    std::cerr << "Failed to read tasks: " << e1.what() << std::endl;
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
    std::sort(tasks.begin(), tasks.end(),
              [](const Task &a, const Task &b) { return a.id < b.id; });

    for (const auto &task : tasks) {
      std::cout << "[" << (task.done ? "x" : " ") << "] " << task.id << ": "
                << task.desc << std::endl;
    }
  } catch (const std::runtime_error &e) {
    std::cout << "Failed to read tasks: " << e.what() << std::endl;
    exit(1);
  }
}

void remove_command(const int &id) {
  const std::string store_path = get_store_path();
  try {
    std::vector<Task> tasks = load_tasks(store_path);
    if (tasks.empty()) {
      println("No tasks at the moment.");
      return;
    }

    std::optional<size_t> task_index;
    for (size_t i = 0; i < tasks.size(); ++i) {
      if (tasks[i].id == id) {
        task_index = i;
      }
    }
    if (!task_index.has_value()) {
      eprintln("Task not found");
      exit(1);
    }

    tasks.erase(tasks.begin() + task_index.value());

    try {
      save_tasks(store_path, tasks);
      println("Removed.");
    } catch (const std::runtime_error &e2) {
      std::cerr << "Failed to save tasks: " << e2.what() << std::endl;
      exit(1);
    }
  } catch (const std::runtime_error &e1) {
    std::cerr << "Failed to read tasks: " << e1.what() << std::endl;
    exit(1);
  }
}

void check_command(const int &id) {
  const std::string store_path = get_store_path();
  try {
    std::vector<Task> tasks = load_tasks(store_path);
    if (tasks.empty()) {
      println("No tasks at the moment.");
      return;
    }

    std::optional<size_t> task_index;
    for (size_t i = 0; i < tasks.size(); ++i) {
      if (tasks[i].id == id) {
        task_index = i;
      }
    }
    if (!task_index.has_value()) {
      eprintln("Task not found");
      exit(1);
    }

    // Reversing bool state
    tasks[task_index.value()].done = !tasks[task_index.value()].done;

    try {
      save_tasks(store_path, tasks);
      println("Checked.");
    } catch (const std::runtime_error &e2) {
      std::cerr << "Failed to save tasks: " << e2.what() << std::endl;
      exit(1);
    }
  } catch (const std::runtime_error &e1) {
    std::cerr << "Failed to read tasks: " << e1.what() << std::endl;
    exit(1);
  }
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
  case Command::CHECK:
    if (argc < 3) {
      println("ID is required.");
      return 1;
    }
    if (argc > 3) {
      println("Too many arguments");
      return 1;
    }

    {
      try {
        const auto id_string = std::string(argv[2]);
        const auto id = std::stoi(id_string);
        check_command(id);
      } catch (const std::invalid_argument &e) {
        std::cerr << "ID is not a number" << std::endl;
        return 1;
      } catch (const std::out_of_range &e) {
        std::cerr << "ID is out of range." << std::endl;
        return 1;
      }
    }
    break;
  case Command::REMOVE:
    if (argc < 3) {
      println("ID is required.");
      return 1;
    }
    if (argc > 3) {
      println("Too many arguments");
      return 1;
    }

    {
      try {
        const auto id_string = std::string(argv[2]);
        const auto id = std::stoi(id_string);
        remove_command(id);
      } catch (const std::invalid_argument &e) {
        std::cerr << "ID is not a number" << std::endl;
        return 1;
      } catch (const std::out_of_range &e) {
        std::cerr << "ID is out of range." << std::endl;
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
