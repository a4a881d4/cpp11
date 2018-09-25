import sys
sys.path.append('work')

from usrtos import AnyType,UUID
import script
s = script.Script("/tmp/usrtos")
key = s.api.getKey("ExamplesClock")
print("Key:",key)
s.doCap("SetCapByKey",key)
s.push()

s.allocm(1,0,AnyType(32))
scriptCap = UUID(key)
s.callcp(0,scriptCap)
s.ret()
s.push()
