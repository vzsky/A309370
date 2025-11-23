
OUTPUTFILE=log/$(date +%Y%m%d_%H%M%S).log

# write the source code first
cat main.cpp > "$OUTPUTFILE"

# compile
g++-15 -std=c++20 -O3 -fopenmp main.cpp -o main

# run and append output to the same file
./main >> "$OUTPUTFILE" 2>&1

