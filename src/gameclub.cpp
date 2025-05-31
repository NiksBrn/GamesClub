#include "../headers/format.h"
#include <fstream>
#include <regex>
#include <iostream>
#include "../headers/gameclub.h"
#include "../headers/modules.h"


GameClub::GameClub(const std::string& filename)
{
    try {
        format_ = Format(filename);
        std::pair<int, int> open_close_times = format_.get_time_open_close();
        time_open = open_close_times.first;
        time_close = open_close_times.second;
        cost = format_.get_cost();
        tables.resize(format_.get_nums_tabels_());
        total_free_tabel = tables.size();
        // std::cout << "Table size: " << format_.get_nums_tabels_() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(1);
    }
}

void GameClub::print_event(const event& event)
{
    if (event.id != 404)
    std::cout << event<<std::endl;
}

void GameClub::gen_pop_queue(int &tec_time)
{
    client &client_ = clients[queue_.front()];
    queue_.pop();
    for (size_t free_tabel=0; free_tabel < tables.size(); free_tabel++)
    {
        if (tables[free_tabel].status == free_)
        {
            start_tabel(free_tabel+1, tec_time);
            clients[client_.name].num_tabel = free_tabel+1;
            std::cout <<  minutesToTime(tec_time) <<' '<< take_sys << ' '<<client_.name <<' '<< free_tabel+1 << std::endl;
            break;
        }
    }
}

void GameClub::stop_tabel(size_t num_tabel, int &tec_time)
{
    if (num_tabel != 0)
    {
        tables[num_tabel-1].status = free_;
        tables[num_tabel-1].sum += ((tec_time - tables[num_tabel-1].time + 59) / 60) * cost;
        tables[num_tabel-1].total_time+= tec_time - tables[num_tabel-1].time;
        total_free_tabel++;
        if (total_free_tabel >= 1 && !queue_.empty()) gen_pop_queue(tec_time);
    }
}

void GameClub::start_tabel(size_t num_table, int &tec_time)
{
    tables[num_table-1].status = busy;
    tables[num_table-1].time = tec_time;
    total_free_tabel--;
}

void GameClub::gen_out_event(int time, const std::string& name)
{
    std::cout << minutesToTime(time) << " "<< out_sys << ' '<< name << std::endl;
    clients[name].client_status_ = outside;
}

void GameClub::print_time(int time)
{
    std::cout << minutesToTime(time) << std::endl;
}

void GameClub::start()
{
    print_time(time_open);

    bool flag_end = false;
    while (!flag_end)
    {
        event &event_ = format_.get_next_event();
        print_event(event_);
        switch (event_.id)
        {
        case 1:
            if (event_.time_ < time_open || event_.time_ > time_close)
            {
                gen_error_event(event_.time_, NotOpenYet);
            }
            else {
                if (clients.find(event_.client_.name) == clients.end())
                {
                    clients[event_.client_.name] = client(
                event_.client_.name
                );
                }
                if (clients[event_.client_.name].client_status_ == into)
                {
                    gen_error_event(event_.time_, YouShallNotPass);
                }else
                {
                    clients[event_.client_.name].client_status_ = into;

                }
            }
            break;
        case 2:
            if (clients[event_.client_.name].client_status_ != into)
            {
                gen_error_event(event_.time_, ClientUnknown);
            }else if(tables[event_.num_table-1].status == busy)
            {
                gen_error_event(event_.time_, PlaceIsBusy);
            }else
            {
                stop_tabel(clients[event_.client_.name].num_tabel, event_.time_);
                start_tabel(event_.num_table, event_.time_);
                clients[event_.client_.name].num_tabel = event_.num_table;
            }
            break;
        case 3:
            if (total_free_tabel != 0)
            {
                gen_error_event(event_.time_, ICanWaitNoLonger);
            }else
            {
                if(queue_.size() + 1 > tables.size())
                    gen_out_event(event_.time_, event_.client_.name);
                else
                {
                    queue_.push(event_.client_.name);
                }
            }
            break;
        case 4:
            if (clients[event_.client_.name].client_status_ != into)
            {
                gen_error_event(event_.time_, ClientUnknown);
            }else
            {
                stop_tabel(clients[event_.client_.name].num_tabel, event_.time_);
                clients[event_.client_.name].client_status_ = outside;
            }
            break;
        case 404:
            flag_end = true;
            break;
        }
    }
}

void GameClub::end()
{
    for (auto &g:clients)
    {
        if (g.second.client_status_ == into)
        {
            gen_out_event(time_close, g.second.name);
        }
    }
   print_time(time_close);
    for (size_t i = 0; i < tables.size(); i++)
    {
        if (tables[i].status == busy) {stop_tabel(i+1, time_close);}
        std::cout << i+1 <<' '<< tables[i] << std::endl;
    }
}

void GameClub::gen_error_event(int time, error_status error)
{
    std::cout << minutesToTime(time) << ' ' << error_sys <<' ';
    switch (error){
    case NotOpenYet:
        std::cout << "NotOpenYet\n";
        break;
    case YouShallNotPass:
        std::cout << "YouShallNotPass\n";
        break;
    case PlaceIsBusy:
        std::cout << "PlaceIsBusy\n";
        break;
    case ClientUnknown:
        std::cout << "ClientUnknown\n";
        break;
    case ICanWaitNoLonger:
        std::cout << "ICanWaitNoLonger!\n";
        break;
    }
}


