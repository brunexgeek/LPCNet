#!/bin/bash -x

BASE=$(cd "$(dirname $0)/../"; pwd)
sudo docker run -it -u $(id -u):$(id -g) -v $BASE:/lpcnet lpcnet-train:1.0.0-gpu
