#include <easylogging++.h>
INITIALIZE_EASYLOGGINGPP

#include "world.h"

int main(int argc, char *argv[]) {
  el::Configurations conf("easyloggingpp.conf");
  el::Loggers::reconfigureAllLoggers(conf);
  {
    al::World world(8);

    LOG(INFO) << "If the numbers don't match, we have a RACE CONDITION!";

    for (int i = 0; i < 37; ++i) {
      world.update();
    }
    LOG(INFO) << "If the numbers don't match, we have a RACE CONDITION!";
  }
  return 0;
}
