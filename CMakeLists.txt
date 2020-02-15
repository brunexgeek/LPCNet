project(LPCNet)
cmake_minimum_required(VERSION 2.8)

if (UNIX)
    set(SIMD_API "-mavx2 -mfma")
    #set(SIMD_API "-mavx")
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wfatal-errors -pedantic -std=gnu99 -Wl,--no-undefined -fPIC -Wall -Wextra -Wconversion -Werror=return-type -Werror=implicit-function-declaration")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors -pedantic -std=c++11 -Wl,--no-undefined -fPIC -Wall -Wextra -Wconversion -Werror=return-type -Werror=implicit-function-declaration")

    set(CMAKE_C_FLAGS_RELEASE  "${CMAKE_C_FLAGS_RELEASE} -s -O3 ${SIMD_API}")
    set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -s -O3 ${SIMD_API}")

    set(CMAKE_C_FLAGS_DEBUG  "${CMAKE_C_FLAGS_DEBUG} -g -O0 ${SIMD_API}")
    set(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 ${SIMD_API}")
endif()

add_library(liblpcnet STATIC
    src/common.c
    src/lpcnet.c
    src/lpcnet_dec.c
    src/lpcnet_enc.c
    src/nnet.c
    src/model.c
    src/ceps_codebooks.c
    src/pitch.c
    src/freq.c
    src/kiss_fft.c
    src/celt_lpc.c)
target_include_directories(liblpcnet
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
    src/ceps_codebooks.c)
target_link_libraries(dump_data m)
target_compile_definitions(dump_data PRIVATE -DDUMP_DATA=1)
target_include_directories(dump_data
    PRIVATE "model"
    PRIVATE "src"
    PUBLIC "include")