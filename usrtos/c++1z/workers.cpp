#include <usrtworker.hpp>

int main( int argc, char *argv[] )
{
  usrtos::UsrtWorkers *workers = new usrtos::UsrtWorkers( argv[1] );
  workers->mainWorker();
}
