#pragma once

#include <algorithm>
#include <string>
#include <tuple>
#include <type_traits>

#include "Query/Query.hpp"

namespace ADBC
{
    class SQLite3Builder
    {
    public:
        /**
        * @brief 
        */
        template <class _Query>
        static constexpr auto Build(const _Query& query) -> std::string;

    private:
        template <class _Query>
        static constexpr auto BuildSelect(const _Query& query)
            -> std::string;

        template <class _Query>
        static constexpr auto BuildFrom(const _Query& query)
            -> std::string;
    };

    template <class _Query>
    constexpr auto SQLite3Builder::Build(const _Query& query) -> std::string
    {
        constexpr auto state = query.GetState();

        static_assert(state >= QueryState::From, "[ADBC::SQLite3Builder::"
            "Execute] Query's required to have FROM"); 

        auto queryText = BuildSelect(query);
        queryText += BuildFrom(query);

        return queryText;
    }

    template <class _Query>
    constexpr auto SQLite3Builder::BuildSelect
        (const _Query& query) -> std::string
    {
        const auto& outputPack = query.GetOutputPack();

        static_assert(std::tuple_size_v<std::remove_cvref_t
            <decltype(outputPack)>>, "Size of outputPack is 0");

        auto queryText = std::string{ "SELECT " };

        constexpr auto outputsSeparator = ASYS::SL{ ", " };

        auto AddOutput = [&] (const auto& output)
        {
            using ColumnType = std::remove_cvref_t<decltype(output)>;

            queryText += ColumnType::Name;
            queryText += outputsSeparator;
        };

        std::apply([&] (auto&& ... outputs)
        {
            (AddOutput(outputs), ...);
        }, outputPack);

        queryText.pop_back();
        queryText.pop_back();

        queryText += '\n';

        return queryText;
    }

    template <class _Query>
    constexpr auto SQLite3Builder::BuildFrom(const _Query& query)
        -> std::string
    {
        auto queryText = std::string{ "FROM " } 
            + query.GetTable();

        return queryText;
    }
};
