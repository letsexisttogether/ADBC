#pragma once

#include <ASYS/String/StringLiteral.hpp>
#include <type_traits>

#include "Core/Query.hpp"

namespace ADBC 
{
    template <class ... _FieldNames>
    auto Select(_FieldNames&& ... fields) -> Query
    {

    }
};
