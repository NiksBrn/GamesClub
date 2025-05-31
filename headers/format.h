#ifndef FORMAT_H
#define FORMAT_H

#include <vector>
#include <string>
#include <stdexcept>
#include "struct.h"
#include "modules.h"

// class for checking the format

class Format {
public:
    Format() = default;
    explicit Format(const std::string& filename);
    int get_nums_tabels_ () const;
    std::pair <int, int> get_time_open_close() const;
    int get_cost() const;
    event& get_next_event();

private:
    int nums_tabels_;
    int time_open_, time_close_;
    int cost_;
    std::vector<event> events_;
    std::vector<std::string> lines;
    size_t tec_event = 0;

    void validate();

    bool isValidTableCount(const std::string& line) const;
    bool isValidTimeRange(const std::string& line);
    bool isValidCost(const std::string& line) const;
    bool isValidEventLine(const std::string& line);
    bool isValidClientName(const std::string& name) const;
    bool isValidTime(const std::string& time) const;
};

#endif // FORMAT_H