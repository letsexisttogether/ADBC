#pragma once

#include <ASYS/String/StringLiteral.hpp>

namespace ADBC
{
    template <ASYS::StringLiteral _Name, class _Type>
    struct Column
    {
        constexpr static auto Name = _Name;
        using Type = _Type;
    };


};
