#pragma once

#include "Core/Table.hpp"
#include "Query/Query.hpp"

namespace ADBC 
{
    template <ColumnType... _Columns>
    auto Select(_Columns&&... columns)
    {
        auto text = std::string{ "SELECT " };

        auto isFirst = true;
        (
            (
                text += ((isFirst) ? (""): (", ")),
                isFirst = false,
                text += std::remove_cvref_t<_Columns>::Name
            ),
            ...
        );

        auto outputs = std::tie(columns.Value...);

        return Query<decltype(outputs), std::tuple<>>
        {
            std::move(text), outputs, {}
        };
    }
};
