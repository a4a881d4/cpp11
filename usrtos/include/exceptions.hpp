#pragma once

#include <stdexcept>
#include <errno.h>        //Errors
#include <cstring>        //strerror
#include <iostream>
#include <sstream>
#include <execinfo.h>

namespace usrtos {

inline void fill_system_message( int system_error, std::string &str)
{  str = std::strerror(system_error);  }


enum error_code_t
{
   no_error = 0,
   system_error,     // system generated error; if possible, is translated
                     // to one of the more specific errors below.
   other_error,      // library generated error
   security_error,   // includes access rights, permissions failures
   read_only_error,
   io_error,
   path_error,
   not_found_error,
//   not_directory_error,
   busy_error,       // implies trying again might succeed
   already_exists_error,
   not_empty_error,
   is_directory_error,
   out_of_space_error,
   out_of_memory_error,
   out_of_resource_error,
   lock_error,
   sem_error,
   mode_error,
   size_error,
   corrupted_error,
   not_such_file_or_directory,
   invalid_argument,
   timeout_when_locking_error,
   timeout_when_waiting_error,
   owner_dead_error
};

typedef int    native_error_t;
struct ec_xlate
{
   native_error_t sys_ec;
   error_code_t   ec;
};

static const ec_xlate ec_table[] =
{
   { EACCES, security_error },
   { EROFS, read_only_error },
   { EIO, io_error },
   { ENAMETOOLONG, path_error },
   { ENOENT, not_found_error },
   //    { ENOTDIR, not_directory_error },
   { EAGAIN, busy_error },
   { EBUSY, busy_error },
   { ETXTBSY, busy_error },
   { EEXIST, already_exists_error },
   { ENOTEMPTY, not_empty_error },
   { EISDIR, is_directory_error },
   { ENOSPC, out_of_space_error },
   { ENOMEM, out_of_memory_error },
   { EMFILE, out_of_resource_error },
   { ENOENT, not_such_file_or_directory },
   { EINVAL, invalid_argument }
};
inline error_code_t lookup_error(native_error_t err)
{
   const ec_xlate *cur  = &ec_table[0],
                  *end  = cur + sizeof(ec_table)/sizeof(ec_xlate);
   for  (;cur != end; ++cur ){
      if ( err == cur->sys_ec ) return cur->ec;
   }
   return system_error; // general system error code
}

struct error_info
{
   error_info(error_code_t ec = other_error )
      :  m_nat(0), m_ec(ec)
   {}

   error_info(native_error_t sys_err_code)
      :  m_nat(sys_err_code), m_ec(lookup_error(sys_err_code))
   {}

   error_info & operator =(error_code_t ec)
   {
      m_nat = 0;
      m_ec = ec;
      return *this;
   }

   error_info & operator =(native_error_t sys_err_code)
   {
      m_nat = sys_err_code;
      m_ec = lookup_error(sys_err_code);
      return *this;
   }

   native_error_t get_native_error()const
   {  return m_nat;  }

   error_code_t   get_error_code()const
   {  return m_ec;  }

   private:
   native_error_t m_nat;
   error_code_t   m_ec;
};

class usrtos_exception : public std::exception
{
   public:
   usrtos_exception(const char *err)
      :  m_err(other_error)
   {
      try   {  m_str = err; }
      catch (...) {}
   }

   usrtos_exception(const std::stringstream& err)
      :  m_err(other_error)
   {
      try   {  m_str = err.str(); }
      catch (...) {}
   }

   usrtos_exception(std::stringstream& err, size_t s)
      :  m_err(other_error)
   {
      try {
         void* array[s] = {0};
         char **strframe = NULL;
         auto size = backtrace(array, s);
         strframe = (char **)backtrace_symbols(array, size);
         for(int i = 0; i < size; i++){
            err << "frame " << i << ": " << strframe[i] << std::endl;
         }
         m_str = err.str(); 
      }
      catch (...) {}
   }

  usrtos_exception(const error_info &err_info, const char *str = 0)
      :  m_err(err_info)
   {
      try{
         if(m_err.get_native_error() != 0){
            fill_system_message(m_err.get_native_error(), m_str);
         }
         else if(str){
            m_str = str;
         }
         else{
            m_str = "usrtos_exception::library_error";
         }
      }
      catch(...){}
   }

   virtual ~usrtos_exception() throw(){}

   virtual const char * what() const throw()
   {  return m_str.c_str();  }

   native_error_t get_native_error()const { return m_err.get_native_error(); }

   error_code_t   get_error_code()  const { return m_err.get_error_code(); }

   private:
   error_info        m_err;
   std::string       m_str;
};
}; // namespace usrtos