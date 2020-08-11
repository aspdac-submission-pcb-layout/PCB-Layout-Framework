# A note for ASP-DAC 2021 reviewers: 

This repository has been prepared from the original. Some care has been taken to remove the names of the authors. This may have led to some odd wording in the files because an automated processes was used for the blinding. Also, the git submodules were merged in to a single git repository. This has led to some duplicate code where there was originally just a pointer.

# PCB-Layout-Framework

Top-level app for PCB P&R flow

### Documentation
[Documentation page](https://iccad2020-submission271.github.io/PCB-Layout-Framework/)

[Quickstart](https://iccad2020-submission271.github.io/PCB-Layout-Framework/quickstart.html)

### Prerequisites

- GCC >=4.8
- G++ >= 4.8
```
export CXX=g++
```
- SWIG >= 2.0
```
sudo apt-get update
sudo apt-get install swig
```
- Boost >= 1.6
```
sudo apt-get update
sudo apt-get install libboost-all-dev
```
- CMake >= 3.1
```
sudo apt-get update
sudo apt-get install cmake
```
- Current support for kicad_pcb format derived from KiCad v5.1.2

## Current issues

- Tested on CentOS 6
- Tested on Ubuntu 18.04 (Ubuntu 16.04 doesn't come with Python 3)
- Linking shared object files after swig compilation currently fails on OSX. Docker is a good workaround on OSX.

## License
  * BSD-3-clause License [[Link]](LICENSE)
