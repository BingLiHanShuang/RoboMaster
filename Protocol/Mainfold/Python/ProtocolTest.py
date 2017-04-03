import protocol
import time
a=protocol.Protocol('127.0.0.1',6000)

while True:
    time.sleep(1.5)
    a.SendPadPass("123456789","12345")
    print 1
