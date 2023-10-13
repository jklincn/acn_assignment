#!/bin/bash
script_name=$1
ns_path="ns-allinone-3.40"

if [[ -z "$script_name" ]];then
    echo "ERROR: You need to specify a file."
    exit
fi
if [[ ! -e "$script_name" ]];then
    echo "ERROR: No such file."
    exit 
fi
if [[ ! -d "$ns_path" ]];then
    echo "ERROR: $ns_path not found in current path."
    echo "NOTE: Installation details can be found in README.md." 
    exit 
fi

cp $script_name $ns_path"/ns-3.40/scratch"
cd $ns_path"/ns-3.40"
./ns3 run "scratch/"$script_name
rm "scratch/"$script_name

