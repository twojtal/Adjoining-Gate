#! /usr/bin/bash

for file in ./probing_benchmarks/*/*/*.bench; do
    if [ -f "$file" ]; then
echo "Scanning $file"
        temp_output=$(mktemp)
        ./abc <<EOF > "$temp_output"
read_bench "$file"
bfs
scan -g -a
run
quit
EOF
        echo "Scanned $file:" >> outputDes.txt
        tail -n 13 "$temp_output" >> outputDes.txt
        tail -n 13 "$temp_output"
        rm "$temp_output"
        echo "Scan Complete"
        echo -e "\n" >> outputDes.txt
    fi
done

./resultsToCSV