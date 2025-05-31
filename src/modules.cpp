#include "../headers/modules.h"
#include "../headers/struct.h"

int timeToMinutes(const std::string& time) {
    size_t colonPos = time.find(':');
    int hours = std::stoi(time.substr(0, colonPos));
    int minutes = std::stoi(time.substr(colonPos + 1));
    
    return hours * 60 + minutes;
}


std::string minutesToTime(int total_minutes) {
    int hours = total_minutes / 60;
    int minutes = total_minutes % 60;
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << minutes;

    return oss.str();
}
// output event
std::ostream& operator<<(std::ostream& os, const event& e) {
    os << minutesToTime(e.time_)<<' '<< e.id << " " << e.client_.name;
    if (e.num_table != 0) {
        os << " " << e.num_table;
    }
    return os;
}
//output tabel
std::ostream& operator<<(std::ostream& os, const tabel& t){
    os << t.sum << ' '<< minutesToTime(t.total_time);
    return os;
}