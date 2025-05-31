#ifndef STRUCT_H
#define STRUCT_H
#include <string>

enum tabael_status
{
    busy = 0,
    free_ = 1
};

struct tabel
{
    tabael_status status = free_;
    int sum = 0;
    int time = 0;
    int total_time = 0;
    tabel(tabael_status status = free_, int sum = 0, int time = 0)
        : status(status), sum(sum), time(time) {}
    friend std::ostream& operator<<(std::ostream& os, const tabel& t);
};

enum client_status
{
    into = 202,
    outside = 303,
    none = 505,
};

struct client
{
    std::string name;
    client_status client_status_;
    int num_tabel;
    client(std::string name_ = "", client_status client_status_ = none, int num_tabel_ = 0)
        :  name(name_), client_status_(client_status_), num_tabel(num_tabel_){}
};

enum id_event
{
    some = 0,

    arrive_cl = 1,
    take_cl = 2,
    wait_cl = 3,
    out_cl = 4,

    out_sys = 11,
    take_sys = 12,
    error_sys = 13,

    end = 404,

};

struct event
{
    int time_;
    id_event id;
    client client_;
    int num_table;
    event(int time = 0, id_event id = end, client cl = client(), int num_table_ = 0)
        : time_(time), id(id), client_(cl), num_table(num_table_) {}
    friend std::ostream& operator<<(std::ostream& os, const event& e);
};

enum error_status
{
    YouShallNotPass,
    NotOpenYet,
    PlaceIsBusy,
    ClientUnknown,
    ICanWaitNoLonger,

};
#endif //STRUCT_H
