#!/bin/bash

STACK_COLLAPSER="./stackcollapse-perf.pl"
FLAME_GENERATOR="./flamegraph.pl"

if [[ $# -eq 0 ]]; then
    echo Script usage:
    echo "genflame.sh [path-to-executable] (output_file_name)"
    echo "() - optional"
    exit 0
fi

  
if [[ -x $1 ]]; then
    colors=(
        "\033[38;2;255;0;0m"    # Red
        "\033[38;2;255;165;0m"  # Orange
        "\033[38;2;255;255;0m"  # Yellow
    )
    
    fileout="flame.svg"
    if [[ -n $2 ]]; then
        fileout=$2
    fi

    # The text to display
    text="flamegraph"
    echo -n "Generating "
    # Loop through each character and color
    for (( i=0; i<${#text}; i++ )); do
        printf "${colors[i % ${#colors[@]}]}${text:i:1}"
    done
    echo -e "\033[0m for program $1 to file $fileout..."

    sudo perf record -F 99 -a -g -- $1
    echo "Sampling ended..."
    echo "Generating framegraph from collected data..."
    sudo perf script > out.perf
    $STACK_COLLAPSER out.perf > out.folded
    $FLAME_GENERATOR out.folded > $fileout
    echo -e "\n\n"
    echo "Flamegraph generated in file $fileout!"
    echo "End of script."
else
    echo -e "\033[38;2;255;0;0mInvalid path."
    exit 1
fi
