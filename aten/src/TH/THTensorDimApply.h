#ifndef TH_TENSOR_DIM_APPLY_INC
#define TH_TENSOR_DIM_APPLY_INC

// This is an example of SIZE_CHECK argument passable to TH_TENSOR_DIM_APPLY3.
// The TENSOR1, TENSOR2, TENSOR3, DIMENSION will be expanded the same way as
// TH_TENSOR_DIM_APPLY3.
// Specifically, this check ensures that TENSOR1, TENSOR2, TENSOR3 have same
// size except for DIMENSION.
#define TH_TENSOR_DIM_APPLY3_SIZE_EQ_EXCEPT_DIM(TENSOR1, TENSOR2, TENSOR3, DIMENSION) \
{ \
  int shape_check_flag = 0;                                             \
  for(TH_TENSOR_DIM_APPLY_i = 0; TH_TENSOR_DIM_APPLY_i < TENSOR1->dim(); TH_TENSOR_DIM_APPLY_i++) \
  { \
    if (TH_TENSOR_DIM_APPLY_i == DIMENSION) \
      continue; \
    if (TENSOR1->size(TH_TENSOR_DIM_APPLY_i) != TENSOR2->size(TH_TENSOR_DIM_APPLY_i)) { \
      shape_check_flag = 1; \
      break; \
    } \
    if(TENSOR1->size(TH_TENSOR_DIM_APPLY_i) != TENSOR3->size(TH_TENSOR_DIM_APPLY_i)) { \
      shape_check_flag = 1; \
      break; \
    } \
  } \
  if (shape_check_flag == 1) { \
    AT_ERROR("Expected ", #TENSOR1, " ", TENSOR1->sizes(), ", ", #TENSOR2, " ", TENSOR2->sizes(), " and ", #TENSOR3, " ", TENSOR3->sizes(), " to have the same size apart from dimension ", DIMENSION); \
  } \
}

#define TH_TENSOR_DIM_APPLY3(TYPE1, TENSOR1, TYPE2, TENSOR2, TYPE3, TENSOR3, DIMENSION, SIZE_CHECK, CODE) \
{ \
  TYPE1 *TENSOR1##_data = NULL; \
  TH_UNUSED int64_t TENSOR1##_stride = 0, TENSOR1##_size = 0; \
  TYPE2 *TENSOR2##_data = NULL; \
  TH_UNUSED int64_t TENSOR2##_stride = 0, TENSOR2##_size = 0; \
  TYPE3 *TENSOR3##_data = NULL; \
  TH_UNUSED int64_t TENSOR3##_stride = 0, TENSOR3##_size = 0; \
  int64_t *TH_TENSOR_DIM_APPLY_counter = NULL; \
  int TH_TENSOR_DIM_APPLY_hasFinished = THTensor_(numel)(TENSOR1) == 0; \
  int TH_TENSOR_DIM_APPLY_i; \
\
  if( (DIMENSION < 0) || (DIMENSION >= TENSOR1->dim()) ) \
    THError("invalid dimension %d (expected to be 0 <= dim < %d)", DIMENSION, TENSOR1->dim()); \
  int same_dims = 1;                                                    \
  if( TENSOR1->dim() != TENSOR2->dim() ) {                    \
    same_dims = 0;                                                      \
  } \
  if( TENSOR1->dim() != TENSOR3->dim() ) { \
    same_dims = 0;                                   \
  } \
  if (same_dims == 0) { \
    AT_ERROR("inconsistent tensor size, expected ", #TENSOR1, " ", TENSOR1->sizes(), ", ", #TENSOR2, " ", TENSOR2->sizes(), " and ", #TENSOR3, " ",TENSOR3->sizes() , " to have the same number of dimensions"); \
  }                                                                     \
  SIZE_CHECK(TENSOR1, TENSOR2, TENSOR3, DIMENSION)                      \
\
  if (TH_TENSOR_DIM_APPLY_hasFinished) { \
    return; \
  } \
  TH_TENSOR_DIM_APPLY_counter = (int64_t*)THAlloc(sizeof(int64_t)*(TENSOR1->dim())); \
  for(TH_TENSOR_DIM_APPLY_i = 0; TH_TENSOR_DIM_APPLY_i < TENSOR1->dim(); TH_TENSOR_DIM_APPLY_i++) \
    TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i] = 0; \
\
  TENSOR1##_data = THTensor_getStoragePtr(TENSOR1)->data<TYPE1>()+(TENSOR1)->storage_offset(); \
  TENSOR1##_stride = (TENSOR1)->stride(DIMENSION); \
  TENSOR1##_size = TENSOR1->size(DIMENSION); \
\
  TENSOR2##_data = THTensor_getStoragePtr(TENSOR2)->data<TYPE2>()+(TENSOR2)->storage_offset(); \
  TENSOR2##_stride = (TENSOR2)->stride(DIMENSION); \
  TENSOR2##_size = TENSOR2->size(DIMENSION); \
\
  TENSOR3##_data = THTensor_getStoragePtr(TENSOR3)->data<TYPE3>()+(TENSOR3)->storage_offset(); \
  TENSOR3##_stride = (TENSOR3)->stride(DIMENSION); \
  TENSOR3##_size = TENSOR3->size(DIMENSION); \
\
  while(!TH_TENSOR_DIM_APPLY_hasFinished) \
  { \
    CODE \
\
    if(TENSOR1->dim() == 1) \
       break; \
 \
    for(TH_TENSOR_DIM_APPLY_i = 0; TH_TENSOR_DIM_APPLY_i < TENSOR1->dim(); TH_TENSOR_DIM_APPLY_i++) \
    { \
      if(TH_TENSOR_DIM_APPLY_i == DIMENSION) \
      { \
        if(TH_TENSOR_DIM_APPLY_i == TENSOR1->dim()-1) \
        { \
          TH_TENSOR_DIM_APPLY_hasFinished = 1; \
          break; \
        } \
        continue; \
      } \
\
      TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i]++; \
      TENSOR1##_data += TENSOR1->stride(TH_TENSOR_DIM_APPLY_i); \
      TENSOR2##_data += TENSOR2->stride(TH_TENSOR_DIM_APPLY_i); \
      TENSOR3##_data += TENSOR3->stride(TH_TENSOR_DIM_APPLY_i); \
\
      if(TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i] == TENSOR1->size(TH_TENSOR_DIM_APPLY_i)) \
      { \
        if(TH_TENSOR_DIM_APPLY_i == TENSOR1->dim()-1) \
        { \
          TH_TENSOR_DIM_APPLY_hasFinished = 1; \
          break; \
        } \
        else \
        { \
          TENSOR1##_data -= TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i]*TENSOR1->stride(TH_TENSOR_DIM_APPLY_i); \
          TENSOR2##_data -= TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i]*TENSOR2->stride(TH_TENSOR_DIM_APPLY_i); \
          TENSOR3##_data -= TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i]*TENSOR3->stride(TH_TENSOR_DIM_APPLY_i); \
          TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i] = 0; \
        } \
      } \
      else \
        break; \
    } \
  } \
  THFree(TH_TENSOR_DIM_APPLY_counter); \
}

/**
 * Similar to DIM_APPLY(...) but we maintain two sets of pointers: one for the first tensor
 * and one for the second. The two tensors must have the same shape, other than at the
 * specified DIMENSION. This function makes it easy to store the output from reducing the
 * TENSOR at index. For example, in the sum example described below, we could instead do:
 *
 * int64_t i = 0;
 * TYPE1 sum;
 *
 * for (i = 0; i < TENSOR1##_size; ++i) {
 *   sum += TENSOR1##_data[i * TENSOR1##_stride]
 * }
 * *TENSOR2##_data = (TYPE2) sum;
 *
 * In particular, we guarantee that the offset into TENSOR2 will be what you would get if
 * you applied all of the index values used to generate the offset into TENSOR1.
 */
#define TH_TENSOR_DIM_APPLY2(TYPE1, TENSOR1, TYPE2, TENSOR2, DIMENSION, CODE) \
{ \
  TYPE1 *TENSOR1##_data = NULL; \
  TH_UNUSED int64_t TENSOR1##_stride = 0, TENSOR1##_size = 0; \
  TYPE2 *TENSOR2##_data = NULL; \
  TH_UNUSED int64_t TENSOR2##_stride = 0, TENSOR2##_size = 0; \
  int64_t *TH_TENSOR_DIM_APPLY_counter = NULL; \
  int TH_TENSOR_DIM_APPLY_hasFinished = THTensor_(numel)(TENSOR1) == 0; \
  int TH_TENSOR_DIM_APPLY_i; \
\
  if( (DIMENSION < 0) || (DIMENSION >= TENSOR1->dim()) ) \
    THError("invalid dimension %d (expected to be 0 <= dim < %d)", DIMENSION, THTensor_nDimensionLegacyAll(TENSOR1)); \
  if( TENSOR1->dim() != TENSOR2->dim() ) {                    \
    AT_ERROR("inconsistent tensor size, expected ", #TENSOR1, " ", TENSOR1->sizes(), " and ", #TENSOR2, " ", TENSOR2->sizes(), " to have the same number of dimensions");        \
  }                                                                     \
  TH_UNUSED int shape_check_flag = 0;                                             \
  for(TH_TENSOR_DIM_APPLY_i = 0; TH_TENSOR_DIM_APPLY_i < TENSOR1->dim(); TH_TENSOR_DIM_APPLY_i++) \
  { \
    if(TH_TENSOR_DIM_APPLY_i == DIMENSION) \
      continue; \
    if(TENSOR1->size(TH_TENSOR_DIM_APPLY_i) != TENSOR2->size(TH_TENSOR_DIM_APPLY_i)) { \
      AT_ERROR("Expected ", #TENSOR1, " ", TENSOR1->sizes(), " and ", #TENSOR2, " ", TENSOR2->sizes(), " to have the same size in dimension ", DIMENSION); \
    }                                                                   \
  } \
\
  if (TH_TENSOR_DIM_APPLY_hasFinished) { \
    return; \
  } \
  TH_TENSOR_DIM_APPLY_counter = (int64_t*)THAlloc(sizeof(int64_t)*(TENSOR1->dim())); \
  for(TH_TENSOR_DIM_APPLY_i = 0; TH_TENSOR_DIM_APPLY_i < TENSOR1->dim(); TH_TENSOR_DIM_APPLY_i++) \
    TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i] = 0; \
\
  TENSOR1##_data = THTensor_getStoragePtr(TENSOR1)->data<TYPE1>()+(TENSOR1)->storage_offset(); \
  TENSOR1##_stride = (TENSOR1)->stride(DIMENSION); \
  TENSOR1##_size = TENSOR1->size(DIMENSION); \
\
  TENSOR2##_data = THTensor_getStoragePtr(TENSOR2)->data<TYPE2>()+(TENSOR2)->storage_offset(); \
  TENSOR2##_stride = (TENSOR2)->stride(DIMENSION); \
  TENSOR2##_size = TENSOR2->size(DIMENSION); \
\
  while(!TH_TENSOR_DIM_APPLY_hasFinished) \
  { \
    CODE \
\
    if(TENSOR1->dim() == 1) \
       break; \
 \
    for(TH_TENSOR_DIM_APPLY_i = 0; TH_TENSOR_DIM_APPLY_i < TENSOR1->dim(); TH_TENSOR_DIM_APPLY_i++) \
    { \
      if(TH_TENSOR_DIM_APPLY_i == DIMENSION) \
      { \
        if(TH_TENSOR_DIM_APPLY_i == TENSOR1->dim()-1) \
        { \
          TH_TENSOR_DIM_APPLY_hasFinished = 1; \
          break; \
        } \
        continue; \
      } \
\
      TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i]++; \
      TENSOR1##_data += TENSOR1->stride(TH_TENSOR_DIM_APPLY_i); \
      TENSOR2##_data += TENSOR2->stride(TH_TENSOR_DIM_APPLY_i); \
\
      if(TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i] == TENSOR1->size(TH_TENSOR_DIM_APPLY_i)) \
      { \
        if(TH_TENSOR_DIM_APPLY_i == TENSOR1->dim()-1) \
        { \
          TH_TENSOR_DIM_APPLY_hasFinished = 1; \
          break; \
        } \
        else \
        { \
          TENSOR1##_data -= TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i]*TENSOR1->stride(TH_TENSOR_DIM_APPLY_i); \
          TENSOR2##_data -= TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i]*TENSOR2->stride(TH_TENSOR_DIM_APPLY_i); \
          TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i] = 0; \
        } \
      } \
      else \
        break; \
    } \
  } \
  THFree(TH_TENSOR_DIM_APPLY_counter); \
}

/**
 * The basic idea for DIM_APPLY: Given a TENSOR and a DIMENSION, provide access to the data stored
 * at all sets of dimension values other than DIMENSION, such that we can get all the values at those
 * fixed indices for the various values at DIMENSION.
 *
 * Suppose we have a 2x3x4 Tensor A, and we have DIMENSION=2. Then we will hit CODE (2x3) times, and the
 * pointer into storage will be at:
 *
 * A[0][0]
 * A[0][1]
 * A[0][2]
 * A[1][0]
 * A[1][1]
 * A[1][2]
 *
 * And at each point, we can access the data for each of the four elements of the Tensor via
 * TENSOR##_stride. So for example, if we wanted to sum the elements there, we could do:
 *
 * int64_t i = 0;
 * TYPE sum;
 * for (i = 0; i < TENSOR##_size; i++) {
 *  sum += TENSOR##_data[i * TENSOR##_stride]
 * }
 *
 * Note that we don't have to have DIMENSION be the last tensor. If we have DIMENSION=1, then we will hit the
 * code (2x4) times, with pointer into the storage at:
 *
 * offset +
 *   stride_0 * 0 + stride_2 * 0
 *   stride_0 * 1 + stride_2 * 0
 *   stride_0 * 0 + stride_2 * 1
 *   stride_0 * 1 + stride_2 * 1
 *   stride_0 * 0 + stride_2 * 2
 *   stride_0 * 1 + stride_2 * 2
 *   stride_0 * 0 + stride_2 * 3
 *   stride_0 * 1 + stride_2 * 3
 *
 * So we can again sum over the values at DIMENSION with the other indices fixed.
 */
#define TH_TENSOR_DIM_APPLY(TYPE, TENSOR, DIMENSION, CODE) \
{ \
  TYPE *TENSOR##_data = NULL; \
  int64_t TENSOR##_stride = 0, TENSOR##_size = 0; \
  int64_t *TH_TENSOR_DIM_APPLY_counter = NULL; \
  int TH_TENSOR_DIM_APPLY_hasFinished = 0; \
  int TH_TENSOR_DIM_APPLY_i; \
\
  if( (DIMENSION < 0) || (DIMENSION >= THTensor_nDimensionLegacyAll(TENSOR)) ) \
    THError("invalid dimension"); \
\
  TENSOR##_data = THTensor_getStoragePtr(TENSOR)->data<TYPE>()+(TENSOR)->storage_offset(); \
  TENSOR##_stride = (TENSOR)->stride(DIMENSION); \
  TENSOR##_size = TENSOR->size(DIMENSION); \
  /* Counter stores the indices into the Tensor at any time */ \
  TH_TENSOR_DIM_APPLY_counter = (int64_t*)THAlloc(sizeof(int64_t)*(THTensor_nDimensionLegacyAll(TENSOR))); \
  for(TH_TENSOR_DIM_APPLY_i = 0; TH_TENSOR_DIM_APPLY_i < THTensor_nDimensionLegacyAll(TENSOR); TH_TENSOR_DIM_APPLY_i++) \
    TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i] = 0; \
\
  while(!TH_TENSOR_DIM_APPLY_hasFinished) \
  { \
    CODE \
\
    if(THTensor_nDimensionLegacyAll(TENSOR) == 1) \
       break; \
 \
    for(TH_TENSOR_DIM_APPLY_i = 0; TH_TENSOR_DIM_APPLY_i < THTensor_nDimensionLegacyAll(TENSOR); TH_TENSOR_DIM_APPLY_i++) \
    { \
       /* Check if the index is equal to DIMENSION. We don't need to update the */ \
       /* offset if this is the case, and can consider the next index. However, */ \
       /* in the case that the DIMENSION is the last index in the Tensor, then */ \
       /* we have parsed the entire tensor and can exit */ \
      if(TH_TENSOR_DIM_APPLY_i == DIMENSION) \
      { \
        if(TH_TENSOR_DIM_APPLY_i == THTensor_nDimensionLegacyAll(TENSOR)-1) \
        { \
          TH_TENSOR_DIM_APPLY_hasFinished = 1; \
          break; \
        } \
        continue; \
      } \
\
      /* Bump the counter at this index, update the pointer */ \
      TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i]++; \
      TENSOR##_data += TENSOR->stride(TH_TENSOR_DIM_APPLY_i); \
\
      if(TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i] == TENSOR->size(TH_TENSOR_DIM_APPLY_i)) \
      { \
        /* Handled TENSOR_size(dim) iterations for DIM_APPLY_i. If this is the last dimension, exit */ \
        if(TH_TENSOR_DIM_APPLY_i == THTensor_nDimensionLegacyAll(TENSOR)-1) \
        { \
          TH_TENSOR_DIM_APPLY_hasFinished = 1; \
          break; \
        } \
        else \
        { \
          /* Reset the counter, and the pointer to the beginning of the storage for this combination of indices */ \
          TENSOR##_data -= TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i]*TENSOR->stride(TH_TENSOR_DIM_APPLY_i); \
          TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i] = 0; \
        } \
      } \
      else \
        break; \
    } \
  } \
  THFree(TH_TENSOR_DIM_APPLY_counter); \
}

#endif
