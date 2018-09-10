#ifndef capability_Bearer_H
#define capability_Bearer_H

#include <capability.hpp>
#include <usrtkey.hpp>

#include <dlfcn.h>
#include <stdio.h>
#include <dirent.h>

namespace usrtos {

class UsrtCapabilityBearer {

  typedef CCapability*(*factroyFunc)();
  typedef uuid(*getKeyFunc)(CCapability* item);
  typedef void(*runFunc)(CCapability* item,void *argv);
  typedef void(*destroyFunc)(CCapability* item);

  private:
    char mName[256];
    int mValid;
    factroyFunc mFactroy;
    getKeyFunc mGetKey;
    runFunc mRun;
    destroyFunc mDestroy;
    void *handle;
    uuid key;
    CCapability* item;

    void init()
    {
      mFactroy = (factroyFunc)getFunc("factroy");
      mRun = (runFunc)getFunc("run");
      mGetKey = (getKeyFunc)getFunc("getKey");
      mDestroy = (destroyFunc)getFunc("destroy");
      if( mValid ) 
          item = mFactroy();
      else
          item = NULL;
      if( mValid )
          key = mGetKey(item);
      else
          key=UsrtKey::keySentinel();
    }
    
    const char *getFileName( const char *n) {
      strcpy(mName,getWorkingDir());
      strcat(mName,"/");
      strcat(mName,n);
      return mName;
    }
    
    const char * getWorkingDir (void) {
      char *workdir;
      if ((workdir = getenv ("USRTWORK")) != NULL)     return workdir;
      return "work";
    };

    void * getFunc(const char *sym) {
      void *func = dlsym(handle, sym);
      const char *error;
      if ((error = dlerror()) != NULL)    {
          fprintf (stderr, "%s\n", error);
          mValid=0;
          return NULL;
      }
      return func;
    };

    int getHandle(const char* lib) {
      const char *path = getFileName(lib);
      handle = dlopen (path, RTLD_LAZY);
      if ( handle==NULL ) {
          fprintf (stderr, "load %s(%s) error : ", lib, path);
          fprintf (stderr, "%s\n", dlerror());
          return -1;
      }
      else
          return 0;
    };

  public:
    UsrtCapabilityBearer( const char* lib )
    {
      mValid=0;
      if( getHandle(lib)==0 ) {
          dlerror();        /* Clear any existing error */
          mValid=1;
          init();
      }
    };

    UsrtCapabilityBearer(uuid k) {
      mValid=0;
      std::string ks = UsrtKey::key2string(k);
          
      const char *workdir = getWorkingDir();
      struct dirent* entry;
      DIR* dir = opendir(workdir);
      while((entry = readdir(dir)) != NULL) {
        dlerror();
        if(strncmp(entry->d_name,"lib",3) != 0)
          continue;
        handle = dlopen(getFileName(entry->d_name), RTLD_LAZY);
        if(!handle) {
          fprintf(stderr," check lib %s fail\n",entry->d_name);
          fprintf (stderr, "%s:%s\n", ks.c_str(), dlerror());
          continue;
        }
        mValid=1;
        init();
        if( key==k )
          break;
        mValid=0;
        dlclose(handle);
      }
      if(key != k) {
        std::cout << ks << " no find " << std::endl;
      }
    };

    void runLP(void* lpArgv) {
      mRun(item,lpArgv);
    };

    uuid getKey() { return key; };
    
    int isValid() { return mValid; };
    
    const char *getName() { return mName; };
    
    ~UsrtCapabilityBearer() {
      if (handle) {
        if( item!=NULL ) 
          mDestroy(item);
        dlclose(handle);
      }
    }; 
};
};
#endif //Capability_Bearer_H

