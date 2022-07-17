#!/bin/bash

# Name of direcotry where modules are stored (in current local dir)
moduleDir="modules"

# Setting $moduleDir to current local directory/$moduleDir
localDir=$(readlink -f "$0")
moduleDir="$(dirname "$localDir")/$moduleDir"

# Prepending and appending string for each module on bar
pre=" "
app=" |"

# Enable unicode (1/0)
unicode="1"

# Interval between long_functions execution (divisor)
longFunctionsInterval=10

# Counters
# long_functions
l=0
# get_song
s=0

# Import modules
. "$moduleDir/dwm_alsa.sh"
. "$moduleDir/dwm_date.sh"
. "$moduleDir/dwm_networkmanager.sh"
. "$moduleDir/dwm_playerctl.sh"
. "$moduleDir/dwm_weather.sh"

# Functions to run only at specified interval as they are more system intensive
long_functions() {
	internet="$(dwm_networkmanager)"
}

while true; do
	# How many seconds-1 between each longFunctions run
	if [ $(( $l % $longFunctionsInterval )) = 0 ]; then
		l=0
		long_functions
	fi

	mainbar=""
	mainbar="$mainbar$internet"
	mainbar="$mainbar $(dwm_date) "
	
	subbar=" "
	subbar="$subbar$(dwm_weather) |"
	subbar="$subbar l=$l : s=$s | "

	# Counter for long_functions
	((l=l+1))
	# Pause timeout counter for get_song (dwm_playerctl.sh)
	((s=s+1))

	get_song

	subbar="$subbar$(dwm_alsa) "
	subbar="$subbar$(print_song)"
	
	xsetroot -name "$mainbar;$subbar"
	
	sleep 1
done

