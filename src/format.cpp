#include "../headers/format.h"
#include <fstream>
#include <sstream>
#include <regex>
#include <algorithm>
#include <iostream>

using namespace std;

Format::Format(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + filename);
    }

    string line;
    while (getline(file, line)) {
        lines.push_back(line);

    }

    if (lines.empty()) {
        throw runtime_error("File is empty");
    }

    validate();
}

void Format::validate() {
    if (lines.size() < 3) {
        throw runtime_error("Not enough lines in the file");
    }

    // nums tabels
    if (!isValidTableCount(lines[0])) {
        throw runtime_error("Invalid number of tables in line 1\n" + lines[0]);
    }

    nums_tabels_ = stoi(lines[0]);

    // time
    if (!isValidTimeRange(lines[1])) {
        throw runtime_error("Invalid time format in line 2\n" + lines[1]);
    }



    // cost
    if (!isValidCost(lines[2])) {
        throw runtime_error("Invalid cost per hour in line 3\n" + lines[2]);
    }

    cost_ = stoi(lines[2]);

    // events
    for (size_t i = 3; i < lines.size(); ++i) {
        if (!isValidEventLine(lines[i])) {
            throw runtime_error("Invalid event format in line " + to_string(i + 1) + "\n" + lines[i]);
        }

        // checking the chronological order
        if (i > 3) {
            string prev_time = lines[i-1].substr(0, 5);
            string current_time = lines[i].substr(0, 5);
            if (current_time < prev_time) {
                throw runtime_error("Events not in chronological order in line " + to_string(i + 1) + "\n" + lines[i]);
            }
        }
    }
    events_.push_back(event());
}

bool Format::isValidTableCount(const string& line) const {
    return regex_match(line, regex("^\\d+$")) && stoi(line) > 0;
}

bool Format::isValidTimeRange(const string& line)
{
    if (!regex_match(line, regex("^([01]\\d|2[0-3]):([0-5]\\d) ([01]\\d|2[0-3]):([0-5]\\d)$"))) {
        return false;
    }

    istringstream iss(line);
    string open_time, close_time;
    iss >> open_time >> close_time;
    time_open_ = timeToMinutes(open_time);
    time_close_ = timeToMinutes(close_time);
    return open_time < close_time;
}

bool Format::isValidCost(const string& line) const {
    return regex_match(line, regex("^\\d+$")) && stoi(line) > 0;
}

bool Format::isValidEventLine(const string& line) {
    istringstream iss(line);
    string time, id, client_name, table_number_str;

    event event_;

    if (!(iss >> time >> id >> client_name)) {
        return false;
    }

    if (!isValidTime(time)) {
        return false;
    }

    if (!regex_match(id, regex("^[1-4]$"))) {
        return false;
    }

    if (!isValidClientName(client_name)) {
        return false;
    }

    event_.time_ = timeToMinutes(time);
    event_.client_ = client(client_name);
    event_.id = id_event(std::stoi(id));

    if (iss >> table_number_str) {
        if (!regex_match(table_number_str, regex("^[1-9][0-9]*$"))) {
            return false;
        }
        event_.num_table = stoi(table_number_str);
    }


    string extra;
    if (iss >> extra) {
        return false;
    }
    events_.push_back(event_);
    return true;
}

bool Format::isValidClientName(const string& name) const {
    return regex_match(name, regex("^[a-z0-9_-]+$"));
}

bool Format::isValidTime(const string& time) const {
    return regex_match(time, regex("^([01]\\d|2[0-3]):([0-5]\\d)$"));
}

int Format::get_nums_tabels_ () const
{
    return nums_tabels_;
}
std::pair <int, int> Format::get_time_open_close() const
{
    return std::make_pair(time_open_, time_close_);
}

int Format::get_cost() const
{
    return cost_;
}

event& Format::get_next_event()
{
    return events_[tec_event++];
}