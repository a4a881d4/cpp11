#include <stdexcept>

namespace usrtos {

class usrtos_exception : public std::exception
{
   public:
   usrtos_exception(const char *err)
      :  m_err(other_error)
   {
      try   {  m_str = err; }
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