// This file is generated by device_partition.py
#pragma once
#include <luisa/core/dll_export.h>// for LC_BACKEND_API
#include <luisa/backends/ext/cuda/lcub/dcub/dcub_common.h>
#include <luisa/backends/ext/cuda/lcub/cuda_lcub_command.h>

namespace luisa::compute::cuda::lcub {
// DOC:  https://nvlabs.github.io/cub/structcub_1_1_device_partition.html
class LC_BACKEND_API DevicePartition {
    template<typename T>
    using BufferView = luisa::compute::BufferView<T>;
    using UCommand = luisa::unique_ptr<luisa::compute::cuda::CudaLCubCommand>;
public:

    static void Flagged(size_t &temp_storage_size, BufferView<int32_t> d_in, BufferView<int32_t> d_flags, BufferView<int32_t> d_out, BufferView<int32_t> d_num_selected_out, int num_items) noexcept;
    static UCommand Flagged(BufferView<int> d_temp_storage, BufferView<int32_t> d_in, BufferView<int32_t> d_flags, BufferView<int32_t> d_out, BufferView<int32_t> d_num_selected_out, int num_items) noexcept;

    static void Flagged(size_t &temp_storage_size, BufferView<uint32_t> d_in, BufferView<int32_t> d_flags, BufferView<uint32_t> d_out, BufferView<int32_t> d_num_selected_out, int num_items) noexcept;
    static UCommand Flagged(BufferView<int> d_temp_storage, BufferView<uint32_t> d_in, BufferView<int32_t> d_flags, BufferView<uint32_t> d_out, BufferView<int32_t> d_num_selected_out, int num_items) noexcept;

    static void Flagged(size_t &temp_storage_size, BufferView<int64_t> d_in, BufferView<int32_t> d_flags, BufferView<int64_t> d_out, BufferView<int32_t> d_num_selected_out, int num_items) noexcept;
    static UCommand Flagged(BufferView<int> d_temp_storage, BufferView<int64_t> d_in, BufferView<int32_t> d_flags, BufferView<int64_t> d_out, BufferView<int32_t> d_num_selected_out, int num_items) noexcept;

    static void Flagged(size_t &temp_storage_size, BufferView<uint64_t> d_in, BufferView<int32_t> d_flags, BufferView<uint64_t> d_out, BufferView<int32_t> d_num_selected_out, int num_items) noexcept;
    static UCommand Flagged(BufferView<int> d_temp_storage, BufferView<uint64_t> d_in, BufferView<int32_t> d_flags, BufferView<uint64_t> d_out, BufferView<int32_t> d_num_selected_out, int num_items) noexcept;

    static void Flagged(size_t &temp_storage_size, BufferView<float> d_in, BufferView<int32_t> d_flags, BufferView<float> d_out, BufferView<int32_t> d_num_selected_out, int num_items) noexcept;
    static UCommand Flagged(BufferView<int> d_temp_storage, BufferView<float> d_in, BufferView<int32_t> d_flags, BufferView<float> d_out, BufferView<int32_t> d_num_selected_out, int num_items) noexcept;

    static void Flagged(size_t &temp_storage_size, BufferView<double> d_in, BufferView<int32_t> d_flags, BufferView<double> d_out, BufferView<int32_t> d_num_selected_out, int num_items) noexcept;
    static UCommand Flagged(BufferView<int> d_temp_storage, BufferView<double> d_in, BufferView<int32_t> d_flags, BufferView<double> d_out, BufferView<int32_t> d_num_selected_out, int num_items) noexcept;
};
}// namespace luisa::compute::cuda::lcub