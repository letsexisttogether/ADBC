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

    template <ColumnType _Column, class _Param>
    auto NotEquals(_Column&& column, _Param&& param)
    {
        using ParamType = std::remove_cvref_t<_Param>;

        return Condition<std::tuple<ParamType>>
        {
            std::string{ std::remove_cvref_t<_Column>::Name } + " != ?",
            std::tuple<ParamType>
            {
                std::forward<_Param>(param)
            }
        };
    }
    
    template <ColumnType _Column, class ... _Params>
    auto Between(_Column&& column, _Params&& ... param)
        requires (sizeof ... (_Params) == 2)
    {
        return Condition<std::tuple<_Params...>>
        {
            std::string{ std::remove_cvref_t<_Column>::Name } + " BETWEEN ? AND ?",
            std::tuple<std::tuple<_Params...>>
            {
                std::forward<_Params>(param)...
            }
        };
    }
};
