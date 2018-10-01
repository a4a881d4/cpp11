from script import Script
from time import time,sleep
from usrtos import AnyType,UUID,uClock
import sys
import os

def clear(dir,start,end):
	aS = Script(dir)
	aS.s.nTask()
	aS.s.allocm(1,0,AnyType(32))
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
	aS.s.callsy(0,scriptCap)
	aS.s.ret()
	aS.s.test()
	aS.s.push()

	#		self.newTask(0xf,0,scriptCap,now)

if __name__ == '__main__':
	if len(sys.argv) > 2:
		clear("/tmp/usrtos", int(sys.argv[1]), int(sys.argv[2]))
	elif len(sys.argv) > 1:
		clear("/tmp/usrtos", int(sys.argv[1]), 1)
	else:
		clear("/tmp/usrtos", -1, 1)