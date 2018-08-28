#include <regex>
#include <string>
#include <iostream>
using namespace std;

#define BOOST_FILESYSTEM_NO_DEPRECATED

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/progress.hpp>
#include <iostream>

namespace fs = boost::filesystem;

int main( int argc, char* argv[] )
{
  std::regex e("[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}$");
  boost::progress_timer t( std::clog );

  fs::path full_path( fs::initial_path<fs::path>() );

  if ( argc > 1 )
    full_path = fs::system_complete( fs::path( argv[1] ) );
  else
    std::cout << "\nusage:   finduuid [path]" << std::endl;

  unsigned long file_count = 0;
  unsigned long dir_count = 0;
  unsigned long other_count = 0;
  unsigned long err_count = 0;

  if ( !fs::exists( full_path ) )
  {
    std::cout << "\nNot found: " << full_path.string() << std::endl;
    return 1;
  }

  if ( fs::is_directory( full_path ) )
  {
    std::cout << "\nIn directory: "
              << full_path.string() << "\n\n";
    fs::directory_iterator end_iter;
    for ( fs::directory_iterator dir_itr( full_path );
          dir_itr != end_iter;
          ++dir_itr )
    {
      try
      {
        if ( fs::is_regular_file( dir_itr->status() ) )
        {
          ++file_count;
          std::cmatch what;
          auto fn = dir_itr->path().filename();
          if(std::regex_match(fn.c_str(),what,e))
            std::cout <<  fn << "\n";
        }
      }
      catch ( const std::exception & ex )
      {
        ++err_count;
        std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
      }
    }
  }
  else // must be a file
  {
    std::cout << "\nFound: " << full_path.string() << "\n";    
  }
  return 0;
}

/*

int main(int argc, char *argv[]) 
{
	string auuid = "/tmp/usrtos/40e9ff6f-ed33-5be9-83d4-e06ed2d18334";
	cmatch what;
	if(regex_match(auuid.c_str(),what,e)) {
		for(int i=0;i<what.size();i++) {
			cout << "No " << i << ": " << what[i] << endl;
		}
	}
}
*/