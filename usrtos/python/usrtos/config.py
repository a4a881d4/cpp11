import sys
sys.path.append('work')

from usrtos import UsrtConfig as cfg
from optparse import OptionParser

class Config(cfg):
	noArgv = [
		  "HelloWorld"
		, "DumpQueue"
		, "DumpThread"
		, "ListCap"
		, "UnlockReady"
		, "UnlockWait"
		, "SetDefaultKeeper"
		, "Exit"
		]
	withInt = [
		"Start"
	]
	withKey = [
		  "DelCapByKey"
		, "SetCapByKey"
		, "SetKeeper"
	]
	withStr = [
		"SetCap"
	]
	system = [
		  "capWorkersKeeper"
		, "capCallBackLunchTask"
		, "capExamplesHelloWorld"
	]
	def __init__(self,mdir):
		cfg.__init__(self,mdir)
		self.keys = {}
		for cap in Config.noArgv:
			self.keys[cap] = {'f' : self.byKey, 'k' : self.getWorkerKey(cap)}
		for cap in Config.withInt:
			self.keys[cap] = {'f' : self.byKeyInt, 'k' : self.getWorkerKey(cap)}
		for cap in Config.withStr:
			self.keys[cap] = {'f' : self.byKeyStr, 'k' : self.getWorkerKey(cap)}
		for cap in Config.withKey:
			self.keys[cap] = {'f' : self.byKeyKey, 'k' : self.getWorkerKey(cap)}
		for cap in Config.system:
			self.keys[cap] = {'f' : None, 'k' : self.getKey(cap)}

	def done(self,cap,argv=None):
		a = self.keys[cap]
		if argv == None:
			a['f'](a['k'])
		else:
			a['f'](a['k'],argv)

	def dumpKeys(self):
		for k in self.keys:
			print(k,":",self.keys[k]['k'])


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
                help= "memory block directory")

(option, arges) = parse.parse_args()

aCfg = Config(option.dir)

if option.n != None:
	aCfg.done(arges[0],int(option.n))
elif option.str != None:
	aCfg.done(arges[0],option.str)
else:
	aCfg.done(arges[0])

if option.k:
	aCfg.dumpKeys()

if option.ucap:
	print(option.ucap,":",aCfg.getKey(option.ucap))
	