# CMAKE build
building instructions for my AUTOr3pair project.
The versions used for developing are: 
* Ubuntu 22.04.2 LTS
* Cmake 3.27.0
* CGAL 6.0
* Geos 3.11.2
* gdb 12.1
* Val3dity 

### Linux enviroment
Seeing Cmake in combination with CGAL are not easy to use on windows, developing happens in a linux environment.
To get a linux enviroment you could use a "virtual enviroment" though WSL or make a Ubuntu partition on your pc. 
When you want to use WSL:
* In cmd run: ``wsl --install``  
* After that restart computer & open WSL. Now state (as asked) username & password of your choice

### Install the used dependencies
#### Cgal
install CGAL using the following 2 commands:
* ``sudo apt-get update``
* ``sudo apt-get install libcgal-dev``
  * Answer ``y`` when they ask "After this operation, 309 MB of additional disk space will be used.
        Do you want to continue?"

#### install cmake
To install CMake, run the following commands on your Ubuntu terminal:
- ``sudo apt-get install build-essential libssl-dev``
    - Answer ``y`` when they ask "After this operation, 132 MB of additional disk space will be used.
      Do you want to continue?"
- ``cd /tmp``
- ``wget https://github.com/Kitware/CMake/releases/download/v3.23.0/cmake-3.23.0.tar.gz``
- ``tar -zxvf cmake-3.23.0.tar.gz``
- ``cd cmake-3.23.0``
- ``./bootstrap``  (takes quite some time)
- ``make``  (takes quite some time)
- ``sudo make install``

To check if instalation was correct you can use: `` cmake --version``

#### install Geos
install geos through following command
- ``cd /tmp``
- ``wget https://download.osgeo.org/geos/geos-3.11.2.tar.bz2``
- ``tar xvfj geos-3.11.2.tar.bz2``
- ``cd geos-3.11.2``
- ``mkdir _build``
- ``cd _build``
- Set up the build
    - ``cmake -DCMAKE_BUILD_TYPE=Release ..``
    - ``cmake --build .``  (takes quite some time)
    - ``sudo cmake --build . --target install``

#### For val3dity
*use develop branch*

1. install Eigen library: `$ sudo apt install libeigen3-dev`
    - Answer ``y`` when they ask "After this operation, 9215 kB of additional disk space will be used.
      Do you want to continue?"
2. install GEOS library: `$ sudo apt install libgeos++-dev`
    - Answer ``y`` when they ask "After this operation, 5946 kB of additional disk space will be used.
      Do you want to continue?"



#### install GDB
- ``sudo apt-get update``
- ``sudo apt-get install gdb``
    - Answer ``y`` when they ask "After this operation, 35.1 MB of additional disk space will be used.
      Do you want to continue?"

#### errors
errors occured and fixed by:
```
CMake Error at /usr/lib/x86_64-linux-gnu/cmake/Boost-1.74.0/BoostConfig.cmake:141 (find_package):
  Could not find a package configuration file provided by "boost_filesystem"
  (requested version 1.74.0) with any of the following names:

    boost_filesystemConfig.cmake
    boost_filesystem-config.cmake

  Add the installation prefix of "boost_filesystem" to CMAKE_PREFIX_PATH or
  set "boost_filesystem_DIR" to a directory containing one of the above
  files.  If "boost_filesystem" provides a separate development package or
  SDK, be sure it has been installed.
Call Stack (most recent call first):
  /usr/lib/x86_64-linux-gnu/cmake/Boost-1.74.0/BoostConfig.cmake:258 (boost_find_component)
  /usr/local/share/cmake-3.23/Modules/FindBoost.cmake:594 (find_package)
  /mnt/c/Users/Lisa/Documents/Github/val3dity/CMakeLists.txt:46 (find_package)

```
fix by ``sudo apt -y install libboost-filesystem-dev ``

#### cgal draw
Not needed for implementation, but sometimes used to visually things during developping
```
sudo apt-get install libcgal-qt5-dev
sudo apt-get install qt5-default
sudo apt-get install build-essential
sudo apt-get install qtscript5-dev
sudo apt-get install libqt5svg5-dev
sudo apt-get install qttools5-dev
```
### JetBrains CLION or CLION Nova setting
When running the cmake project you need to run CMAKE with the option `-DLIBRARY=true`
![Toolchains.png](_images%2FToolchains.png)
![Clion_settings.png](_images%2FClion_settings.png)


