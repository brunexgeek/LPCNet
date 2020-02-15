FROM tensorflow/tensorflow:1.14.0-gpu-py3

RUN apt update && apt install -y \
    cmake \
    make \
    wget \
    nano \
    cgdb \
    sox \
    file && apt clean

RUN pip3 install keras==2.2.4

ENTRYPOINT [ "/lpcnet/docker/entrypoint.sh" ]
