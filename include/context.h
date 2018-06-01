//
// Created by aLIEzTed on 5/23/18.
//

#ifndef SIMPLEH264_CONTEXT_H
#define SIMPLEH264_CONTEXT_H
#include <map>
#include <h264/sps.h>
#include <h264/pps.h>
#include <memory>
class Context {
 private:
  static Context* ctx;
  std::map<int, SPS> sps_table;
  std::map<int, PPS> pps_table;
  Context() {}
 public:

  static Context *getInstance() {
    if (ctx == nullptr) {
      ctx = new Context();
    }
    return ctx;
  }

  const SPS& lookup_sps_table(int id);

  const PPS& lookup_pps_table(int id);

  void add_sps(SPS sps);

  void add_pps(PPS pps);

};
#endif //SIMPLEH264_CONTEXT_H
