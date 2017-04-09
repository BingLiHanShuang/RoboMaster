import protocol
import time
import random
a=protocol.Protocol('192.168.1.100',6000)
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
    send_pass = GeneraatePass()
    for i in range(5):
        b=raw_input()
        send_pad=GeneratePad()
        a.SendPadPass(send_pad, send_pass)

        print send_pad,send_pass

    time.sleep(1.5)
    #print 1
