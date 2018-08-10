#include <iostream>

using namespace std;

struct Acode
{
	static constexpr int code[3][2] = {
		{1,2},
		{10,3},
		{3,4}
	};
};

template <int BS, int C, int R, typename Code>
struct qccode
{
	static const int BlockSize = BS;
	static const int Cols = C;
	static const int Rows = R;
	static constexpr bool at(int c, int r) {
		int cs = c%BlockSize;
		int cc = c/BlockSize;
		int rs = r%BlockSize;
		int rr = r/BlockSize;
		int shift = Code::code[rr][cc];
		return ((cs-shift+BlockSize-rs)%BlockSize)==0;
	};
	void dumpCode() {
		for(int r=0;r<BlockSize*Rows;r++) {
			for(int c=0;c<BlockSize*Cols;c++)
				cout << at(c,r);
			cout << endl;
		}
	};
};


int main()
{
	struct qccode<32,2,3,struct Acode> my;
	my.dumpCode();
}
