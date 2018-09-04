#ifdef FUNCLASS
#include <captype.h>
#include <capability.h>

class FUNCLASS : CCapability {
	private:
		static uuid key;
	public:
		uuid getKey();
		int run( void *argv );
		int destroy();
};

extern "C" {
	CCapability *factroy() {
		FUNCLASS *item = new FUNCLASS();
		return (CCapability*)item;
	}
	int run( CCapability* item, void *argv ) {
		return (FUNCLASS *)item->run( argv );
	}
	uuid getKey( CCapability* item){
		return ((FUNCLASS *)item)->getKey();
	}
	void destroy( CCapability* item ) {
		((FUNCLASS *)item)->destroy();
	}
};	
#define _STR(x) #x
#define STR(x) _STR(x)

long long int FUNCLASS::key = md5first(STR(FUNCLASS));
uuid FUNCLASS::getKey() {
	return key;
}
int FUNCLASS::destroy( ) {
	delete this;
	return 1;
}

#else
#error "Must define FUNCLASS Macro"
#endif
