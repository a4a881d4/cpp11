#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <USRTWorkers.h>
#include <Log.h>
#include <trace.h>
int main( int argc, char *argv[] )
{
  clearLog();
  attachLog(6);
  attachLog(TRACELOG);
  std::USRTWorkers *workers = new std::USRTWorkers( argv[1] );
  workers->mainWorker( workers );
}
