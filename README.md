# dwmbar-k 
[![CMake](https://img.shields.io/github/actions/workflow/status/kiddersmxj/dwmbar-k/cmake.yml?style=for-the-badge)](https://github.com/kiddersmxj/dwmbar-k/actions/workflows/cmake.yml)

A minature backend built for dwm making use of X11 status setting, written in cpp. Can be used with main and staus bar patches.

### Current and planned features
- [x] `XSR()` parsing handling and printing
- [x] Module handling
    - [x] Auto launching modules
    - [x] Re-launching modules
- [x] Modules
    - [x] media
    - [x] network
    - [x] battery
    - [x] bluetooth
    - [x] brightness
    - [x] cpu
    - [x] network signal/usage
    - [x] thermal throttling check
    - [x] time
    - [x] volume
    - [x] weather
- [x] Colour customization
- [x] Remove need for `launch.sh`
- [ ] Error handling
    - [x] Backup unsuccessful output in case of failure
    - [x] Write backup to output in case of failure
    - [ ] Warning for overly failed output
- [x] Code cleanup
    - [x] Separate std-k and dwmbar-k libs
- [x] Third bar

### Installation
For main branch `git clone https://github.com/kiddersmxj/dwmbar-k`

For latest release check https://github.com/kiddersmxj/dwmbar-k/releases

run `./install.sh` to install - this runs a cmake script to compile and install the project

### Usage
Put `$INSTALL_LOCATION/bin/dwmbar-k` in `.xinitrc` or equivalent

