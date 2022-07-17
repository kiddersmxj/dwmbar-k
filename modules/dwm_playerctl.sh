#!/bin/bash

# How many seconds after the music is paused the song stays at the top of the bar prepended with the pause status
pauseLength=600

# Gets all of the currently active players and puts them in an array
get_players() { readarray -t players < <(playerctl -l | awk '{gsub(/[[:punct:]]/, "\n")} 1' RS='[[:space:]]'); }

# Gets the status of all players in array (playing/paused)
get_stat() {
	for (( p=0; p<${#players[@]}; p++ )); do
		# Stat is rewritten for each player
		stat="$(playerctl -s --player=${players[p]} status)"
		if [ "$stat" = "Playing" ]; then
			# If playing then player is added to playing array
			playing[${#playing[@]}]=${players[p]}
		fi
	done
	# Number of playing players
	no_of_playing=${#playing[@]}
}

# Gets and sets value of song variable
get_playing() {
	# If there is only one player playing
	if [ $no_of_playing = 1 ]; then	
		# Set player to the only player in the array (the first one)
		player=${playing[0]}
		# Get position through the track
		position="$(playerctl position | sed 's/..\{6\}$//')"
		# Convert the position from seconds to min
		position="$(sec2min $position)"
		# Get artist of playing track in useable format
		artist="$(playerctl --player=$player metadata --format '{{ artist }}')"
		# Get title of playing track in useable format
		title="$(playerctl --player=$player metadata --format '{{ title }}')"
		# If unicode chars are enabled
		if [ "$unicode" = "1" ]; then
			# Set the track status to play symbol
			stat="▶"
		fi
		# Set stored song to song without the status symbol
		songStore="$artist - $title ($position)"
		# Set song to song with the status symbol
		song="$stat $songStore"
		# s is always 0 when song is playing as there is no paused track waiting to timeout
		s=0
	elif [ ${#playing[@]} = 0 ]; then
		# NOTHING PLAYING ******************************************
		# If s - paused timeout counter is greater or equal to pause timeout length
		if [ $s -gt $pauseLength ]; then
			# Clear all variables
			s=0
			song=""
			songStore=""
			stat=""
		# If stored song has any value
		elif [ "$songStore" != "" ]; then
			# If unicode chars are enabled
			if [ "$unicode" = "1" ]; then
				# Set status symbol to paused
				stat="⏸"
			fi
		# Set song to paused status as no song is playing but there is a stored song value so must be within the timeout ranger therefore display the stored song :)
		song="$stat $songStore"
		fi
	else
		# ERROR*****************************************************
		# Two or more players are playing at the same time
		song="*SONG-ERROR*"
	fi
}

# Convert seconds to mins
sec2min() { printf "%d:%02d" "$((10#$1 / 60))" "$((10#$1 % 60))"; }

# Called by main bar script to print song
print_song() {
	# If there is any value for song
	if [ "$song" != "" ]; then
		# Prepend song with | to make it look neat then print as a string
		printf "%s" "| $song"
	fi
}

# Called in main bar script to get song
get_song() {
	# Array of all players with songs in memory
	players=()
	# Array of all currently playing players (should only be 1 player)
	playing=()
	# Gets players and populates the players array
	get_players
	# Gets status and populates the playing array
	get_stat
	# Gets currently playing song if there is any - if not checks pause timeout count and displays paused track
	get_playing
}

