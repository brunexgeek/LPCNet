/* Copyright (c) 2018 Mozilla */
/*
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include "arch.h"
#include "lpcnet.h"
#include "freq.h"
#include "nnet_data.h"

#define MODE_ENCODE 0
#define MODE_DECODE 1
#define MODE_FEATURES 2
#define MODE_SYNTHESIS 3



static uint32_t now()
{
    #ifdef __WINDOWS__

    struct __timeb32 current;
    _ftime32_s(&current);
    return (uint32_t) ((current.time * 1000) + current.millitm);

    #else

    struct timespec current;
    clock_gettime(CLOCK_MONOTONIC, &current);
    return (uint32_t) ((current.tv_sec * 1000) + current.tv_nsec / (1000 * 1000));

    #endif
}




int main(int argc, char **argv) {
    int mode;
    FILE *fin, *fout;
    if (argc != 5)
    {
        fprintf(stderr, "usage: lpcnet_demo -encode <input.pcm> <compressed.lpcnet> <model>\n");
        fprintf(stderr, "       lpcnet_demo -decode <compressed.lpcnet> <output.pcm> <model>\n");
        fprintf(stderr, "       lpcnet_demo -features <input.pcm> <features.f32> <model>\n");
        fprintf(stderr, "       lpcnet_demo -synthesis <features.f32> <output.pcm> <model>\n");
        return 0;
    }
    if (strcmp(argv[1], "-encode") == 0) mode=MODE_ENCODE;
    else if (strcmp(argv[1], "-decode") == 0) mode=MODE_DECODE;
    else if (strcmp(argv[1], "-features") == 0) mode=MODE_FEATURES;
    else if (strcmp(argv[1], "-synthesis") == 0) mode=MODE_SYNTHESIS;
    else {
        exit(1);
    }
    fin = fopen(argv[2], "rb");
    if (fin == NULL) {
	fprintf(stderr, "Can't open %s\n", argv[2]);
	exit(1);
    }

    fout = fopen(argv[3], "wb");
    if (fout == NULL) {
	fprintf(stderr, "Can't open %s\n", argv[3]);
	exit(1);
    }

    if (mode == MODE_ENCODE) {
        LPCNetEncState *net;
        net = lpcnet_encoder_create();
        while (1) {
            unsigned char buf[LPCNET_COMPRESSED_SIZE];
            short pcm[LPCNET_PACKET_SAMPLES];
            fread(pcm, sizeof(pcm[0]), LPCNET_PACKET_SAMPLES, fin);
            if (feof(fin)) break;
            lpcnet_encode(net, pcm, buf);
            fwrite(buf, 1, LPCNET_COMPRESSED_SIZE, fout);
        }
        lpcnet_encoder_destroy(net);
    } else if (mode == MODE_DECODE) {
        nnet_data_load(argv[4], &defaultModel);
        LPCNetDecState *net;
        net = lpcnet_decoder_create();
        while (1) {
            unsigned char buf[LPCNET_COMPRESSED_SIZE];
            short pcm[LPCNET_PACKET_SAMPLES];
            fread(buf, sizeof(buf[0]), LPCNET_COMPRESSED_SIZE, fin);
            if (feof(fin)) break;
            lpcnet_decode(net, buf, pcm);
            fwrite(pcm, sizeof(pcm[0]), LPCNET_PACKET_SAMPLES, fout);
        }
        lpcnet_decoder_destroy(net);
    } else if (mode == MODE_FEATURES) {
        LPCNetEncState *net;
        net = lpcnet_encoder_create();
        while (1) {
            float features[4][NB_TOTAL_FEATURES];
            short pcm[LPCNET_PACKET_SAMPLES];
            fread(pcm, sizeof(pcm[0]), LPCNET_PACKET_SAMPLES, fin);
            if (feof(fin)) break;
            lpcnet_compute_features(net, pcm, features);
            fwrite(features, sizeof(float), 4*NB_TOTAL_FEATURES, fout);
        }
        lpcnet_encoder_destroy(net);
    } else if (mode == MODE_SYNTHESIS) {
        if (nnet_data_load(argv[4], &defaultModel))
        {
            perror("Unable to load model");
            return 1;
        }

        struct stat info;
        if (stat(argv[2], &info))
        {
            perror(argv[2]);
            return 1;
        }

        float *in_features = (float*) malloc(info.st_size);
        float *end_features = in_features + info.st_size / sizeof(float);
        if (!in_features) return 1;

        fread(in_features, 1, info.st_size, fin);

        LPCNetState *net;
        net = lpcnet_create();
        uint32_t ticks = now();
        uint64_t frames = 0;
        while (in_features < end_features) {
            float features[NB_FEATURES];
            short pcm[LPCNET_FRAME_SIZE];
            RNN_COPY(features, in_features, NB_FEATURES);
            RNN_CLEAR(&features[18], 18);
            lpcnet_synthesize(net, features, pcm, LPCNET_FRAME_SIZE);
            fwrite(pcm, sizeof(pcm[0]), LPCNET_FRAME_SIZE, fout);
            frames += LPCNET_FRAME_SIZE;
            in_features += NB_TOTAL_FEATURES;
        }
        ticks = now() - ticks;
        printf("Elapsed time: %d ms\n", ticks);
        printf("  Audio time: %d ms\n", (frames / 16));
        printf(" Performance: %.2f:1", (frames / 16.0F) / ticks );
        lpcnet_destroy(net);
    } else {
        fprintf(stderr, "unknown action\n");
    }
    fclose(fin);
    fclose(fout);
    return 0;
}
