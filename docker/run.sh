#!/bin/bash -x

BASE=$(cd "$(dirname $0)/../"; pwd)
RUNTIME="--runtime=nvidia"
if [ "$1" == "cpu"]; then
	RUNTIME=''
fi
sudo docker run -it $RUNTIME -v $BASE:/lpcnet lpcnet-train:1.0.0 $@
