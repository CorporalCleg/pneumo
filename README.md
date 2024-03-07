### 1. Download repo

### 2. Run command (build container):
```
docker build -t pneumo_app .
```

### 3. Opportunities of container usage:

#### 1. Work with CLI inside container:

```
docker run --rm -it -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=$DISPLAY -u qtuser pneumo_app bash
```

#### 2. Run script with app: 

in first terminal run interface in container:
```
docker run --rm -it -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=$DISPLAY -u qtuser -p 12346:12345 pneumo_app  python3 app_x/main.py
```

in second terminal run
```
python3 press_app_qt/app_x/client.py    
```