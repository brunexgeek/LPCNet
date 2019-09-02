FROM tensorflow/tensorflow:1.14.0-gpu-py3-jupyter

RUN apt update && apt install -y \
    cmake \
    make \
    file && apt clean

ENTRYPOINT [ "/bin/bash" ]