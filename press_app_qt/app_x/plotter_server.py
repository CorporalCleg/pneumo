import socket
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from icecream import ic


class pserver():
    def __init__(self, window_size=10):
        self.server = socket.socket()
        self.hostname = 'localhost'
        print(self.hostname)
        self.port = 1414
        self.server.bind((self.hostname, self.port))
        self.server.listen(5)

        self.window_size = window_size
        self.time = [0]
        self.targets = [0.0]
        self.data = [0.0]
        
        
        
    
    def recv_n_redraw(self, frame):

        #get telemetry from server
        self.conn, _ = self.server.accept()
        d = self.conn.recv(1024)
        self.conn.close()
        
        measure, target = d.decode().split()
    
        if len(self.time) > self.window_size:

            #update values
            self.data = self.data[-self.window_size:] + [float(measure)]
            self.targets = self.targets[-self.window_size:] + [float(target)]
            self.time = self.time[-self.window_size:] + [self.time[-1] + 1]

            #render
            axes.cla()
            axes.set_ylim(-3, 3)
            axes.set_xlim(self.time[-self.window_size], self.time[-1])
            axes.plot(self.time, self.data, color="red", label='measurements')
            axes.plot(self.time, self.targets, color="blue", label='target')
            axes.legend(loc=1)

            
        else:
            axes.plot(self.time, self.data, color="red", label='measurements')
            axes.plot(self.time, self.targets, color="blue", label='target')
            self.data.append(float(measure))
            self.targets.append(float(target))
            self.time.append(self.time[-1] + 1)

    

if __name__ == '__main__':
    fig, axes = plt.subplots(nrows=1, ncols=1, figsize=(10, 5))


    plt.style.use("ggplot")
    plt.ylim(-2, 2)

    s = pserver()
    anim = FuncAnimation(fig, lambda frame: s.recv_n_redraw(frame), frames = None, interval=500)

    plt.show()