import protocol
import time
import random
a=protocol.Protocol('127.0.0.1',6000)
def GeneratePad():
    num_origin=[1,2,3,4,5,6,7,8,9]
    num_res=""
    for i in range(9):
        index=random.randint(0,len(num_origin)-1)
        num_res+=str(num_origin[index])
        num_origin.remove(num_origin[index])
    return num_res
def GeneraatePass():
    num_origin=[1,2,3,4,5,6,7,8,9]
    num_res=""
    for i in range(5):
        index=random.randint(0,len(num_origin)-1)
        num_res+=str(num_origin[index])
        num_origin.remove(num_origin[index])
    return num_res

while True:

    send_pad=GeneratePad()
    send_pass=GeneraatePass()
    print send_pad,send_pass
    a.SendPadPass(send_pad,send_pass)
    time.sleep(1.5)
    #print 1
