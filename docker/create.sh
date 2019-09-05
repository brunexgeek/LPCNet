#!/bin/bash -x

BASE=$(cd "$(dirname $0)"; pwd)
cd $BASE && sudo docker build -t lpcnet-train:1.0.0 .