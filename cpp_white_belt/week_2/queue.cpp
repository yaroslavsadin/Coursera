#include <iostream>
#include <vector>
#include <string>
using namespace std;

enum person {quiet, worry};

vector<person> queue;

struct Command {
    string name;
    int args;
};

bool ValidCommand(string name) {
    if (name == "COME" || name == "WORRY" || name == "WORRY_COUNT" || name == "QUIET")
        return true;
    return false;
}

Command GetCommand(void) {
    Command command;
    cin >> command.name;
    if (!ValidCommand(command.name)) return {"NOT_VAILD",-1};
    if (command.name == "WORRY_COUNT") {
        command.args = 0;
    } else {
        cin >> command.args;
    }
    return command;
}

void ServeCommand(const Command& command) {
    if (command.name == "COME") {
        if (command.args >= 0) {
            queue.resize(queue.size()+command.args,quiet);
        } else {
            queue.resize(queue.size()+command.args);
        }
    } else if (command.name == "QUIET") {
        queue[command.args] = quiet;
    } else if (command.name == "WORRY") {
        queue[command.args] = worry;
    } else if (command.name == "WORRY_COUNT") {
        int worry_count = 0;
        for(auto person : queue) {
            if (person == worry) worry_count++;
        }
        cout << worry_count << endl;
    } else {
        return;
    }
}

int main (void) {
    int num_ops;
    cin >> num_ops;

    for (int i = num_ops; i > 0; i--) {
        ServeCommand(GetCommand());
    }

    return 0;
}