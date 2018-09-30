import sys
sys.path.append('work')

from config import Config as cfg
from usrtos import UsrtScript as script
from usrtos import AnyType,UUID,uClock
from userAPI import UserAPI as uAPI

from optparse import OptionParser
from utility import getStructFieldOffest
from ctypes import *
from time import time

class task(Structure):
	_fields_ = [  ("argv",  c_char*32)
				, ("ID",    c_longlong*2)
				, ("noE",   c_longlong)
				, ("noL",   c_char*4)
				, ("valid", c_char*4)
				, ("key",   c_char*16)
				, ("cbkey", c_char*16)
				, ("cbarg", c_char*56)
				]

class Task:
	fields = getStructFieldOffest(task)
	def __init__(self):
		pass

class Script:
	def __init__(self,mdir,ssize = 65536*4):
		self.s = script(mdir,ssize)
		self.cfg = cfg(mdir)
		self.api = uAPI(mdir)
		self.clock = uClock()

	def reset(self):
		self.s.reset()
	
	def nScript():
		self.s.nScript()

	def newTask(self,reg_task,reg_argv,key,now):
		self.s.allocm(0,reg_task,AnyType(sizeof(task)))
		self.s.clearm(reg_task)
		aStr = AnyType(0)
		aStr.setUUID(key)
		self.s.immevl(0xf0,aStr)
		self.s.savevl(0xf0,reg_task,AnyType(Task.fields["key"]))
		self.s.immevl(0xf0,AnyType(10))
		self.s.savevl(0xf0,reg_task,AnyType(Task.fields["ID"]))
		self.s.immevl(0xf0,AnyType(now-100))
		self.s.selfsc(0xf0)
		self.s.immevl(0xf1,AnyType(100))
		self.s.selfsm(0xf1)
		self.s.immetp(0xf1,6)
		self.s.savevl(0xf0,reg_task,AnyType(Task.fields["noE"]))
		self.s.savevl(0xf1,reg_task,AnyType(Task.fields["noL"]))
		self.s.savegp(reg_argv,reg_task,AnyType(Task.fields["argv"]))
		self.s.pushof(reg_task)

	def doCap(self,cn,arg=0):
		if cn in cfg.noArgv:
			self.s.allocm(1,0,AnyType(32))
			self.s.clearm(0)
		elif cn in cfg.withInt:
			self.s.allocm(1,0,AnyType(32))
			self.s.clearm(0)
			self.s.immevl(1,AnyType(int(arg)))
			self.s.savevl(1,0,AnyType(0))
		elif cn in cfg.withStr:
			self.s.allocm(1,0,AnyType(32))
			self.s.clearm(0)
			aStr = AnyType(0)
			aStr.setString(arg)
			self.s.immevl(1,aStr)
			self.s.savevl(1,0,AnyType(0))
		elif cn in cfg.withKey:
			self.s.allocm(1,0,AnyType(32))
			self.s.clearm(0)
			aStr = AnyType(0)
			aStr.setUUID(UUID(arg))
			self.s.immevl(1,aStr)
			self.s.savevl(1,0,AnyType(0))
		else:
			pass
		if cn in self.cfg.keys:
			scriptCap = UUID(self.cfg.keys[cn]['k'])
			self.s.callsy(0,scriptCap)
		
		if cn in uAPI.noArgv:
			self.s.allocm(1,0,AnyType(32))
			self.s.clearm(0)
		elif cn in uAPI.withInt:
			self.s.allocm(1,0,AnyType(32))
			self.s.clearm(0)
			self.s.immevl(1,AnyType(int(arg)))
			self.s.savevl(1,0,AnyType(0))
		elif cn in uAPI.withStr:
			self.s.allocm(1,0,AnyType(32))
			self.s.clearm(0)
			aStr = AnyType(0)
			aStr.setString(arg)
			self.s.immevl(1,aStr)
			self.s.savevl(1,0,AnyType(0))
		else:
			pass

		if cn in self.api.keys:
			scriptCap = UUID(self.api.keys[cn]['k'])
			now = int(time())*int(1e9)+3000000000
			self.newTask(0xf,0,scriptCap,now)
			
	def initKey(self):
		for cap in self.api.keys:
			print(cap,":",self.api.keys[cap]['k'])
			self.doCap("SetCapByKey",self.api.keys[cap]['k'])

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

	parse.add_option("-i", "--init",
	                dest= "init",
	                action= "store_true",
	                default= False,
	                help= "init workers")
	
	parse.add_option("-c", "--script",
	                dest= "script",
	                action= "store_true",
	                default= False,
	                help= "run script mode")
	(option, arges) = parse.parse_args()

	aScript = Script(option.dir)
	if option.script:
		aScript.s.nScript()
	else:
		aScript.s.nTask()
	if option.init:
		print("Initial workers by script")
		aScript.initKey()
		aScript.doCap("SetDefaultKeeper")
		aScript.doCap("SetWorker",aScript.api.keys["WorkersTaskScript"]['k'])
		aScript.doCap("Start",1)
		aScript.doCap("UpdateWorkerKey",0)
	else:
		if option.n != None:
			aScript.doCap(arges[0],int(option.n))
		elif option.str != None:
			aScript.doCap(arges[0],option.str)
		else:
			aScript.doCap(arges[0])

	aScript.s.ret()
	aScript.s.test()
	aScript.s.push()


if __name__ == '__main__':
	main()

		