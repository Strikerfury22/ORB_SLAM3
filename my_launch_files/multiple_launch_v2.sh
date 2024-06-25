#!/bin/bash
for nTokens in {1,5,15,22}
do
	./my_launch_files/launch_single_call_v2.sh MH01 $nTokens "testAlone"
done
