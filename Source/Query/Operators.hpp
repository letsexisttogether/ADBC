#pragma once

#include <string>

#include "Schema/Column.hpp"

namespace ADBC::Operators
{
    template <ColumnType _Column>
    auto Equals(const _Column& column) -> std::string
    {
        auto text = std::string{ _Column::Name };
        text += " = ?";

        return text;
    }

    template <ColumnType _Column>
    auto NotEquals(const _Column& column) -> std::string
    {
        auto text = std::string{ _Column::Name };
        text += " != ?";

        return text;
    }

    template <ColumnType _Column>
    auto Between(const _Column& column) -> std::string
    {
        auto text = std::string{ _Column::Name };
        text += " BETWEEN ? AND ?";

        return text;
    }
};
