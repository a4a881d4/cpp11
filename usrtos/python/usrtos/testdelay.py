from script import Script
from time import time,sleep
from usrtos import AnyType,UUID,uClock

def once(aScript):
	aScript.reset()
	scriptCap = UUID(aScript.api.keys["ExamplesClock"]['k'])
	now = int(time())*int(1e9)+3000000000
	for i in range(100):
		aScript.newTask(0xf,0,scriptCap,now)
		now += 10000000
	aScript.s.ret()
	aScript.s.push()

def TestDelay(dir):
	aScript = Script(dir)
	aScript.s.allocm(1,0,AnyType(65536*8))
	aScript.s.clearm(0)
	aScript.s.ret()
	aScript.s.push()
	for i in range(2000):
		once(aScript)
		sleep(1)
	sleep(1)
	aScript.reset()
	scriptCap = UUID(aScript.api.keys["ExamplesAnClock"]['k'])
	now = int(time())*int(1e9)+3000000000
	aScript.newTask(0xf,0,scriptCap,now)
	aScript.s.ret()
	aScript.s.push()

if __name__ == '__main__':
	TestDelay("/tmp/usrtos")