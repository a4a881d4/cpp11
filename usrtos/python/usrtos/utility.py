def getStructFieldOffest(st):
	r = {}
	for x in st._fields_:
		(a,b) = x
		s = repr(getattr(st,a))
		poff = s.find('ofs=')
		off = s[poff:]
		pcomma = off.find(',')
		ioff = int(off[4:pcomma])
		r[a] = ioff
	return r