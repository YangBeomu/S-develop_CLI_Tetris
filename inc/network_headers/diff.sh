#!/bin/bash

FIRST_DIR="."
SECOND_DIR="/home/202434-157206/beomu/project/Wizbz_Free-LTE/include/network_headers"

if [[ ! -d "$FIRST_DIR" || ! -d "$SECOND_DIR" ]]; then
	echo "Failed to find directorys"
	exit 1
fi

for file in "$FIRST_DIR"/*; do
	filename=$(basename "$file")
	second_file="$SECOND_DIR/$filename"

	if [[ -f "$second_file" ]]; then
		echo "====Diff : $filename==="
		diff -bB "$file" "$second_file"
		echo
	else
		echo "Failed to find files"
	fi
done

