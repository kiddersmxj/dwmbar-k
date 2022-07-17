#!/bin/sh

# A dwm_bar function to print the weather from wttr.in
# Joe Standring <git@joestandring.com>
# GNU GPLv3

# Dependencies: curl

# Change the value of LOCATION to match your city
dwm_weather() {
	location=London
	weather=$(curl -s wttr.in/$location?format=2 | grep -o ".[0-9].*{0,4}")
printf "%s" "$weather"
}

