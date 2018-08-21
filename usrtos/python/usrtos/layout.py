from ctypes import *
from version import VERSION
import hashlib
from uuid import UUID,uuid5

class Head(Structure):
	_fields_ = [ ('name',     c_char*32)   #000
				,('type',     c_char*32)   #032
				,('metaSize', c_longlong)  #064
				,('dataSize', c_longlong)  #072
				,('cpSize',   c_longlong)  #080
				,('version',  c_char*40)   #088
				,('sha1',     c_char*40)   #128
				,('rdlock',   c_int)       #168
				,('wrlock',   c_int)       #172
				]                          #176

class block:
	align = 4096
	namespace = UUID('8ea09e05-fd67-5949-a9ab-e722a3dae01c')
	def __init__(self,name):
		if len(name)>32:
			self.name = name[:32]
		else:
			self.name = name
		self.type = "block"
		self.metaSize = block.align
	
	def genHead(self):
		self.head = Head()
		memset(pointer(self.head),0,sizeof(self.head))
		memmove(self.head.name,self.name,len(self.name))
		memmove(self.head.type,self.type,len(self.type))
		self.head.metaSize = c_longlong(self.metaSize)
		self.head.cpSize = c_longlong(self.cpSize)
		self.head.dataSize = c_longlong(self.dataSize)
		self.VERSION = VERSION
		memmove(self.head.version,self.VERSION,len(self.VERSION))
		m = hashlib.sha1()
		m.update(string_at(addressof(self.head),128))
		self.sha1 = m.hexdigest()
		memmove(self.head.sha1,self.sha1,len(self.sha1))
		self.fileName = str(uuid5(block.namespace,self.sha1))
		
	def setSZ(self,sz,cp=0):
		self.dataSize = self.align2page(sz)
		self.cpSize = self.align2page(cp)
	
	def defaultSZ(self,sz,cp=0):
		self.dataSize = self.align2page(sz)
		self.cpSize = self.align2page(cp)
	
	def align2page(self,l):
		return int((l+block.align-1)/block.align)*block.align

	def dump(self):
		dumps = ['name','type','dataSize','metaSize','cpSize','VERSION','sha1','fileName']
		for item in dumps:
			print(item,": ",self.__dict__[item])

def main():
	a = block("root")
	a.defaultSZ(4096*24,4096*3)
	a.genHead()
	a.dump()

if __name__ == '__main__':
	main()
	