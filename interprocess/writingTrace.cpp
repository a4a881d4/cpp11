#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "doc_anonymous_mutex_shared_data.hpp"
#include <iostream>
#include <cstdio>
#include <string>

using namespace boost::interprocess;

int main (int argc, char *argv[])
{
   if(argc!=2) {
      std::cout << "usage: " << argv[0] << " a|b" << std::endl;
      return 1;
   }
   typedef void(*SetF)(struct shared_memory_log*);
   typedef bool(*CheckF)(struct shared_memory_log*);
   
   SetF seta = [](struct shared_memory_log* x) 
      { x->end_a = true; };
   decltype(seta) setb = [](struct shared_memory_log* x) 
      { x->end_b = true; };
   CheckF checka = [](struct shared_memory_log* x) -> bool 
      { return x->end_a; };
   decltype(checka) checkb = [](struct shared_memory_log* x) -> bool 
      { return x->end_b; };

   decltype(seta) setself;
   decltype(checka) checkother;

   auto dump = [](struct shared_memory_log* x) {
      std::cout << "end_a " << x->end_a 
         << " end_b " << x->end_b 
         << " line " << x->current_line
         << std::endl;
      for(auto i = 0; i< x->current_line && i<shared_memory_log::NumItems; i+=10 ) {
         std::cout << i << " "
            << x->items[i]
            << std::endl;
      }
   };

   bool Master;
   if(std::string(argv[1])=="a") {
      setself = seta;
      checkother = checkb;
      Master = true;
   } else {
      setself = setb;
      checkother = checka;
      Master = false;
   }
   try{
      //Remove shared memory on construction and destruction
      struct shm_remove
      {
         //shm_remove() { shared_memory_object::remove("MySharedMemory"); }
         ~shm_remove(){ shared_memory_object::remove("MySharedMemory"); }
      } remover;

      //Create a shared memory object.
      /*
      if(Master) {
         shared_memory_object shm
            (open_or_create               //only create
            ,"MySharedMemory"          //name
            ,read_write   //read-write mode
            );
      } else {
         shared_memory_object shm
            (open_only               //only create
            ,"MySharedMemory"          //name
            ,read_write   //read-write mode
            );
      }
      */
      shared_memory_object shm
            (open_or_create               //only create
            ,"MySharedMemory"          //name
            ,read_write   //read-write mode
            );
      //Set size
      if(Master)
         shm.truncate(sizeof(shared_memory_log));

      //Map the whole shared memory in this process
      mapped_region region
         (shm                       //What to map
         ,read_write   //Map it as read-write
         );

      //Get the address of the mapped region
      void * addr       = region.get_address();

      //Construct the shared structure in memory
      shared_memory_log * data;
      if(Master)
         data = new (addr) shared_memory_log;
      else
         data = static_cast<shared_memory_log*>(addr);

      //Write some logs
      for(int i = 0; i < shared_memory_log::NumItems; ++i){
         //Lock the mutex
         scoped_lock<interprocess_mutex> lock(data->mutex);
         std::sprintf(data->items[(data->current_line++) % shared_memory_log::NumItems]
                  ,"%s_%s_%d", "process", argv[1], i);
         std::cout << argv[1] << " : " << i << std::endl;
         dump(data);
         if(i == (shared_memory_log::NumItems-1))
            setself(data);
            //data->end_a = true;
         //Mutex is released here
      }

      //Wait until the other process ends
      while(1){
         scoped_lock<interprocess_mutex> lock(data->mutex);
         dump(data);
         if(checkother(data))
            break;
      }
   }
   catch(interprocess_exception &ex){
      std::cout << ex.what() << std::endl;
      return 1;
   }
   return 0;
}