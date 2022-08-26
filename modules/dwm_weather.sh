#!/bin/sh

# A dwm_bar function to print the weather from wttr.in
# Dependencies: curl

# Change the value of LOCATION to match your city
dwm_weather() {
	testSign=$(curl -s wttr.in/$LOCATION?format=1)
	if [[ "$testSign" == "-" ]]; then
		sign="-"
	fi
	weather="$sign$(curl -s wttr.in/$LOCATION?format=1 | grep -o "[0-9].")"
	weather="$(curl -s wttr.in/$LOCATION?format=1 | hexdump -v -e '"\\\x" 1/1 "%02x"' --length 3) $weather"
	weather="$weather\xc2\xb0C"

printf "$weather"
}

