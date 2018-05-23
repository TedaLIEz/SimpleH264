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
  static std::unique_ptr<Context> ctx;
  std::map<int, Sps> sps_table;
  Context() {}
 public:

  static Context& getInstance() {
    if (ctx.get()  == nullptr) {
      ctx.reset(new Context());
    }
    return *ctx;
  }

  const Sps* lookup_sps_table(int id);

  void add_sps(Sps sps);



};
#endif //SIMPLEH264_CONTEXT_H
