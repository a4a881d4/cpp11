import sys
sys.path.append('work')

from optparse import OptionParser
from usrtos import UsrtTaskAPI as api

import config
class UserAPI(api):
	noArgv = [
		  "ExamplesHelloWorld"
		]
	withInt = [
		"ExamplesArgvInt"
	]
	withKey = [
	]
	withStr = [
		"ExamplesArgvStr"
	]
	system = [
		"CallBackLunchTask"
	]
	def __init__(self,mdir):
		api.__init__(self,mdir)
		self.keys = {}
		for cap in UserAPI.noArgv:
			self.keys[cap] = {'f' : self.byKeyInt, 'k' : self.getKey(cap)}
		for cap in UserAPI.withInt:
			self.keys[cap] = {'f' : self.byKeyInt, 'k' : self.getKey(cap)}
		for cap in UserAPI.withStr:
			self.keys[cap] = {'f' : self.byKeyStr, 'k' : self.getKey(cap)}
		# for cap in UserAPI.withKey:
		# 	self.keys[cap] = {'f' : self.byKeyKey, 'k' : self.getWorkerKey(cap)}
		for cap in UserAPI.system:
			self.keys[cap] = {'f' : None, 'k' : self.getKey(cap)}

	def done(self,cap,argv=0):
		a = self.keys[cap]
		a['f'](a['k'],argv)

	def dumpKeys(self):
		for k in self.keys:
			print(k,":",self.keys[k]['k'])

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

	parse.add_option("-k", "--keys",
	                dest= "k",
	                action= "store_true",
	                default= False,
	                help= "dump keys")

	parse.add_option("-t", "--test",
	                dest= "test",
	                action= "store_true",
	                default= False,
	                help= "test hello world")

	parse.add_option("-i", "--setcap",
	                dest= "cap",
	                action= "store_true",
	                default= False,
	                help= "set cap key")

	(option, arges) = parse.parse_args()

	aAPI = UserAPI(option.dir)

	if option.n != None:
		aAPI.done(arges[0],int(option.n))
	elif option.str != None:
		aAPI.done(arges[0],option.str)
	else:
		aAPI.done(arges[0],0)
	if option.k:
		aAPI.dumpKeys()

	if option.test:
		k = aAPI.keys["ExamplesHelloWorld"]['k']
		agpTask = aAPI.newTask(k,8)
		bgpTask = aAPI.newTask(aAPI.keys["ExamplesArgvInt"]['k'],8)
		aAPI.setCallBack(agpTask
			, aAPI.keys["CallBackLunchTask"]['k']
			, 2
			, 10
			, 100000
			, agpTask
			)
		aAPI.emitTask(agpTask)

	if option.cap:
		aCfg = config.Config(option.dir)
		for cap in aAPI.keys:
			print(aAPI.keys[cap]['k'])
			aCfg.done("SetCapByKey",aAPI.keys[cap]['k'])

if __name__ == '__main__':
	main()