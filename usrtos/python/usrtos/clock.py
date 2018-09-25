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
	def init(self):
		self.peek()
		self.S0 = self.sys
		self.C0 = self.cpu

		self.S = 0
		self.C = 0

		self.v = .0

		self.ds2 = 1.
		self.needReset = True

	def reset(self):
		self.peek()
		self.S0 = self.sys
		self.C0 = self.cpu
		self.S = 0
		self.C = 0

	def look(self):
		self.peek()
		return (self.sys - self.S0,self.cpu - self.C0)

	def update(self):
		(nS,nC) = self.look()
		print(nS,nC)
		if self.needReset:
			self.v = float(nC-self.C)/float(nS-self.S)
			self.S,self.C = nS,nC
			self.needReset = False
		else: 
			dS = nS - self.S
			estC = dS*self.v + self.C
			eC = nC - estC
			print("error: ",eC)
			self.v += 0.5*eC/float(dS)
			e = abs(eC)/self.v
			if e > 1000:
				self.reset()
				print("reset")
				if e > 10000:
					print("needReset")
					self.needReset = True
			else:
				self.C = estC
				self.S = nS
		print("v:",self.v,"EC: ",self.C,"C:",nC)



c = UsrtClock()
c.init()
print(c.look())

while 1:
	time.sleep(0.5)
	c.update()
	