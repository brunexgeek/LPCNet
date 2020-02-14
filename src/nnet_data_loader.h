#ifndef NNET_DATA_LOADER_H
#define NNET_DATA_LOADER_H


#include "nnet.h"

#define GRU_A_EMBED_SIG_OUT_SIZE 1152
//extern const EmbeddingLayer gru_a_embed_sig;

#define GRU_A_EMBED_PRED_OUT_SIZE 1152
//extern const EmbeddingLayer gru_a_embed_pred;

#define GRU_A_EMBED_EXC_OUT_SIZE 1152
//extern const EmbeddingLayer gru_a_embed_exc;

#define GRU_A_DENSE_FEATURE_OUT_SIZE 1152
//extern const DenseLayer gru_a_dense_feature;

#define EMBED_PITCH_OUT_SIZE 64
//extern const EmbeddingLayer embed_pitch;

#define FEATURE_CONV1_OUT_SIZE 128
#define FEATURE_CONV1_STATE_SIZE (102*2)
#define FEATURE_CONV1_DELAY 1
//extern const Conv1DLayer feature_conv1;

#define FEATURE_CONV2_OUT_SIZE 128
#define FEATURE_CONV2_STATE_SIZE (128*2)
#define FEATURE_CONV2_DELAY 1
//extern const Conv1DLayer feature_conv2;

#define FEATURE_DENSE1_OUT_SIZE 128
//extern const DenseLayer feature_dense1;

#define EMBED_SIG_OUT_SIZE 128
//extern const EmbeddingLayer embed_sig;

#define FEATURE_DENSE2_OUT_SIZE 128
//extern const DenseLayer feature_dense2;

#define GRU_A_OUT_SIZE 384
#define GRU_A_STATE_SIZE 384
//extern const GRULayer gru_a;

#define GRU_B_OUT_SIZE 16
#define GRU_B_STATE_SIZE 16
//extern const GRULayer gru_b;

#define DUAL_FC_OUT_SIZE 256
//extern const MDenseLayer dual_fc;

#define SPARSE_GRU_A_OUT_SIZE 384
#define SPARSE_GRU_A_STATE_SIZE 384
//extern const SparseGRULayer sparse_gru_a;

#define MAX_RNN_NEURONS 384
#define MAX_CONV_INPUTS 384
#define MAX_MDENSE_TMP 512

typedef struct  {
    EmbeddingLayer *gru_a_embed_sig;
    EmbeddingLayer *gru_a_embed_pred;
    EmbeddingLayer *gru_a_embed_exc;
    DenseLayer *gru_a_dense_feature;
    EmbeddingLayer *embed_pitch;
    Conv1DLayer *feature_conv1;
    Conv1DLayer *feature_conv2;
    DenseLayer *feature_dense1;
    EmbeddingLayer *embed_sig;
    DenseLayer *feature_dense2;
    GRULayer *gru_a;
    GRULayer *gru_b;
    MDenseLayer *dual_fc;
    SparseGRULayer *sparse_gru_a;
} NNetModel;

typedef struct {
  float feature_conv1_state[FEATURE_CONV1_STATE_SIZE];
  float feature_conv2_state[FEATURE_CONV2_STATE_SIZE];
  float gru_a_state[GRU_A_STATE_SIZE];
  float gru_b_state[GRU_B_STATE_SIZE];
} NNetState;

extern NNetModel defaultModel;

const NNetModel *nnet_get_model();

int nnet_data_load( const char *fileName, NNetModel *model );

#endif // NNET_DATA_LOADER_H
