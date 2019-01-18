# slam-cg

SLAM source code using OpenCV,Eigen,Sophus,Pangolin libraries.

---

# Prerequisites

* OpenCV
* Eigen3
* Pangolin (**GLEW** needed)
* Sophus (commit id `a621ff2`)
* JsonCpp
  - `sudo apt install libjsoncpp-dev`
* GTest
  - [install_gtest.sh](https://gist.github.com/cggos/edd0f8a96b9b6126afc0759beb716acd)

# Build

```bash
mkdir build & cd build
cmake .. & make -j4
```
