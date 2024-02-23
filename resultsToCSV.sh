#!/bin/bash

#Obtain parameter
if [ -z "$1" ]; then
    echo "Usage: $0 <integer>"
    exit 1
fi

# Assign the first parameter to a variable
resolution="$1"

# Set default values for input and output file paths
input_file="outputG$resolution.txt"
output_file="resultsG$resolution.csv"

# Use awk to filter and extract specific results with headers
awk -F ': ' '/^Scanned/ {benchmark_path = $1; scanned_found = 1; next}
            {if (scanned_found) {
                gsub("Scanned ", "", benchmark_path);
                gsub(":", "", benchmark_path);
                if (/^Scan Time/) {scan_time = $2}
                else if (/^PIs/) {pis = $2}
                else if (/^POs/) {pos = $2}
                else if (/^Total nodes in circuit/) {total_nodes = $2}
                else if (/^Nodes visited/) {nodes_visited = $2}
                else if (/^Nodes leaking key information/) {nodes_leaking = $2}
                else if (/^Added adjoining gates to/) {added_gates = $NF; gsub(/[^0-9]/, "", added_gates)}
                else if (/^Adjoining Gate Add Runtime/) {add_runtime = $2}
                else if (/^Total Adjoining Gate Inputs Reduced/) {pi_reduction = $2}
                else if (/^Final PIs/) {final_pis = $2}
                else if (/^Final POs/) {final_pos = $2}
                else if (/^[a-zA-Z0-9_]+ [0-9]+>[[:space:]]*quit/) {
                    if (!header_printed) {
                        print "Benchmark Path,Scan Time,PIs,POs,Total Nodes,Nodes Visited,Nodes Leaking,Added Nodes,Add Runtime,Total PIs Reduced,Final PIs,Final POs";
                        header_printed = 1;
                    }
                    print benchmark_path "," scan_time "," pis "," pos "," total_nodes "," nodes_visited "," nodes_leaking "," added_gates "," add_runtime "," pi_reduction "," final_pis "," final_pos;
                    scanned_found = 0;
                }
            }
            } END { if (header_printed == 0) { 
                    print "Benchmark Path,Scan Time,PIs,POs,Total Nodes,Nodes Visited,Nodes Leaking,Added Nodes,Add Runtime,Total PIs Reduced,Final PIs,Final POs";
                }
            }' "$input_file" > "$output_file"

echo "Extraction complete. Output saved to $output_file"