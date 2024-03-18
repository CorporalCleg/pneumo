import socket
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

time = [1]
data = [0.0]

fig, ax = plt.subplots()
graph = ax.plot(time, data)[0]
plt.ylim(-2, 2)

class server():
    def __init__(self):
        self.server = socket.socket()
        self.hostname = socket.gethostname()
        self.port = 12345
        self.server.bind((self.hostname, self.port))
        self.server.listen(5)
        
        
        
    
    def recv(self, frame):
        global graph
        self.conn, _ = self.server.accept()
        d = self.conn.recv(1024)
        self.conn.close()
        data.append(float(d.decode()))
        time.append(time[-1] + 1)

        if len(data) > 6:
            graph.set_xdata(time)
            graph.set_ydata(data)
            plt.xlim(time[-6], time[-1])
            

    # def send(self, target: dict):
    #     self.conn, _ = self.server.accept()
    #     data = json.dumps(target).encode()
    #     data = self.conn.send(data)
    #     self.conn.close()
    #     print('call')

    #     return
    
s = server()
anim = FuncAnimation(fig, lambda frame: s.recv(frame), frames = None)
plt.show()