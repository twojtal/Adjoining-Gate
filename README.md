# Adjoining Gate System:

The Adjoining Gate system is an open-source set of functions for implementing an optical side-channel attack prevention measure as an extention to logic locking. Leakage scanning functions rely on the open-source EOFM-based attack created by Zuzak et al. from "A Combined Logical and Physical Attack on Logic Obfuscation". A detailed overview of Adjoining Gate development, implementation, and functionality can be found in the paper titled:

  Adjoining Gates: Mitigating Optical Side-Channel Attacks on Integrated Circuits through Security-Aware Placement
  Author: Thomas Wojtal

This code is built on the Berkely ABC tool, which can be found at (https://people.eecs.berkeley.edu/~alanmi/abc/).

## Compiling:

To compile, clone this repo and then type `make`.

### Compiling as C or C++

The current version of ABC can be compiled with C compiler or C++ compiler.

 * To compile as C code (default): make sure that `CC=gcc` and `ABC_NAMESPACE` is not defined.
 * To compile as C++ code without namespaces: make sure that `CC=g++` and `ABC_NAMESPACE` is not defined.
 * To compile as C++ code with namespaces: make sure that `CC=g++` and `ABC_NAMESPACE` is set to the name of the requested namespace. For example, add `-DABC_NAMESPACE=xxx` to OPTFLAGS.

## Running the Program:

The program is run within the ABC synthesis tool. After successfully compiling abc, Adjoining Gate functions can be run from the abc command line. To do so:

* Launch the abc command line tool

  ```./abc```

* Read in an obfuscated benchmark circuit. Several have been provided in the probing_benchmarks directory of this repo.

  ```read_bench <BENCHMARK_NAME>```

* Group the circuit into Adjacency Tags.

  ```abc 01> bfs -g <group_size>```

* Launch the leakage scan on this benchmark. The usage notes for the command is shown below:

  ```abc 01> scan -g -a```

    usage: scan [-apgkclovrh]
                The physical portion of the CLAP attack in ABC.
    -a         : list nodes in adjacency tag order
    -p         : prints only the leakage of the network
    -g         : scan the groups based on adjacency tag (network must first be BFS grouped)
    -k <key>   : input the correct oracle key value for EOFM probing simulation
    -c <int>   : maximum number of key inputs for a node to be considered for EOFM probing [default = 7]
    -l <float> : minimum portion of keyspace that must be eliminated for a multi-node probe to be run [default = 0.006125]
    -o <str>   : set name of SAT solver output file from physical portion of CLAP attack [default = \"physical_clap_out.bench\"]
    -v         : toggle printing verbose information [default = %s]
    -r <int>   : set the probe resolution size [default = 1]
    -h         : print the command usage

* Use the run command to apply Adjoining Gates to all leaking nodes.

  ```abc 01> run```

## Adjoining Gate Example Runs

### Automated Run

  $ ./autoRun.sh

### Manual Run

  $ ./abc
  UC Berkeley, ABC 1.01 (compiled Jan 29 2024 23:04:40)
  abc 01> read_bench ./probing_benchmarks/c1908/SLL/c1908_SLL.bench
  abc 02> bfs
  abc 02> scan -g -a
  abc 02> run
  abc 02> scan -g -a
  
## Benchmarks

All benchmarks from the ICCAD'22 manuscript can be found in the `probing_benchmarks` directory. The correct key value for each of these benchmarks is contained in the `probing_benchmarks/iccad_benchmark_keys.txt`.

b14:
read_bench ./probing_benchmarks/b14/AntiSAT/b14_BA16_gate_level_final.v.bench
read_bench ./probing_benchmarks/b14/Full-Lock/b14_locked_40_40_gate_level_final.v.bench
read_bench ./probing_benchmarks/b14/SFLL/b14_SFLL_277_gate_level_final.v.benchfix
read_bench ./probing_benchmarks/b14/SLL/b14_SLL.bench

c1908:
read_bench ./probing_benchmarks/c1908/AntiSAT/c1908_BA16.bench
read_bench ./probing_benchmarks/c1908/Full-Lock/c1908_locked_32_32_gate_level_final.v.bench
read_bench ./probing_benchmarks/c1908/SFLL/c1908_33_gate_level_final.v.bench
read_bench ./probing_benchmarks/c1908/SLL/c1908_SLL.bench

c5315:
read_bench ./probing_benchmarks/c5315/AntiSAT/c5315_BA16_gate_level_final.v.bench
read_bench ./probing_benchmarks/c5315/Full-Lock/c5315_locked_40_40_gate_level_final.v.bench
read_bench ./probing_benchmarks/c5315/SFLL/c5315_SFLL_178_gate_level_final.v.bench
read_bench ./probing_benchmarks/c5315/SLL/c5315_SLL.bench

des:
read_bench ./probing_benchmarks/des/AntiSAT/des_BA16_gate_level_final.v.bench
read_bench ./probing_benchmarks/des/Full-Lock/des_locked_40_40_gate_level_final.v.bench
read_bench ./probing_benchmarks/des/SFLL/des_SFLL_256_gate_level_final.v.benchfix
read_bench ./probing_benchmarks/des/SLL/des_SLL.bench

## Final remarks:

For questions, please reach out to:

Thomas Wojtal <tsw4235@rit.edu> or Michael Zuzak <mjzeec@rit.edu>
Department of Computer Engineering
Rochester Institute of Technology (RIT) 
    
## Acknowledgements:

A special thanks to the NSF for supporting this work under Grant 2245573.
