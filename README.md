### 1. Download repo

### 2. Run command (build container):
```
sh install.sh
```

### 3. Opportunities of container usage:

#### 1. Work with CLI inside container:

```
docker run --rm -it -v /tmp/.X11-unix:/tmp/.X11-unix -v $(pwd):/app -e DISPLAY=$DISPLAY -u qtuser -p 12346:12345 --device /dev/ttyUSB1:/dev/ttyUSB0 pneumo bash
```

#### 2. Run script with app: 

```
sh run.sh
```