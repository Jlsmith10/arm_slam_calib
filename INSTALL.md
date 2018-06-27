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
sudo add-apt-repository ppa:libccd-debs/ppa
sudo add-apt-repository ppa:fcl-debs/ppa
sudo add-apt-repository ppa:dartsim/ppa
sudo add-apt-repository ppa:personalrobotics/ppa
sudo apt-get update

sudo apt-get install libaikido-all-dev
```

* [DART](https://github.com/dartsim/dart)
DART (Dynamic Animation and Robotics Toolkit) is an open source C++ library used for kinematic and dynamic applications.
### Install DART 5.1 (6 might work but haven't tested it out)
```
sudo apt-add-repository ppa:libccd-debs
sudo apt-add-repository ppa:fcl-debs
sudo apt-add-repository ppa:dartsim
sudo apt-get update

sudo apt-get install libdart5-dev
```
* [BRISK](https://github.com/clemenscorny/brisk)

* [OpenCV](https://github.com/opencv/opencv)

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
cd gtsam
mkdir build
cd build
cmake ..
make check
make install
```
* [Eigen3](http://eigen.tuxfamily.org/index.php?title=Main_Page)

* [PCL](https://github.com/PointCloudLibrary/pcl)
### Install PCL 1.8
```
git clone https://github.com/PointCloudLibrary/pcl
cd pcl
mkdir build
cd build
cmake ..
make install
```


* [Assimp](https://github.com/assimp/assimp)

* [TBB](https://github.com/wjakob/tbb)







