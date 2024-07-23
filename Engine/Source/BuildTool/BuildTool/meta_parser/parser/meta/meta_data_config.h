#pragma once

namespace NativeProperty
{
    const auto MCLASS = "MCLASS";

	const auto MSTRUCT = "MSTRUCT";

    const auto MPROPERTY = "MPROPERTY";

    const auto MFUNCTION = "MFUNCTION";

	const auto MENUM = "MENUM";

	inline std::set<std::string> MTags = { MCLASS, MSTRUCT, MPROPERTY, MFUNCTION, MENUM };

} // namespace NativeProperty
