# Darknet with NNPACK
This repo is developed based on the work of  https://github.com/thomaspark-pkj/NNPACK-darknet.git.
please refer the link for the detail, if needed.
I fork the repo to migrate accelrated darknet to ZCU102 platform and to see the performance.

## Build from ZCU102
Log in to ZCU102 using SSH.<br/>
Install [PeachPy](https://github.com/Maratyszcza/PeachPy) and [confu](https://github.com/Maratyszcza/confu)
```
sudo pip install --upgrade git+https://github.com/Maratyszcza/PeachPy
sudo pip install --upgrade git+https://github.com/b9515308/confu.git
```
Install [Ninja](https://ninja-build.org/)
```
git clone https://github.com/ninja-build/ninja.git
cd ninja
git checkout release
./configure.py --bootstrap
export NINJA_PATH=$PWD
```
Install clang
```
sudo apt-get install clang
```
Install [NNPACK-darknet](https://github.com/b9515308/NNPACK-darknet.git)
```
git clone https://github.com/b9515308/NNPACK-darknet.git
cd NNPACK-darknet
confu setup
python ./configure.py --backend arm
$NINJA_PATH/ninja

```
Build darknet-nnpack
```
git clone https://github.com/b9515308/darknet-nnpack.git
cd darknet-nnpack
make
```

## Test
The weight files can be downloaded from the [YOLO homepage](https://pjreddie.com/darknet/yolo/).
```
YOLOv2
./darknet detector test cfg/coco.data cfg/yolo.cfg yolo.weights data/person.jpg
Tiny-YOLO
./darknet detector test cfg/voc.data cfg/tiny-yolo-voc.cfg tiny-yolo-voc.weights data/person.jpg
```
## Result 
Model | Build Options | Prediction Time (seconds)
:-:|:-:|:-:
YOLOv2 | NNPACK=1,ARM_NEON=1 |?
YOLOv2 | NNPACK=0,ARM_NEON=0 | ?
Tiny-YOLO | NNPACK=1,ARM_NEON=1 | ?
Tiny-YOLO | NNPACK=0,ARM_NEON=0 |?
