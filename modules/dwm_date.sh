#!/bin/sh

# A dwm_bar function that shows the current date and time

# Date is formatted like like this: "[Mon 01-01-00 00:00:00]"
dwm_date () {
	#printf "%s" "$pre"
	if [ "$unicode" = "1" ]; then
		printf "📆 $(date "+%a %d-%m-%y %T")"
	else
		printf "%s$(date "+%a %d-%m-%y %T")"
	fi
	#printf "%s" "$app"
}

