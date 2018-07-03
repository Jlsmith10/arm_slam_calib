## Install arm_slam_calib on Ubuntu 14.04

## ROS Packages
* geometry_msgs
* cv_bridge
* sensor_msgs
* std_msgs
* roscpp
* image_transport
* visualization_msgs
* pcl_ros
* joint_state_recorder
### Install pcl_ros v.1.8
```


```

* [apriltags](https://github.com/personalrobotics/apriltags)
### Install april tags and dependencies 
#### !!!! NOTE: These commands are still not working, need to resolve by either using apriltags2_ros, which does work, or somehow fixing the compilation errors !!!!
```	
sudo apt-get install libcgal-dev
git clone https://github.com/personalrobotics/apriltags 
git clone https://github.com/swatbotics/apriltags-cpp 
```

* [joint_state_recorder](https://github.com/personalrobotics/joint_state_recorder)
### Install joint_state_recorder
```
git clone  https://github.com/personalrobotics/joint_state_recorder
catkin_make
```

## Non-ROS Dependencies
* [AIKIDO](https://github.com/personalrobotics/aikido)
```
git clone https://github.com/personalrobotics/aikido
git checkout tags/v0.2.0
mkdir build && cd build
cmake ..
make 
sudo make install
```

* [DART](https://github.com/dartsim/dart)
DART (Dynamic Animation and Robotics Toolkit) is an open source C++ library used for kinematic and dynamic applications.
### Install DART 6
```
sudo apt-add-repository ppa:libccd-debs
sudo apt-add-repository ppa:fcl-debs
sudo apt-add-repository ppa:dartsim
sudo apt-get update

sudo apt-get install libdart6-all-dev
```
OR install from source using the second part of these instructions (Install DART from source)
```
http://dartsim.github.io/install_dart_on_ubuntu.html#install-required-dependencies
TODO: add this to Dockerfile...
```
* [BRISK](https://github.com/clemenscorny/brisk)
```
git clone https://github.com/clemenscorny/brisk
git checkout opencv3

mkdir build && cd build
cmake ..
make
```
### NOTE: I had trouble installing this due to no briskConfig.cmake, but you can manually set the BRISK_ variables:
Add the following to ~/.bashrc:
```
export BRISK_LIBRARIES=/home/james/brisk/build/lib
export BRISK_INCLUDE_DIRS=/home/james/brisk/include
```


* [OpenCV](https://github.com/opencv/opencv)
### Use the OpenCV built-in to ROS Indigo, no need to install a separate one

* [OpenGV](https://github.com/laurentkneip/opengv)
#### NOTE: Not in catkin workspace, this is a generic CMake package.
```
git clone https://github.com/laurentkneip/opengv
```
* [GTSAM](https://github.com/devbharat/gtsam)
### Install GTSAM
#### NOTE: Not in catkin workspace, this is a generic CMake package.
```
git clone https://bitbucket.org/gtborg/gtsam

git checkout origin release/3.2.1 # TODO: Double check!

cd gtsam
mkdir build && cd build
cmake ..
make check
make install
```

Then add the following to ~/.bashrc:
```
export GTSAM_DIR=/home/james/gtsam/build/  // <-- insert the directory that you installed gtsam (replace /home/james)
```
* [Eigen3](http://eigen.tuxfamily.org/index.php?title=Main_Page)
### Use the Eigen3 built-in to ROS Indigo, no need to install

* [PCL](https://github.com/PointCloudLibrary/pcl)
### Install PCL 1.8
```
git clone https://github.com/PointCloudLibrary/pcl
cd pcl
git fetch --all --tags --prune
git checkout tags/pcl-1.8.0
mkdir build && cd build
cmake ..
make install
```
Point the PCL_DIR directory to the folder that contains the Config.cmake file
```
export PCL_DIR=/home/james/pcl/build/  // <-- insert the directory that you installed pcl (replace /home/james)
```
* [Assimp](https://github.com/assimp/assimp)

* [TBB](https://github.com/wjakob/tbb)

* [MicroHTTPd] 
`
sudo apt-get install libmicrohttpd-dev
`







