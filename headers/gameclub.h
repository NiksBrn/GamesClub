#ifndef GAMECLUB_H
#define GAMECLUB_H
#include <map>
#include <queue>
#include <vector>
#include "format.h"
#include "struct.h"
//  class simulate games club
//  loop: init -> start -> end
//  init: input file + check format file (class format)
//  start: processing events
//  end: club close

class GameClub{
public:
  explicit GameClub(const std::string& filename);
  void print_event(const event& event);
  void start();
  void end();
private:
  Format format_;
  std::map<std::string, client> clients;
  std::vector<tabel> tables;
  int time_open = 0;
  int time_close = 0;
  int cost = 0;

  void gen_error_event(int time, error_status error);
  void gen_pop_queue(int& tec_time);
  void stop_tabel(size_t num_tabel, int& tec_time);
  void start_tabel(size_t num_table, int& tec_time);
  void gen_out_event(int time, const std::string& name);
  void print_time(int time);

  int total_free_tabel = 0;
  // queue clients
  std::queue<std::string> queue_;
};
#endif //GAMECLUB_H
