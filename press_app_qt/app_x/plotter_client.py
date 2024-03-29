import socket
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation


class p_client():
    def __init__(self, window_size=10):
        self.port = 1414
        self.hostname = 'localhost'
        self.server_addr = (self.hostname, self.port)

        self.window_size = window_size
        self.time = [0]
        self.targets = [0.0]
        self.data = [0.0]
        
    
    def recv_n_redraw(self, frame):

        #client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client = socket.socket()
        #ic(self.hostname)
        client.connect(self.server_addr)

        #get telemetry from server
        d = client.recv(1024)
        client.close()

        measure, target = d.decode().split()
    
        if len(self.time) > self.window_size:

            #update values
            self.data = self.data[-self.window_size:] + [abs(float(measure))]
            self.targets = self.targets[-self.window_size:] + [float(target)]
            self.time = self.time[-self.window_size:] + [self.time[-1] + 0.5]

            #render
            axes.cla()
            axes.set_title('Pressure')

            axes.set_xlabel("time, )")
            axes.set_ylabel("Pressure, kPa")
            axes.set_ylim(min(self.data + self.targets) - 0.25, max(self.data + self.targets) + 0.25)
            axes.set_xlim(self.time[-self.window_size], self.time[-1])
            axes.plot(self.time, self.data, color="red", label='measurements')
            axes.plot(self.time, self.targets, color="blue", label='target')

            axes.grid(True, linestyle='-.')
            axes.legend(loc=1)
            
        else:
            axes.plot(self.time, self.data, color="red", label='measurements')
            axes.plot(self.time, self.targets, color="blue", label='target')
            self.data.append(float(measure))
            self.targets.append(float(target))
            self.time.append(self.time[-1] + 0.5)

    

if __name__ == '__main__':
    fig, axes = plt.subplots(nrows=1, ncols=1, figsize=(10, 5))


    
    plt.style.use("Solarize_Light2")

    plt.ylim(-2, 2)

    clnt = p_client()
    anim = FuncAnimation(fig, lambda frame: clnt.recv_n_redraw(frame), frames = None, interval=50)
    
    plt.show()
