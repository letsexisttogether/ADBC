#pragma once

#include <string>
#include <type_traits>

#include "Schema/Column.hpp"

namespace ADBC::OPS
{
    template <class _QueryParamPack>
    struct Condition
    {
        std::string Text{};
        _QueryParamPack Params{};
    };

    template <ColumnType _Column, class _Param>
    auto Equals(_Column&& column, _Param&& param)
    {
        using ParamType = std::remove_cvref_t<_Param>;

        return Condition<std::tuple<ParamType>>
        {
            std::string{ std::remove_cvref_t<_Column>::Name } + " = ?",
            std::tuple<ParamType>
            {
                std::forward<_Param>(param)
            }
        };
    }
};
