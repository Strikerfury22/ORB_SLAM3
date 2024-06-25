#!/bin/bash
for nTokens in {1,5,15,22}
do
	./my_launch_files/launch_single_call_v3.sh MH01 $nTokens
done
