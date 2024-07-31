#pragma once

#ifndef LUISA_COMPUTE_DX_BACKEND_PCH_H
#define LUISA_COMPUTE_DX_BACKEND_PCH_H

#include <dxgi1_6.h>
#include <Windows.h>
#include <d3dx12.h>
#include <luisa/core/logging.h>
#include <luisa/core/basic_traits.h>
#include <luisa/core/basic_types.h>
#include <luisa/core/concepts.h>
#include <luisa/core/magic_enum.h>
#include <luisa/core/stl/vector.h>
#include <luisa/core/stl/unordered_map.h>
#include <luisa/core/stl/string.h>
#include <luisa/core/dynamic_module.h>
#include <luisa/core/stl/filesystem.h>
#include <luisa/runtime/buffer.h>
#include <luisa/runtime/image.h>
#include <luisa/runtime/shader.h>
#include <luisa/runtime/dispatch_buffer.h>
#include <luisa/vstl/common.h>
#include <luisa/vstl/functional.h>

#endif