#include <dag/nodes.hpp>
#include <iostream>
#include <vector>
using namespace usrtos;
using namespace usrtos::dag;

Func *factoryFunc(const char *n) {
	return new Func(n);
}
int main() {
	Var   TransBB("TransBB")
		, TTSym("TTSym")
		, TFSym("TFSym")
		, TPilot("TPilot")
		, PSS("PSS")
		, TTFCI("TTFCI")
		, Interleaved("Interleaved")
		, Coded("Coded")
		, CRCed("CRCed")
		, Input("Input")
		;

	Var   RecvBB("RecvBB")
		, RTSym("RTSym")
		, RFSym("RFSym")
		, RPilot("RPilot")
		, Channel("Channel")
		, Demoded("Demoded")
		, RTFCI("RTFCI")
		, Deinterleaved("Deinterleaved")
		, Decoded("Decoded")
		, Output("Output")
		;
	
	factoryFunc("addCP")
			-> In(TTSym,"in")
			-> Out(TransBB,"out")
			;
	factoryFunc("IFFT")
			-> In(TFSym,"in")
			-> Out(TTSym,"out")
			;
	factoryFunc("Mux")
			-> Out(TFSym,"out")
			-> In(TPilot,"in")
			-> In(PSS,"in")
			-> In(TTFCI,"in")
			-> In(Interleaved,"in")
			;
	factoryFunc("Interleave")
			-> Out(Interleaved,"out")
			-> In(Coded,"in")
			;
	factoryFunc("TurboEncoder")
			-> Out(Coded,"out")
			-> In(CRCed,"in")
			;
	factoryFunc("addCRC")
			-> Out(CRCed,"out")
			-> In(Input,"in")
			;
	factoryFunc("TPilotGen")
			-> Out(TPilot,"out")
			;
	factoryFunc("PSSGen")
			-> Out(PSS,"out")
			;
	factoryFunc("TFCIGen")
			-> Out(TTFCI,"out")
			;
	
	factoryFunc("removeCP")
			-> Out(RTSym,"out")
			-> In(RecvBB,"in")
			;
	factoryFunc("FFT")
			-> Out(RFSym,"out")
			-> In(RTSym,"in")
			;
	factoryFunc("RPilotGen")
			-> Out(RPilot,"out")
			;
	factoryFunc("ChannelEst")
			-> Out(Channel,"out")
			-> In(RPilot,"in")
			-> In(RFSym,"in")
			;
	factoryFunc("Demodulate")
			-> Out(Demoded,"out")
			-> In(RFSym,"in")
			-> In(Channel,"in")
			;
	factoryFunc("TFCIDecode")
			-> Out(RTFCI,"out")
			-> In(Demoded,"in")
			;
	factoryFunc("TurboDecoder")
			-> Out(Decoded,"out")
			-> In(Deinterleaved,"in")
			;
	factoryFunc("Deinterleave")
			-> Out(Deinterleaved,"out")
			-> In(Demoded,"in")
			;
	factoryFunc("RemoveCRC")
			-> Out(Output,"out")
			-> In(Decoded,"in")
			;
	
	auto f = NodeBase::Funcs();
	for(auto it = f.begin();it!=f.end();++it)
		std::cout << "//" << it->first << std::endl;
	auto dot = NodeBase::Dot();
	std::cout << dot << std::endl;
}