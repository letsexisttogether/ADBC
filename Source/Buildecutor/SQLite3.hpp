#pragma once

#include <tuple>
#include <type_traits>

#include "Query/Query.hpp"

namespace ADBC
{
    class SQLite3Buildecutor
    {
    public:
        static constexpr auto MaxQuerySize = 1024;

        template<class _Query>
        static consteval auto BuildComptime();

        template<class _Query>
        static auto BuildSQL() -> std::string;

    private:
        template<class _Query>
        static consteval auto Build();

        template<class _Query>
        static consteval auto BuildSelect();

        template<class _Query>
        static consteval auto BuildFrom();
    };

    template<class _Query>
    consteval auto SQLite3Buildecutor::BuildComptime()
    {
        constexpr auto queryText = Build<_Query>();

        return ASYS::Trim<queryText>();
    }

    template<class _Query>
    consteval auto ADBC::SQLite3Buildecutor::Build()
    {
        static_assert(
            _Query::State >= QueryState::From,
            "[ADBC::SQLite3Buildecutor::Build] Query requires FROM"
        );

        auto queryText = ASYS::SL<MaxQuerySize>{};

        queryText.Append(BuildSelect<_Query>());
        queryText.Append(BuildFrom<_Query>());

        return queryText;
    }

    template <class _Query>
    auto SQLite3Buildecutor::BuildSQL() -> std::string
    {
        constexpr auto builtQuery = Build<_Query>();

        return std::string
        { 
            builtQuery.Data.data(), 
            builtQuery.GetLength()
        };
    }

    template<class _Query>
    consteval auto SQLite3Buildecutor::BuildSelect()
    {
        using OutputPack = typename _Query::QueryOutputPack;

        constexpr auto outputsCount = std::tuple_size_v<OutputPack>;

        static_assert(outputsCount);

        auto queryText = std::string{};

        queryText += "SELECT ";

        [&]<std::size_t... _Indices>(std::index_sequence<_Indices...>)
        {
            ([&]
            {
                using Column = std::remove_cvref_t<std::tuple_element_t
                    <_Indices, OutputPack>>;

                if constexpr (_Indices != 0)
                {
                    queryText += ", ";
                }

                queryText += Column::Name;
            }(), ...);
        } (std::make_index_sequence<outputsCount>{});

        queryText += '\n';

        return queryText;
    }

    template<class _Query>
    consteval auto SQLite3Buildecutor::BuildFrom()
    {
        using Table = typename _Query::Table;

        auto queryText = std::string{};

        queryText += "FROM ";
        queryText += Table::Name;

        return queryText;
    }
};
