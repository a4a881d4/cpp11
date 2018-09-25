import sys
sys.path.append('work')

from usrtos import uClock
import time

# import script


# s = script.Script("/tmp/usrtos")
# key = s.api.getKey("ExamplesClock")
# print("Key:",key)
# s.doCap("SetCapByKey",key)
# s.push()

# s.allocm(1,0,AnyType(32))
# scriptCap = UUID(key)
# s.callcp(0,scriptCap)
# s.ret()
# s.push()

class UsrtClock(uClock):
	def reset(self):
		self.peek()
		self.S0 = self.sys
		self.C0 = self.cpu

		self.S = 0
		self.C = 0

		self.v = .0

		self.ds2 = 1.

	def look(self):
		self.peek()
		return (self.sys - self.S0,self.cpu - self.C0)

	def update(self):
		(nS,nC) = self.look()
		print(nS,nC)
		if int(self.v) == 0:
			self.v = float(nC-self.C)/float(nS-self.S)
			self.S,self.C = nS,nC
		else: 
			dS = nS - self.S
			estC = dS*self.v + self.C
			eC = nC - estC
			print("error: ",eC)
			# self.ds2 += float(dS*dS)
			self.v += 0.25*eC/float(dS)
			self.C = estC
			self.S = nS
		print("v:",self.v,"EC: ",self.C,"C:",nC)



c = UsrtClock()
c.reset()
print(c.look())

while 1:
	time.sleep(10)
	c.update()
	