#!/bin/bash

if [[ $1 != "" ]]; then
    ARGS="-D$1"
fi

mkdir -p build
# Remove the user's existing config so the cmake install step re-copies the
# in-repo dwmbar.conf to ~/.dwmbar.conf (the install rule is a no-op when the
# file already exists).
rm -f "$HOME/.dwmbar.conf"
script -q -c "cmake -B build $ARGS && cmake --build build && sudo cmake --install build" | tee build/build.log

# Copyright (c) 2024, Maxamilian Kidd-May
# All rights reserved.

# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree. 

