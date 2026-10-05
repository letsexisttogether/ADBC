#pragma once 

#include <type_traits>

#include <ASYS/String/StringLiteral.hpp>

namespace ADBC
{
    template <ASYS::StringLiteral _Name, class _Type, bool _IsValueEmbedded = true>
    struct Column
    { 
        using ValueType = _Type;
        using StorageType = std::conditional_t<_IsValueEmbedded,
            _Type, _Type&>;

        constexpr Column() requires (_IsValueEmbedded) = default;

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

        return Column<_Name, Type>
        {
            std::forward<_Type>(value)
        };
    }

    template <ASYS::StringLiteral _Name, class _Type>
    constexpr auto Col()
    {
        return Column<_Name, _Type>{ _Type{} };
    }

    template <class _Column>
    concept ColumnType = requires (_Column column)
    {
        typename std::remove_cvref_t<_Column>::ValueType;
        typename std::remove_cvref_t<_Column>::StorageType;
        std::remove_cvref_t<_Column>::Name;
        column.Value;
    };
};
