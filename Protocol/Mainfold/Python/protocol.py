import socket
import protocol_pb2 as protobuf
class Protocol:
    def __init__(self,address,port):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.address=(address,int(port))
    def SendToUDP(self,data):
        self.socket.sendto(data,self.address)
    def Send(self,datatype,data):
        res=protobuf.Message()
        res.messagetype=datatype
        res.data=data
        self.SendToUDP(res.SerializeToString())
    def SendPadPass(self,pad,password):
        res=protobuf.PadPass()
        res.pad=pad
        res.password=password
        self.Send(protobuf.Message.PadPass,res.SerializeToString())


# a=Protocol('192.168.1.107',6000)
# #a.Send(protobuf.Message.PADPASS,"wzq")
# a.SendPadPass("528796413","58971")