#include <iostream>
#include <vector>
#include <assert.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/sha1.hpp>
using namespace boost;
using namespace boost::uuids;
using namespace boost::uuids::detail;
using namespace std;

int main()
{
	uuid u = lexical_cast<uuid>("01234567-89ab-cdef-0123-456789abcdef");
	cout<<u<<endl;
	string str = lexical_cast<string>(u);  //uuid转换到字符串
	cout<<str<<endl;

	// uuid base = string_generator()("{0123456789abcdef0123456789abcdef}");
	name_generator ngen(u);//构造名字生成器
	uuid u1 = ngen("rfbuffer");//为名字mario生成UUID

	str = lexical_cast<string>(u1);  //uuid转换到字符串
	cout<<str<<endl;

	sha1 sha;
	char *szMsg = "a short message";   //用于摘要的消息
	sha.process_bytes(szMsg, strlen(szMsg)); //处理多个字节
	unsigned int digest[5];      //摘要的返回值
	sha.get_digest(digest);
 	for (int i = 0; i< 5; ++i)
		cout<< hex << digest[i];    //16进制输出
	cout<<endl;

}