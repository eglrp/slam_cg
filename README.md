# slam_cg

SLAM source code using OpenCV,Eigen,Sophus,Pangolin libraries.

---

# Prerequisites

* OpenCV 3.x.x
* Eigen3
  - `sudo apt install libeigen3-dev`
* Pangolin
  - required: `sudo apt install libglew-dev`
* Sophus
  - recommend version: commit id **a621ff2** (a621ff2e56c56c839a6c40418d42c3c254424b5c)
* JsonCpp
  - `sudo apt install libjsoncpp-dev`
* GTest
  - [install_gtest.sh](https://gist.github.com/cggos/edd0f8a96b9b6126afc0759beb716acd)

# Build

```bash
mkdir build & cd build
cmake .. & make -j4
```
