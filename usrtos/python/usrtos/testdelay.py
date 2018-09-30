from script import Script
from time import time,sleep
from usrtos import AnyType,UUID,uClock
import sys
import os

def once(aScript):
	aScript.s.nScript()
	aScript.s.allocm(4,0,AnyType(0)) 
	aScript.s.immeos(0,AnyType(0))
	aScript.s.immesz(0,AnyType(65536*8))
	scriptCap = UUID(aScript.api.keys["ExamplesClock"]['k'])
	now = int(time())*int(1e9)+2000000000
	for i in range(1000):
		aScript.newTask(0xf,0,scriptCap,now)
		now += 1000000
	aScript.s.ret()
	aScript.s.push()

def TestDelay(dir,mins):
	aScript = Script(dir)
	aScript.s.nScript()
	aScript.s.allocm(4,0,AnyType(0)) 
	aScript.s.immeos(0,AnyType(0))
	aScript.s.immesz(0,AnyType(65536*8))
	aScript.s.clearm(0)
	aScript.s.ret()
	aScript.s.push()
	for i in range(mins*60):
		once(aScript)
		sleep(1)
	sleep(1)
	aScript.s.nScript()
	aScript.s.allocm(4,0,AnyType(0)) 
	aScript.s.immeos(0,AnyType(0))
	aScript.s.immesz(0,AnyType(65536*8))
	scriptCap = UUID(aScript.api.keys["ExamplesAnClock"]['k'])
	now = int(time())*int(1e9)+3000000000
	aScript.newTask(0xf,0,scriptCap,now)
	aScript.s.ret()
	aScript.s.push()

if __name__ == '__main__':
	try:
		os.remove("/tmp/usrtos/test.result")
	except Exception as e:
		print(e)

	if len(sys.argv) > 1:
		TestDelay("/tmp/usrtos", int(sys.argv[1]))
	else:
		TestDelay("/tmp/usrtos", 1)