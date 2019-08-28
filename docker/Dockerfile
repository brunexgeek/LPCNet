FROM tensorflow/tensorflow:1.13.2-gpu-py3

RUN apt update && apt install -y \
    cmake \
    make \
    file && apt clean

RUN pip3 install keras

ENTRYPOINT [ "/bin/bash" ]