#!/bin/bash -x

BASE=$(cd "$(dirname $0)/../"; pwd)
sudo nvidia-docker run -it -v $BASE:/lpcnet lpcnet-train:1.0.0
