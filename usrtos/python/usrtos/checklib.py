import sys
sys.path.append('work')

import usrtos

# a = usrtos.UsrtMem(sys.argv[1])

# a.dump()

# a.dumpHead();

# aGp = a.newGP()

# print(aGp.offset,aGp.objsize)

# a.dump()

# bp = a.GPandLP(aGp)

# print(aGp.offset,aGp.objsize)

# a.dump()

# os = usrtos.UsrtScript(sys.argv[1],256)
# os.moveal(0,1)
# os.selfrd(0)
# os.selfro(0,usrtos.AnyType(1000000))
# tid = usrtos.UUID("8ea09e05-fd67-5949-a9ab-e722a3dae01c")
# os.immeid(0,tid)
# id = usrtos.AnyType(0)
# id.setUUID(tid)
# os.immevl(0,id)
# os.allocm(1,1,usrtos.AnyType(128))
# os.savevl(0,1,usrtos.AnyType(0))
# id.setString("capWorkersSetDefaultKeeper")
# os.immevl(2,id)
# os.savevl(2,1,usrtos.AnyType(16))

# os.ret()

# os.test()

import time

print(time.time())

clock = usrtos.uClock()

clock.peek()
print(clock.sys)
print(clock.cpu)
print(clock.system_clock)

for i in range(10):
	clock.cupdate()
	time.sleep(0.2)
clock.peek()
print(clock.sys)
print(clock.cpu)
print(clock.system_clock)
t = time.time()*1e9
print(t-clock.system_clock)
clock.peek()
print(t-clock.system_clock)


