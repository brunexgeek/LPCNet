#!/bin/bash -x

cd /lpcnet

if [ -z "$1" ]; then
	/bin/bash
else
	$@
fi
