# JACK++ – C++ wrapper for JACK

The **jack++** library is a lightweight C++ wrapper for the [JACK Audio Connection Kit](https://jackaudio.org/), simplifying audio processing and integration in C++ applications.

## Installation

### Binary

Debian/Ubuntu/etc:

```shell
sudo add-apt-repository ppa:ppa-verse/public
sudo apt install libjack++
```

Install the development package, if you are planning to develop applications with **jack++**:
```shell
sudo apt install libjack++-dev
```

RaspberryPi:

```shell
sudo add-apt-repository -S deb https://ppa.launchpadcontent.net/ppa-verse/public/ubuntu jammy main
sudo apt install libjack++
```

Install the development package, if you are planning to develop applications with **jack++**:
```shell
sudo apt install libjack++-dev
```

### From source

Stable version (requires [CMake](https://cmake.org/) >= 3.16):

```shell
p=jack v=1.0.2
wget https://github.com/dimitry-ishenko-media/${p}/archive/v${v}.tar.gz
tar xzf v${v}.tar.gz
mkdir ${p}-${v}/build
cd ${p}-${v}/build
cmake ..
make
sudo make install
```

Latest master (requires [git](https://git-scm.com/) and [CMake](https://cmake.org/) >= 3.16):

```shell
p=jack
git clone --recursive https://github.com/dimitry-ishenko-media/${p}.git
mkdir ${p}/build
cd ${p}/build
cmake ..
make
sudo make install
```

## Usage

To use **jack++** in your application simply add:

```c++
#include <jack++.hpp>
```

to your file(s) and link with `-ljack++`. For [CMake](https://cmake.org/)-based projects use:

```cmake
find_package(jack++ REQUIRED)
...
target_link_libraries(marvin PRIVATE jack++::jack++)
```

to find the library and link with it. **jack++** additionally exports `jack++::jack++_static` and `jack++::jack++_shared` targets, which you can use to explicitly link with the static and shared versions of the library respectively.

## Description

_TODO_

For now check the [example](./example/) directory for usage examples.

## Authors

* **Dimitry Ishenko** - dimitry (dot) ishenko (at) (gee) mail (dot) com

## License

This project is distributed under the GNU GPL license. See the
[LICENSE.md](LICENSE.md) file for details.
