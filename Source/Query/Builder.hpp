#pragma once

#include "Schema/Column.hpp"
#include "Query/Query.hpp"

namespace ADBC 
{
    template <ColumnType... _Columns>
    auto Select(_Columns&&... columns)
    {
        auto text = std::string{ "SELECT " };

        if (!(sizeof ... (_Columns)))
        {
            text += "*";
        }

        auto isFirst = true;
        (
            (
                text += ((isFirst) ? (""): (", ")),
                isFirst = false,
                text += std::remove_cvref_t<_Columns>::Name
            ),
            ...
        );

        text += '\n';

        auto outputs = std::tie(columns.Value...);

        return Query<decltype(outputs), std::tuple<>, QueryState::Select>
        {
            std::move(text), outputs, {}
        };
    }
};
