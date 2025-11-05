#!/bin/bash

# all_code_files.txt should contain all filenames (one per line)
filelist="all_code_files.txt"

# loop through each filename
while IFS= read -r file; do
    # check if file exists before printing
    if [[ -f "$file" ]]; then
        echo "========== [ START OF $file ] =========="
        cat "$file"
        echo "========== [ END OF $file ] =========="
        echo
    else
        echo "[!] File '$file' not found, skipping..."
    fi
done < "$filelist"
