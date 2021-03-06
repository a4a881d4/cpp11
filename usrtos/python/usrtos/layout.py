from ctypes import *
from version import VERSION
import hashlib
from uuid import UUID,uuid5
import os
import json
import sys

class Head(Structure):
	_fields_ = [ ('name',     c_char*32)   #000
				,('type',     c_char*32)   #032
				,('metaSize', c_longlong)  #064
				,('dataSize', c_longlong)  #072
				,('cpSize',   c_longlong)  #080
				,('longitude',c_double)    #088
				,('latitude', c_double)    #096
				,('altitude', c_double)    #104
				,('version',  c_char*40)   #112
				,('sha1',     c_char*40)   #152
				]                          #192

class block:
	align = 4096
	namespace = UUID('8ea09e05-fd67-5949-a9ab-e722a3dae01c')
	dumps = [ 'name','type'
			, 'dataSize','metaSize'
			, 'cpSize','VERSION'
			, 'sha1','fileName'
			, 'longitude', 'latitude', 'altitude'
			]
	def __init__(self,name):
		n = name.encode(encoding="utf-8")
		if len(n)>32:
			self.name = n[:32]
		else:
			self.name = n
		self.type = b"block"
		self.metaSize = block.align
		self.defaultSZ(0)
		self.setPosition(0.,0.,0.)

	
	def genHead(self):
		self.head = Head()
		memset(byref(self.head),0,sizeof(self.head))
		self.head.name = self.name
		self.head.type = self.type
		self.head.metaSize = c_longlong(self.metaSize)
		self.head.cpSize = c_longlong(self.cpSize)
		self.head.dataSize = c_longlong(self.dataSize)
		self.head.longitude = c_double(self.longitude)
		self.head.latitude = c_double(self.latitude)
		self.head.altitude = c_double(self.altitude)
		self.VERSION = VERSION
		self.head.version = self.VERSION
		m = hashlib.sha1()
		m.update(string_at(addressof(self.head),152))
		self.sha1 = m.hexdigest()
		self.head.sha1 = self.sha1.encode(encoding="utf-8")
		self.fileName = str(uuid5(block.namespace,self.sha1))
		self.head.rdlock = (c_int*10)(0)
		self.head.wrlock = (c_int*10)(0)
		
	def setSZ(self,sz,cp=0):
		self.dataSize = self.align2page(sz)
		self.cpSize = self.align2page(cp)
	
	def defaultSZ(self,sz,cp=0):
		self.dataSize = self.align2page(sz)
		self.cpSize = self.align2page(cp)
	
	def align2page(self,l):
		return int((l+block.align-1)/block.align)*block.align

	def setPosition(self,lo,la,hi):
		self.longitude = lo
		self.latitude = la
		self.altitude = hi

	def dump(self):
		for item in block.dumps:
			print(item,": ",self.__dict__[item])
	def toMap(self):
		r = {}
		for key in block.dumps:
			v = self.__dict__[key]
			if type(v) == type(b"a"):
				v = v.decode()
			r[key] = v
		return r

	def toFile(self,dir='/tmp/usrtos'):
		fn = os.path.join(dir,self.fileName)
		zeros = b'\0'*256
		with open(fn,'wb') as f:
			length = self.metaSize + self.dataSize
			while length>255:
				f.write(zeros)
				length -= 256
			f.write(zeros[:length])
			f.seek(0,0)
			f.write(string_at(addressof(self.head),sizeof(self.head)))
			f.flush()
			f.close()

	@staticmethod
	def fromMap(m):
		a = block(m["name"])
		a.type = m["type"].encode(encoding='utf-8')
		a.defaultSZ(m["dataSize"]*block.align,m["cpSize"]*block.align)
		a.metaSize = a.align2page(m["metaSize"]*block.align)
		a.setPosition(m["longitude"],m["latitude"],m["altitude"])
		a.genHead()
		a.dump()
		a.toFile()

def test():
	a = block("root")
	a.defaultSZ(4096*24,4096*3)
	a.setPosition(120.,40.,50.)
	a.genHead()
	a.dump()
	a.toFile()
	print(json.dumps(a.toMap(), sort_keys=True, indent=2))

def main(fn):
	with open(fn) as f:
		blocks=json.load(f)
		for m in blocks:
			block.fromMap(m)

if __name__ == '__main__':
	if len(sys.argv)==1:
		test()
	else:
		main(sys.argv[1])
	