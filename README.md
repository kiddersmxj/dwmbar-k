# dwmbar-k 
[![CMake](https://img.shields.io/github/actions/workflow/status/kiddersmxj/dwmbar-k/cmake.yml?style=for-the-badge)](https://github.com/kiddersmxj/dwmbar-k/actions/workflows/cmake.yml)

A minature backend built for dwm making use of X11's `xsetroot -name` written in cpp. Can be used with main and staus bar patches.

### Current and planned features
- [x] Modules output
- [x] Parent program handles output
- [x] `XSR()` parsing handling and printing
- [x] Module handling
    - [x] Auto launching modules
    - [x] Re-launching modules
- [x] Synchronisation
    - [x] Internal program clock
    - [x] Multi program clock
- [ ] Additional modules
    - [x] media-k
    - [ ] network-k
- [ ] Colour customization
- [x] Remove need for `launch.sh`
- [ ] Error handling
    - [x] Backup unsuccessful output in case of failure
    - [x] Write backup to output in case of failure
    - [ ] Data file passing to parent
    - [ ] Warning for overly failed output
- [ ] Code cleanup
    - [ ] Separate std-k and dwmbar-k libs
- [ ] Third bar
    - [ ] Get bar length
    - [ ] Auto resising module output

### Installation
For main branch `git clone https://github.com/kiddersmxj/dwmbar-k`

For latest release check https://github.com/kiddersmxj/dwmbar-k/releases

Run `sudo make` in local repo - Makefile does not install to `/usr/bin` as program relies o multiple binaries in dictated directory structure to work

### Usage
Put `$INSTALL_LOCATION/bin/dwmbar-k &` in `.xinitrc` or equivalent

