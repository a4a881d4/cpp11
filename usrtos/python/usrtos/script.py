import sys
sys.path.append('work')

from config import Config as cfg
from usrtos import UsrtScript as script
from usrtos import AnyType,UUID

from optparse import OptionParser
class Script(script):
	def __init__(self,mdir):
		script.__init__(self,mdir,4096)
		self.cfg = cfg(mdir)

	def cap(self,cn,arg=0):
		if cn in cfg.noArgv:
			self.allocm(1,0,AnyType(32))
		elif cn in cfg.withInt:
			self.allocm(1,0,AnyType(32))
			self.immevl(1,AnyType(int(arg)))
			self.savevl(1,0,AnyType(0))
		elif cn in cfg.withStr:
			self.allocm(1,0,AnyType(32))
			aStr = AnyType(0)
			aStr.setString(arg)
			self.immevl(1,aStr)
			self.savevl(1,0,AnyType(0))
		else:
			pass
		scriptCap = UUID(self.cfg.keys["capWorkersScript"]['k'])
		self.callsy(0,scriptCap)

def main():
	parse = OptionParser()

	parse.add_option("-n", "--integer",
	                dest= "n",
	                help= "integer agrv")

	parse.add_option("-s", "--string",
	                dest= "str",
	                help= "string agrv")

	parse.add_option("-d", "--dir",
	                dest= "dir",
	                default= "/tmp/usrtos",
	                help= "memory block directory")

	parse.add_option("-u", "--userkey",
	                dest= "ucap",
	                help= "get user capability key")

	parse.add_option("-k", "--keys",
	                dest= "k",
	                action= "store_true",
	                default= False,
	                help= "dump keys")

	(option, arges) = parse.parse_args()

	aScript = Script(option.dir)

	if option.n != None:
		aScript.cap(arges[0],int(option.n))
	elif option.str != None:
		aScript.cap(arges[0],option.str)
	else:
		aScript.cap(arges[0])

	if option.k:
		aScript.cfg.dumpKeys()

	if option.ucap:
		print(option.ucap,":",aScript.cfg.getKey(option.ucap))

	aScript.ret()
	aScript.test()

if __name__ == '__main__':
	main()

		