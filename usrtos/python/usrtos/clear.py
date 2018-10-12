from script import Script
from time import time,sleep
from usrtos import AnyType,UUID,uClock
import sys
import os
from optparse import OptionParser

def clear(dir,start,end,mode):
	aS = Script(dir)
	if mode=="system":
		aS.s.nTask()
	else:
		aS.s.nScript()
	aS.s.allocm(1,0,AnyType(16))
	aS.s.clearm(0)
	now = int(time()*1e9)
	iS = now + int(start*1e9)
	iE = now + int(end*1e9)
	aS.s.immevl(1,AnyType(iS))
	aS.s.selfsc(1)
	aS.s.savevl(1,0,AnyType(0))
	aS.s.immevl(1,AnyType(iE))
	aS.s.selfsc(1)
	aS.s.savevl(1,0,AnyType(8))
	scriptCap = UUID(aS.cfg.keys["capWorkersClearer"]['k'])
	if mode=="system":
		aS.s.callsy(0,scriptCap)
	else:
		aS.s.selfst(0,1)
	
	aS.s.ret()
	aS.s.test()
	aS.s.push()

if __name__ == '__main__':
	parse = OptionParser()

	parse.add_option("-s", "--start",
	                dest= "s",
	                type= "float",
	                default=-1,
	                help= "clear start")

	parse.add_option("-e", "--end",
	                dest= "e",
	                type= "float",
	                default=1,
	                help= "string agrv")

	parse.add_option("-d", "--dir",
	                dest= "dir",
	                default= "/tmp/usrtos",
	                help= "memory block directory")

	parse.add_option("-c", "--script",
	                dest= "script",
	                action= "store_true",
	                default= False,
	                help= "run script mode")

	(option, arges) = parse.parse_args()

	if option.script:
		clear(option.dir, option.s, option.e, "script")
	else:
		clear(option.dir, option.s, option.e, "system")
	