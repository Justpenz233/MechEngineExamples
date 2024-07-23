 // This file is generated by device_run_length_encode.py
#include "device_run_length_encode.h"
#include "dcub_utils.cuh"
#include <cub/device/device_run_length_encode.cuh>

namespace luisa::compute::cuda::dcub {
// DOC:  https://nvlabs.github.io/cub/structcub_1_1_device_run_length_encode.html
cudaError_t DeviceRunLengthEncode::Encode(void* d_temp_storage, size_t& temp_storage_bytes, const int32_t*  d_in, int32_t*  d_unique_out, int32_t*  d_counts_out, int32_t*  d_num_runs_out, int  num_items, cudaStream_t stream)
{
    return ::cub::DeviceRunLengthEncode::Encode(d_temp_storage, temp_storage_bytes, d_in, d_unique_out, d_counts_out, d_num_runs_out, num_items, stream);
}

cudaError_t DeviceRunLengthEncode::Encode(void* d_temp_storage, size_t& temp_storage_bytes, const uint32_t*  d_in, uint32_t*  d_unique_out, int32_t*  d_counts_out, int32_t*  d_num_runs_out, int  num_items, cudaStream_t stream)
{
    return ::cub::DeviceRunLengthEncode::Encode(d_temp_storage, temp_storage_bytes, d_in, d_unique_out, d_counts_out, d_num_runs_out, num_items, stream);
}

cudaError_t DeviceRunLengthEncode::Encode(void* d_temp_storage, size_t& temp_storage_bytes, const int64_t*  d_in, int64_t*  d_unique_out, int32_t*  d_counts_out, int32_t*  d_num_runs_out, int  num_items, cudaStream_t stream)
{
    return ::cub::DeviceRunLengthEncode::Encode(d_temp_storage, temp_storage_bytes, d_in, d_unique_out, d_counts_out, d_num_runs_out, num_items, stream);
}

cudaError_t DeviceRunLengthEncode::Encode(void* d_temp_storage, size_t& temp_storage_bytes, const uint64_t*  d_in, uint64_t*  d_unique_out, int32_t*  d_counts_out, int32_t*  d_num_runs_out, int  num_items, cudaStream_t stream)
{
    return ::cub::DeviceRunLengthEncode::Encode(d_temp_storage, temp_storage_bytes, d_in, d_unique_out, d_counts_out, d_num_runs_out, num_items, stream);
}

cudaError_t DeviceRunLengthEncode::NonTrivialRuns(void* d_temp_storage, size_t& temp_storage_bytes, const int32_t*  d_in, int32_t*  d_offsets_out, int32_t*  d_lengths_out, int32_t*  d_num_runs_out, int  num_items, cudaStream_t stream)
{
    return ::cub::DeviceRunLengthEncode::NonTrivialRuns(d_temp_storage, temp_storage_bytes, d_in, d_offsets_out, d_lengths_out, d_num_runs_out, num_items, stream);
}

cudaError_t DeviceRunLengthEncode::NonTrivialRuns(void* d_temp_storage, size_t& temp_storage_bytes, const uint32_t*  d_in, int32_t*  d_offsets_out, int32_t*  d_lengths_out, int32_t*  d_num_runs_out, int  num_items, cudaStream_t stream)
{
    return ::cub::DeviceRunLengthEncode::NonTrivialRuns(d_temp_storage, temp_storage_bytes, d_in, d_offsets_out, d_lengths_out, d_num_runs_out, num_items, stream);
}

cudaError_t DeviceRunLengthEncode::NonTrivialRuns(void* d_temp_storage, size_t& temp_storage_bytes, const int64_t*  d_in, int32_t*  d_offsets_out, int32_t*  d_lengths_out, int32_t*  d_num_runs_out, int  num_items, cudaStream_t stream)
{
    return ::cub::DeviceRunLengthEncode::NonTrivialRuns(d_temp_storage, temp_storage_bytes, d_in, d_offsets_out, d_lengths_out, d_num_runs_out, num_items, stream);
}

cudaError_t DeviceRunLengthEncode::NonTrivialRuns(void* d_temp_storage, size_t& temp_storage_bytes, const uint64_t*  d_in, int32_t*  d_offsets_out, int32_t*  d_lengths_out, int32_t*  d_num_runs_out, int  num_items, cudaStream_t stream)
{
    return ::cub::DeviceRunLengthEncode::NonTrivialRuns(d_temp_storage, temp_storage_bytes, d_in, d_offsets_out, d_lengths_out, d_num_runs_out, num_items, stream);
}
}
