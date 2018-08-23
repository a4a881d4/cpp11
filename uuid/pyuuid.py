import uuid
import hashlib

x = uuid.UUID("01234567-89ab-cdef-0123-456789abcdef")

y = uuid.uuid5(x,"rfbuffer")

print(str(y))

print(uuid.uuid5(x,"usrtos"))
