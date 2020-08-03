import socket
import time
HOST = '192.168.2.95'  # The server's hostname or IP address
PORT = 23        # The port used by the server

from tkinter import *
root=Tk()
root.minsize(400, 400)
root.title('MapperCar')
car = Label(root, text="x")
car.place(x=200, y=200)
obj_front = Label(root, text="--")
obj_front.place(x=200, y=200)
obj_back = Label(root, text="--")
obj_back.place(x=200, y=200)
obj_left = Label(root, text="|")
obj_left.place(x=200, y=200)
obj_right = Label(root, text="|")
obj_right.place(x=200, y=200)
data = ''

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    while 1:
        data = repr(s.recv(1024))
        #print('Received', repr(data))
        dist_front = data[2:5]
        dist_left = data[6:9]
        dist_right = data[10:13]
        dist_back = data[14:17]
        print("F:" + dist_front)
        print("L:" + dist_left)
        print("R:" + dist_right)
        print("B:" + dist_back)
        print("----------------")
        
        obj_front.place(x=200, y=200 - int(dist_front))
        obj_back.place(x=200, y=200 + int(dist_back))
        obj_left.place(x=200 - int(dist_left), y=200)
        obj_right.place(x=200 + int(dist_right), y=200)
        root.update()
