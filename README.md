![Alt text](https://i.imgur.com/g6F8NX1.png)

WooD is intended to be a recreation of the Doom engine written in C++. This repository houses the source code and various other files required for building

## Getting Started
### Prerequisites
| Name            | Description    |
|-----------------|----------------|
| cmake           | build system   |
| libglfw3        | OpenGL library |
| build-essential | C/C++ compiler |

You will also need DOOM2.wad

### Building
It is recommended to create a separate a folder for building. CMake is used to to generate the build files.

    mkdir build && cd build
    cmake ..
    make