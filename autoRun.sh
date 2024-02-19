#! /usr/bin/bash

output_file="outputG2.txt"

for file in ./probing_benchmarks/*/*/*.bench; do
    if [ -f "$file" ]; then
echo "Scanning $file"
        temp_output=$(mktemp)
        ./abc <<EOF > "$temp_output"
read_bench "$file"
bfs -g 2
scan -g -a
run
quit
EOF
        echo "Scanned $file:" >> "$output_file"
        tail -n 13 "$temp_output" >> "$output_file"
        tail -n 13 "$temp_output"
        rm "$temp_output"
        echo "Scan Complete"
        echo -e "\n" >> "$output_file"
    fi
done

./resultsToCSV.sh