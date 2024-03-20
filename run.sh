sudo chmod a+rw /dev/ttyUSB0
sudo chmod a+rw /dev/ttyUSB1

docker run --rm -it -v /tmp/.X11-unix:/tmp/.X11-unix -v $(pwd):/app -e DISPLAY=$DISPLAY -p 1414:1414 -u qtuser --device /dev/ttyUSB0:/dev/ttyUSB0 pneumo python3 press_app_qt/app_x/main.py
docker run --rm -it -v /tmp/.X11-unix:/tmp/.X11-unix -v $(pwd):/app -e DISPLAY=$DISPLAY -p 1414:1414 -u qtuser --device /dev/ttyUSB1:/dev/ttyUSB0 pneumo python3 press_app_qt/app_x/main.py