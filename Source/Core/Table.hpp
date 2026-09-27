#pragma once 

#include <ASYS/String/StringLiteral.hpp>
#include <type_traits>

namespace ADBC
{
    template <ASYS::StringLiteral _Name, class _Type, bool _IsValueEmbedded>
    struct Column
    {
        using ValueType = _Type;
        using StorageType = std::conditional_t<_IsValueEmbedded,
            _Type, _Type&>;

        constexpr explicit Column(StorageType value) : Value{ value } {}

        static constexpr auto Name = _Name;

        StorageType Value;
    };

    template <ASYS::StringLiteral _Name, class _Type>
    constexpr auto Col(_Type& value)
    {
        return Column<_Name, _Type, false>{value};
    }

    template <ASYS::StringLiteral _Name, class _Type>
    constexpr auto Col(_Type&& value)
    {
        using Type = std::remove_cvref_t<_Type>;

        return Column<_Name, Type, true>
        {
            std::forward<_Type>(value)
        };
    }

    template <ASYS::StringLiteral _Name, class _Type>
    constexpr auto Col()
    {
        return Column<_Name, _Type, true>{ _Type{} };
    }

    template <class _Type>
    concept ColumnType = requires(_Type column)
    {
        typename std::remove_cvref_t<_Type>::ValueType;
        typename std::remove_cvref_t<_Type>::StorageType;
        std::remove_cvref_t<_Type>::Name;
        column.Value;
    };
};
