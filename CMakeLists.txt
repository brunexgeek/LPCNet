project(LPCNet)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g -mavx2 -mfma -Wextra")

add_library(liblpcnet STATIC
    src/common.c
    src/lpcnet.c
    src/lpcnet_dec.c
    src/lpcnet_enc.c
    src/nnet.c
    model/nnet_data.c
    model/ceps_codebooks.c
    src/pitch.c
    src/freq.c
    src/kiss_fft.c
    src/celt_lpc.c)
target_include_directories(liblpcnet
    PRIVATE "model"
    PRIVATE "src"
    PUBLIC "include")


add_executable(lpcnet_demo
    src/lpcnet_demo.c)
target_link_libraries(lpcnet_demo liblpcnet m)
target_include_directories(lpcnet_demo
    PUBLIC "include")


add_executable(dump_data
    src/common.c
    src/dump_data.c
    src/freq.c
    src/kiss_fft.c
    src/pitch.c
    src/celt_lpc.c
    src/lpcnet_dec.c
    src/lpcnet_enc.c
    model/ceps_codebooks.c)
target_link_libraries(dump_data liblpcnet m)
target_include_directories(dump_data
    PRIVATE "model"
    PRIVATE "src"
    PUBLIC "include")