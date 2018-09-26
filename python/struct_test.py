from ctypes import *

class task(Structure):
	_fields_ =   [("argv",  c_char*32)
				, ("ID",    c_longlong)
				, ("noE",   c_longlong)
				, ("noL",   c_longlong)
				, ("valid", c_longlong)
				, ("key",   c_char*16)
				, ("cbkey", c_char*16)
				, ("cbarg", c_char*56)
				]
def getStructFieldOffest(st):
	r = {}
	for x in st._fields_:
		(a,b) = x
		s = repr(getattr(task,a))
		poff = s.find('ofs=')
		off = s[poff:]
		pcomma = off.find(',')
		ioff = int(off[4:pcomma])
		print(s,ioff)
		r[a] = ioff
	return r

of = getStructFieldOffest(task)
print(of)