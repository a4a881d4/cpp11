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
				, ("ID",    c_longlong)
				, ("noE",   c_longlong)
				, ("noL",   c_longlong)
				, ("valid", c_longlong)
				, ("key",   c_char*16)
				, ("cbkey", c_char*16)
				, ("cbarg", c_char*56)
				]

class Task:
	fields = getStructFieldOffest(task)
	def __init__(self):
		pass

class Script(script):
	def __init__(self,mdir,ssize = 65536*4):
		script.__init__(self,mdir,ssize)
		self.cfg = cfg(mdir)
		self.api = uAPI(mdir)
		self.clock = uClock()

	def newTask(self,reg_task,reg_argv,key,now):
		self.allocm(0,reg_task,AnyType(sizeof(task)))
		self.clearm(reg_task)
		aStr = AnyType(0)
		aStr.setUUID(key)
		self.immevl(0xf0,aStr)
		self.savevl(0xf0,reg_task,AnyType(Task.fields["key"]))
		self.immevl(0xf0,AnyType(10))
		self.savevl(0xf0,reg_task,AnyType(Task.fields["ID"]))
		self.immevl(0xf0,AnyType(now))
		self.selfsc(0xf0)
		self.savevl(0xf0,reg_task,AnyType(Task.fields["noE"]))
		self.savevl(0xf0,reg_task,AnyType(Task.fields["noL"]))
		self.savegp(reg_argv,reg_task,AnyType(Task.fields["argv"]))
		self.pushof(reg_task)

	def doCap(self,cn,arg=0):
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
		elif cn in cfg.withKey:
			self.allocm(1,0,AnyType(32))
			aStr = AnyType(0)
			aStr.setUUID(UUID(arg))
			self.immevl(1,aStr)
			self.savevl(1,0,AnyType(0))
		else:
			pass
		if cn in self.cfg.keys:
			scriptCap = UUID(self.cfg.keys[cn]['k'])
			self.callsy(0,scriptCap)
		
		if cn in uAPI.noArgv:
			self.allocm(1,0,AnyType(32))
		elif cn in uAPI.withInt:
			self.allocm(1,0,AnyType(32))
			self.immevl(1,AnyType(int(arg)))
			self.savevl(1,0,AnyType(0))
		elif cn in uAPI.withStr:
			self.allocm(1,0,AnyType(32))
			aStr = AnyType(0)
			aStr.setString(arg)
			self.immevl(1,aStr)
			self.savevl(1,0,AnyType(0))
		else:
			pass

		if cn in self.api.keys:
			scriptCap = UUID(self.api.keys[cn]['k'])
			now = int(time())*int(1e9)+3000000000
			for i in range(1):
				self.newTask(0xf,0,scriptCap,now)
				now += 1000000
	
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

	(option, arges) = parse.parse_args()

	aScript = Script(option.dir)

	if option.init:
		print("Initial workers by script")
		aScript.initKey()
		aScript.doCap("SetDefaultKeeper")
		aScript.doCap("Start",1)
	else:
		if option.n != None:
			aScript.doCap(arges[0],int(option.n))
		elif option.str != None:
			aScript.doCap(arges[0],option.str)
		else:
			aScript.doCap(arges[0])

	aScript.ret()
	aScript.test()
	aScript.push()


if __name__ == '__main__':
	main()

		