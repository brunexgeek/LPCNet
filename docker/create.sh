#!/bin/bash -x

BASE=$(cd "$(dirname $0)"; pwd)
cd $BASE && sudo docker build -t lpcnet-train:2.0.0 .