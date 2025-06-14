#include <easylogging++.h>
INITIALIZE_EASYLOGGINGPP

#include "world.h"

int main(int argc, char *argv[]) {
  el::Configurations conf("easyloggingpp.conf");
  el::Loggers::reconfigureAllLoggers(conf);
  {
    al::World world(8);

    for (int i = 0; i < 37; ++i) {
      world.update();
    }
  }
  return 0;
}
