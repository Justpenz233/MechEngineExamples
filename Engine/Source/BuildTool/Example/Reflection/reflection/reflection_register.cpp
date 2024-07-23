#include <assert.h>

#include "../json.h"
#include "reflection.h"
#include "Reflection/reflection/reflection_register.h"
#include "Reflection/serializer/serializer.h"

#include "_generated/reflection/all_reflection.h"
#include "_generated/serializer/all_serializer.ipp"

namespace Reflection
{
    void TypeMetaRegister::metaUnregister() { TypeMetaRegisterinterface::unregisterAll(); }
} // namespace Reflection