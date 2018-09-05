#ifdef FUNCLASS
#include <captype.hpp>
#include <capability.hpp>
#include <version.hpp>

#define _STR(x) #x
#define STR(x) _STR(x)

class FUNCLASS : public CCapability {
private:
	uuid key;
public:
	const struct CapabilityMeta meta = {
			  .name = STR(FUNCLASS)
			, .type = "USRTOS"
			, .version = sha1str(USRTOSVERSION)
		};
	int run( void *argv );
	uuid getKey() {
		return key;
	};
	FUNCLASS() {
		key = meta2uuid(meta);
	};
	
	int destroy( ) {
		delete this;
		return 1;
	}
};

extern "C" {
	CCapability *factroy() {
		FUNCLASS *item = new FUNCLASS();
		return (CCapability*)item;
	}
	int run( CCapability* item, void *argv ) {
		return ((FUNCLASS *)item)->run( argv );
	}
	uuid getKey( CCapability* item){
		return ((FUNCLASS *)item)->getKey();
	}
	void destroy( CCapability* item ) {
		((FUNCLASS *)item)->destroy();
	}
};	



#else
#error "Must define FUNCLASS Macro"
#endif
