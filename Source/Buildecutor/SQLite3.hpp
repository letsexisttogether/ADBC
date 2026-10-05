#pragma once

#include <algorithm>
#include <string>
#include <tuple>
#include <type_traits>

#include "Query/Query.hpp"
#include "Core/DBConnection.hpp"

namespace ADBC
{
    class SQLite3Buildecutor
    {
    public:
        static constexpr auto MaxQuerySize = 1024;

        template <class _Query>
        static consteval auto Build(const _Query& query);

        template <class _Query>
        static auto BuildSQL(const _Query& query);

    private:
        template <class _Query>
        static constexpr auto BuildSelect(const _Query& query)
            -> std::string;

        template <class _Query>
        static constexpr auto BuildFrom(const _Query& query)
            -> std::string;
    };

    template <class _Query>
    consteval auto SQLite3Buildecutor::Build(const _Query& query)
    {
        constexpr auto state = query.GetState();

        static_assert(state >= QueryState::From,
            "[ADBC::SQLite3Builder::Build] "
            "Query is required to have FROM");

        auto queryText = ASYS::SL<MaxQuerySize>{};

        queryText.Append(BuildSelect(query));
        queryText.Append(BuildFrom(query));

        return queryText;
    }


    template <class _Query>
    auto SQLite3Buildecutor::BuildSQL(const _Query& query)
    {
        constexpr auto builtQuery = Build(query);

        return std::string
        { 
            builtQuery.Data.data(), 
            builtQuery.GetLength()
        };
    }

    template <class _Query>
    constexpr auto SQLite3Buildecutor::BuildSelect
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
    constexpr auto SQLite3Buildecutor::BuildFrom
        (const _Query& query) -> std::string
    {
        auto queryText = std::string{ "FROM " }
            + query.GetTable();

        return queryText;
    }
};
