### 1. Download repo and enter to the root directory of project:
```
cd pneumo
```

### 2. Run command (build container):
```
sh install.sh
```

### 3. Opportunities of container usage (first you need to connect Arduino to port):

#### 1. Work with CLI inside container:
```
docker run --rm -it -v /tmp/.X11-unix:/tmp/.X11-unix -v $(pwd):/app -e DISPLAY=$DISPLAY -u qtuser -p 1414:1414 --device /dev/ttyUSB1:/dev/ttyUSB0 pneumo bash
```

to run script use:

```
python3 press_app_qt/app_x/plotter_client.py
```


#### 2. Run script with app: 

 - __in mode without graphics__:

```
sh run.sh
```

- __in mode with graphics__:

run in current termial:


```
sh run.sh graph
```

next open another termial and execute:

```
python3 ~/pneumo/press_app_qt/app_x/plotter_client.py
```

