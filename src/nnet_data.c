#include "nnet_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

NNetModel defaultModel;

typedef struct
{
    char name[64];
    uint32_t count;
} NNetHeader;


int nnet_data_read_entry( FILE *file, NNetHeader *header )
{
    uint32_t marker;
    if (fread(&marker, 4, 1, file) != 1) return 0;
    if (marker != 0x41544144) return 0;
    if (fread(header->name, sizeof(header->name), 1, file) != 1) return 0;
    if (feof(file)) return 0;
    if (fread(&header->count, sizeof(uint32_t), 1, file) != 1) return 0;
    if (feof(file)) return 0;
    return 1;
}


float *nnet_data_read_floats( FILE *file, const NNetHeader *header )
{
    float *buffer = NULL;
    uint32_t total = (uint32_t)(header->count * sizeof(float));
    if (posix_memalign((void**)&buffer, 32, total) != 0) return NULL;
    if (fread(buffer, total, 1, file) != 1)
    {
        free(buffer);
        return NULL;
    }
    printf("   Read %d entries from '%s'\n", header->count, header->name);
    return buffer;
}


int nnet_data_load( const char *fileName, NNetModel *model )
{
    NNetHeader header;
    FILE *file = NULL;

    memset(model, 0, sizeof(NNetModel));

    printf("Loading '%s'\n", fileName);

    file = fopen(fileName, "rb");
    if (file == NULL) return -1;

    while (nnet_data_read_entry(file, &header))
    {
        printf("Found '%s' (%d entries)\n", header.name, header.count);

        if (strcmp(header.name, "gru_a_embed_sig_weights") == 0)
            model->gru_a_embed_sig.embedding_weights = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "gru_a_embed_pred_weights") == 0)
            model->gru_a_embed_pred.embedding_weights = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "gru_a_embed_exc_weights") == 0)
            model->gru_a_embed_exc.embedding_weights = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "gru_a_dense_feature_weights") == 0)
            model->gru_a_dense_feature.input_weights = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "gru_a_dense_feature_bias") == 0)
            model->gru_a_dense_feature.bias = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "embed_pitch_weights") == 0)
            model->embed_pitch.embedding_weights = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "feature_conv1_weights") == 0)
            model->feature_conv1.input_weights = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "feature_conv1_bias") == 0)
            model->feature_conv1.bias = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "feature_conv2_weights") == 0)
            model->feature_conv2.input_weights = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "feature_conv2_bias") == 0)
            model->feature_conv2.bias = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "feature_dense1_weights") == 0)
            model->feature_dense1.input_weights = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "feature_dense1_bias") == 0)
            model->feature_dense1.bias = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "embed_sig_weights") == 0)
            model->embed_sig.embedding_weights = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "feature_dense2_weights") == 0)
            model->feature_dense2.input_weights = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "feature_dense2_bias") == 0)
            model->feature_dense2.bias = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "gru_a_weights") == 0)
            model->gru_a.input_weights = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "gru_a_recurrent_weights") == 0)
            model->gru_a.recurrent_weights = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "gru_a_bias") == 0)
            model->gru_a.bias = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "gru_b_weights") == 0)
            model->gru_b.input_weights = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "gru_b_recurrent_weights") == 0)
            model->gru_b.recurrent_weights = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "gru_b_bias") == 0)
            model->gru_b.bias = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "dual_fc_weights") == 0)
            model->dual_fc.input_weights = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "dual_fc_bias") == 0)
            model->dual_fc.bias = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "dual_fc_factor") == 0)
            model->dual_fc.factor = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "sparse_gru_a_recurrent_weights_diag") == 0)
            model->sparse_gru_a.diag_weights = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "sparse_gru_a_recurrent_weights") == 0)
            model->sparse_gru_a.recurrent_weights = nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "sparse_gru_a_recurrent_weights_idx") == 0)
            model->sparse_gru_a.idx = (int32_t*) nnet_data_read_floats(file, &header);
        else
        if (strcmp(header.name, "sparse_gru_a_bias") == 0)
            model->sparse_gru_a.bias = nnet_data_read_floats(file, &header);
        else
            fseek(file, (ssize_t)(header.count * sizeof(float)), SEEK_CUR);
    }

    model->gru_a_embed_sig.nb_inputs = 256;
    model->gru_a_embed_sig.dim = 1152;

    model->gru_a_embed_pred.nb_inputs = 256;
    model->gru_a_embed_pred.dim = 1152;

    model->gru_a_embed_exc.nb_inputs = 256;
    model->gru_a_embed_exc.dim = 1152;

    model->gru_a_dense_feature.nb_inputs = 128;
    model->gru_a_dense_feature.nb_neurons = 1152;
    model->gru_a_dense_feature.activation = ACTIVATION_LINEAR;

    model->embed_pitch.nb_inputs = 256;
    model->embed_pitch.dim = 64;

    model->embed_sig.nb_inputs = 256;
    model->embed_sig.dim = 128;

    model->feature_conv1.nb_inputs = 102;
    model->feature_conv1.kernel_size = 3;
    model->feature_conv1.nb_neurons = 128;
    model->feature_conv1.activation = ACTIVATION_TANH;

    model->feature_conv2.nb_inputs = 128;
    model->feature_conv2.kernel_size = 3;
    model->feature_conv2.nb_neurons = 128;
    model->feature_conv2.activation = ACTIVATION_TANH;

    model->feature_dense1.nb_inputs = 128;
    model->feature_dense1.nb_neurons = 128;
    model->feature_dense1.activation = ACTIVATION_TANH;

    model->feature_dense2.nb_inputs = 128;
    model->feature_dense2.nb_neurons = 128;
    model->feature_dense2.activation = ACTIVATION_TANH;

    model->gru_a.nb_inputs = 512;
    model->gru_a.nb_neurons = 384;
    model->gru_a.activation = ACTIVATION_TANH;
    model->gru_a.reset_after = 1;

    model->gru_b.nb_inputs = 512;
    model->gru_b.nb_neurons = 16;
    model->gru_b.activation = ACTIVATION_TANH;
    model->gru_b.reset_after = 1;

    model->dual_fc.nb_inputs = 16;
    model->dual_fc.nb_neurons = 256;
    model->dual_fc.nb_channels = 2;
    model->dual_fc.activation = ACTIVATION_SOFTMAX;

    model->sparse_gru_a.nb_neurons = 384;
    model->sparse_gru_a.activation = ACTIVATION_TANH;
    model->sparse_gru_a.reset_after = 1;

    fclose(file);
    return 0;
}
