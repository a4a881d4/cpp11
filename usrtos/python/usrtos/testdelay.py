from script import Script
from time import time,sleep
from usrtos import AnyType,UUID,uClock

def once(dir):
	aScript = Script(dir)
	scriptCap = UUID(aScript.api.keys["ExamplesClock"]['k'])
	now = int(time())*int(1e9)+3000000000
	for i in range(100):
		aScript.newTask(0xf,0,scriptCap,now)
		now += 10000000
	aScript.ret()
	aScript.push()

def TestDelay(dir):
	aScript = Script(dir)
	aScript.allocm(1,0,AnyType(65536*8))
	aScript.clearm(0)
	aScript.ret()
	aScript.push()
	for i in range(20):
		once(dir)
		sleep(1)
	sleep(1)
	aScript = Script(dir)
	scriptCap = UUID(aScript.api.keys["ExamplesAnClock"]['k'])
	now = int(time())*int(1e9)+3000000000
	aScript.newTask(0xf,0,scriptCap,now)
	aScript.ret()
	aScript.push()

if __name__ == '__main__':
	TestDelay("/tmp/usrtos")