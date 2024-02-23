#! /usr/bin/bash

for resolution in {2..10}; do

output_file="outputG$resolution.txt"

for file in ./probing_benchmarks/*/*/*.bench*; do
    if [ -f "$file" ]; then
echo "Scanning $file"
        temp_output=$(mktemp)
        ./abc <<EOF > "$temp_output"
read_bench "$file"
bfs -g $resolution
scan -g -a
run
quit
EOF
        echo "Scanned $file:" >> "$output_file"
        tail -n 14 "$temp_output" >> "$output_file"
        tail -n 14 "$temp_output"
        rm "$temp_output"
        echo "Scan Complete"
        echo -e "\n" >> "$output_file"
    fi
done

./resultsToCSV.sh "$resolution"

done