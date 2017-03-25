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
    def SendQRCode(self,size,position,angle=0.01):
        res=protobuf.ScanResult()
        for i in position:
            point=res.position.add()
            point.x=i[0]
            point.y = i[1]
        res.picrutesize.x=size[0]
        res.picrutesize.y = size[1]
        res.angle=angle
            # res.position.append(point)

        self.Send(protobuf.Message.ScanResult, res.SerializeToString())

#
#
#
# a=Protocol('192.168.1.107',6000)
# # #a.Send(protobuf.Message.PADPASS,"wzq")
# a.SendQRCode([640,480],[[1,2],[3,4],[5,6],[7,8]])