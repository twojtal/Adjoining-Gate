#! /usr/bin/bash

read -p "Please enter a benchmark set: " user_input

if [ "$user_input" = "c1908" ]; then
    echo "Running ./abc with command for c1908..."
elif [ "$user_input" = "b14" ]; then
    echo "Running ./abc with command for b14..."
elif [ "$user_input" = "c5315" ]; then
    echo "Running ./abc with command for c5315..."
elif [ "$user_input" = "des" ]; then
    echo "Running ./abc with command for des..."
else
    echo "Invalid input. Exiting script."
    exit 0
fi

for file in ./probing_benchmarks/$user_input/*/*.bench; do
    if [ -f "$file" ]; then
        ./abc <<EOF
read_bench "$file"
bfs
scan -g -a
run
scan -g -a
quit
EOF
echo "Scanned $file"
read -p "Press Enter to continue..."
    fi
done

#echo "Timing Results: "