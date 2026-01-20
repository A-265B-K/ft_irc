#!/bin/bash
colours=(31 33 32 36 34 35)
state_file=".step_state"

if [ -f "$state_file" ]; then
	i=$(cat "$state_file")
else
	i=0
fi

colour=${colours[$((i % ${#colours[@]}))]}
printf "\033[${colour}m|\033[0m"

echo $((i + 1)) > "$state_file"