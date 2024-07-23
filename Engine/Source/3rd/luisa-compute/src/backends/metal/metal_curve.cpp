#include <luisa/core/logging.h>
#include <luisa/runtime/rhi/curve_basis.h>
#include "metal_command_encoder.h"
#include "metal_buffer.h"
#include "metal_curve.h"

namespace luisa::compute::metal {

void MetalCurve::_do_add_resources(luisa::vector<MTL::Resource *> &resources) const noexcept {
    LUISA_ASSERT(_descriptor != nullptr, "Curve not built.");
    auto curve_desc = _descriptor->geometryDescriptors()
                          ->object<MTL::AccelerationStructureCurveGeometryDescriptor>(0u);
    resources.emplace_back(curve_desc->controlPointBuffer());
    resources.emplace_back(curve_desc->radiusBuffer());
    resources.emplace_back(curve_desc->indexBuffer());
}

MetalCurve::MetalCurve(MTL::Device *device, const AccelOption &option) noexcept
    : MetalPrimitive{device, option} {}

MetalCurve::~MetalCurve() noexcept {
    if (_descriptor) { _descriptor->release(); }
}

void MetalCurve::build(MetalCommandEncoder &encoder, CurveBuildCommand *command) noexcept {
    std::scoped_lock lock{mutex()};

    auto cp_count = command->cp_count();
    auto seg_count = command->seg_count();

    auto cp_buffer = reinterpret_cast<MetalBuffer *>(command->cp_buffer());
    auto cp_buffer_handle = cp_buffer->handle();
    auto cp_buffer_offset = command->cp_buffer_offset();
    auto cp_stride = command->cp_stride();
    LUISA_ASSERT(cp_stride >= sizeof(float4) &&
                     cp_buffer_offset + cp_count * cp_stride <=
                         cp_buffer_handle->length(),
                 "Invalid control point buffer size.");

    auto radius_buffer_handle = cp_buffer->handle();
    auto radius_buffer_offset = cp_buffer_offset + sizeof(float) * 3u;
    auto radius_stride = command->cp_stride();

    auto seg_buffer = reinterpret_cast<MetalBuffer *>(command->seg_buffer());
    auto seg_buffer_handle = seg_buffer->handle();
    auto seg_buffer_offset = command->seg_buffer_offset();

    LUISA_ASSERT(seg_buffer_offset + seg_count * sizeof(uint) <=
                     seg_buffer_handle->length(),
                 "Invalid segment buffer size.");

    auto [basis, end_cap] = [basis = command->basis()] {
        switch (basis) {
            case CurveBasis::PIECEWISE_LINEAR: return std::make_pair(MTL::CurveBasisLinear, MTL::CurveEndCapsSphere);
            case CurveBasis::CUBIC_BSPLINE: return std::make_pair(MTL::CurveBasisBSpline, MTL::CurveEndCapsNone);
            case CurveBasis::CATMULL_ROM: return std::make_pair(MTL::CurveBasisCatmullRom, MTL::CurveEndCapsNone);
            case CurveBasis::BEZIER: return std::make_pair(MTL::CurveBasisBezier, MTL::CurveEndCapsNone);
            default: break;
        }
        LUISA_ERROR_WITH_LOCATION("Invalid curve basis.");
    }();
    auto cp_per_seg = segment_control_point_count(command->basis());

    auto geometry_buffer_changed = [&, basis = basis, end_cap = end_cap](auto desc) noexcept {
        return desc->curveBasis() != basis ||
               desc->segmentControlPointCount() != cp_per_seg ||
               desc->controlPointCount() != cp_count ||
               desc->segmentCount() != seg_count ||
               desc->controlPointBuffer() != cp_buffer_handle ||
               desc->controlPointBufferOffset() != cp_buffer_offset ||
               desc->controlPointStride() != cp_stride ||
               desc->radiusBuffer() != radius_buffer_handle ||
               desc->radiusBufferOffset() != radius_buffer_offset ||
               desc->radiusStride() != radius_stride ||
               desc->indexBuffer() != seg_buffer_handle ||
               desc->indexBufferOffset() != seg_buffer_offset ||
               desc->curveEndCaps() != end_cap ||
               desc->curveType() != MTL::CurveTypeRound;
    };

    using GeometryDescriptor = MTL::AccelerationStructureCurveGeometryDescriptor;
    auto requires_build = handle() == nullptr ||
                          !option().allow_update ||
                          command->request() == AccelBuildRequest::FORCE_BUILD ||
                          _descriptor == nullptr ||
                          geometry_buffer_changed(_descriptor->geometryDescriptors()
                                                      ->object<GeometryDescriptor>(0u));

    if (requires_build) {
        if (_descriptor) { _descriptor->release(); }
        auto geom_desc = GeometryDescriptor::descriptor();
        geom_desc->setControlPointBuffer(cp_buffer_handle);
        geom_desc->setControlPointBufferOffset(cp_buffer_offset);
        geom_desc->setControlPointStride(cp_stride);
        geom_desc->setControlPointCount(cp_count);
        geom_desc->setControlPointFormat(MTL::AttributeFormatFloat3);
        geom_desc->setRadiusBuffer(radius_buffer_handle);
        geom_desc->setRadiusBufferOffset(radius_buffer_offset);
        geom_desc->setRadiusStride(radius_stride);
        geom_desc->setRadiusFormat(MTL::AttributeFormatFloat);
        geom_desc->setIndexBuffer(seg_buffer_handle);
        geom_desc->setIndexBufferOffset(seg_buffer_offset);
        geom_desc->setIndexType(MTL::IndexTypeUInt32);
        geom_desc->setSegmentCount(seg_count);
        geom_desc->setSegmentControlPointCount(cp_per_seg);
        geom_desc->setCurveType(MTL::CurveTypeRound);
        geom_desc->setCurveBasis(basis);
        geom_desc->setCurveEndCaps(end_cap);
        geom_desc->setOpaque(true);
        geom_desc->setAllowDuplicateIntersectionFunctionInvocation(true);
        geom_desc->setIntersectionFunctionTableOffset(0u);
        auto geom_desc_object = static_cast<NS::Object *>(geom_desc);
        auto geom_desc_array = NS::Array::array(&geom_desc_object, 1u);
        _descriptor = MTL::PrimitiveAccelerationStructureDescriptor::alloc()->init();
        _descriptor->setGeometryDescriptors(geom_desc_array);
        _descriptor->setUsage(usage());
        _do_build(encoder, _descriptor);
    } else {
        _do_update(encoder, _descriptor);
    }
}

}// namespace luisa::compute::metal
