#include <USRTCapabilityBearer.h>

USRTCapabilityBearer::USRTCapabilityBearer( const char* lib )
{
  mValid=0;
    if( getHandle(lib)==0 ) {
        dlerror();        /* Clear any existing error */
        mValid=1;
        init();
    }
}

int USRTCapabilityBearer::getHandle(const char* lib) 
{
    const char *path = getFileName(lib);
    handle = dlopen ( path, RTLD_LAZY);
    if ( handle==NULL ) {
        fprintf (stderr, "load %s(%s) error : ", lib, path);
        fprintf (stderr, "%s\n", dlerror());
        return -1;
    }
    else
        return 0;
}
    
void *USRTCapabilityBearer::getFunc(const char *sym)
{
    void *func = dlsym(handle, sym);
    const char *error;
    if ((error = dlerror()) != NULL)    {
        fprintf (stderr, "%s\n", error);
        mValid=0;
        return NULL;
    }
    return func;
}

void USRTCapabilityBearer::init()
{
    mFactroy=(factroyFunc)getFunc("factroy");
    mRun=(runFunc)getFunc("run");
    mGetKey=(getKeyFunc)getFunc("getKey");
    mDestroy=(destroyFunc)getFunc("destroy");
    if( mValid ) 
        item=mFactroy();
    else
        item=NULL;
    if( mValid )
        key=mGetKey(item);
    else
        key=0LL;
}

void USRTCapabilityBearer::runGP(generalized_memory_t* gpArgv)
{
    void *lpArgv = G2L( gpArgv );
    mRun(item,lpArgv);
}

void USRTCapabilityBearer::runLP(void* lpArgv)
{
    mRun(item,lpArgv);
}

USRTCapabilityBearer::~USRTCapabilityBearer()
{
    if (handle) {
        if( item!=NULL ) 
            mDestroy(item);
        dlclose(handle);
    }
} 

const char *USRTCapabilityBearer::getWorkingDir (void) 
{
        char *workdir;

        if ((workdir = getenv ("USRTWORK")) != NULL)     return workdir;

        return "work";
}

const char *USRTCapabilityBearer::getFileName( const char *n)
{
    strcpy(mName,getWorkingDir());
    strcat(mName,"/");
    strcat(mName,n);
    return mName;
}

USRTCapabilityBearer::USRTCapabilityBearer( int64 k )
{
    mValid=0;
    const char *workdir = getWorkingDir();
    struct dirent* entry;
    DIR* dir = opendir(workdir);
    while( (entry=readdir(dir))!=NULL ) {
        dlerror();
        handle = dlopen (getFileName(entry->d_name), RTLD_LAZY);
        if( !handle ) {
            fprintf(stderr," check lib %s fail\n",entry->d_name);
            fprintf (stderr, "%s\n", dlerror());
            continue;
        }
        mValid=1;
        init();
        if( key==k )
            break;
        mValid=0;
        dlclose(handle);
    }
}


