#pragma once

#include <tuple>

#include <ASYS/String/StringLiteral.hpp>
#include <ASYS/Traits/Traits.hpp>

#include "Schema/Column.hpp"

namespace ADBC
{
    /*
    * Requirements:
    * 1. Query checks itself for methods.
    * 2. Query builds output list, param list.
    * 3. Query builds its type.
    * 4. Query is comp-time.
    * 5. Executor checks the flags of the query.
    * 6. Query's not dependent on the back end, executor is.
    * Builder get 
    */

    template <class ... _Types>
    using QueryOutputPack = std::tuple<_Types&...>;

    template <class ... _Types>
    using QueryParamPack = std::tuple<_Types...>;

    /**
    * @brief Represents current state of the query
    * @details TODO #2: Remake to flags
    */
    enum class QueryState
    {
        None,
        Select,
        From,
        Where
    };

    #define ADBCQueryTemplates                                  \
        template                                                \
        <                                                       \
            class _QueryOutputPack,                             \
            class _Table,                                       \
            class _QueryParamPack,                              \
            QueryState _State                                   \
        >

    #define ADBCQueryMethod                                     \
        Query<_QueryOutputPack, _Table,                         \
        _QueryParamPack, _State>::                              
    
    template
    <
        class _QueryOutputPack = std::tuple<>,
        class _Table = const char*,
        class _QueryParamPack = std::tuple<>,
        QueryState _State = QueryState::None
    >
    class Query
    {
    public:
        consteval Query() = default;

        template <ColumnType... _Columns>
        consteval auto Select(_Columns&&... columns)
            requires (ASYS::IsOneOfV<_State,
            QueryState::None, QueryState::Select>);

        consteval auto From(_Table table)
            requires (ASYS::IsOneOfV<_State,
            QueryState::Select, QueryState::From>);

        consteval auto GetOutputPack() const noexcept
            -> const _QueryOutputPack&;
        consteval auto GetOutputPack() noexcept
            -> _QueryOutputPack&;

        consteval auto GetTable() const noexcept
            -> const _Table&;
        consteval auto GetTable() noexcept
            -> _Table&;

        consteval auto GetParamPack() const noexcept
            -> const _QueryParamPack&;
        consteval auto GetParamPack() noexcept
            -> _QueryParamPack&;

        consteval auto GetState() const noexcept
            -> QueryState;

    private:
        Query(const Query&) = delete;
        auto operator = (const Query&) = delete;

        consteval Query(_QueryOutputPack outputs);
        consteval Query(_QueryOutputPack outputs, _Table table);
        consteval Query(_QueryOutputPack outputs, _Table table,
            _QueryParamPack params);

    private:
        template <class, class, class, QueryState>
        friend class Query;

    private:
        _QueryOutputPack m_Outputs{};
        _Table m_Table{};
        _QueryParamPack m_Params{};
    };

    ADBCQueryTemplates
    template <ColumnType... _Columns>
    consteval auto ADBCQueryMethod Select(_Columns&&... columns)
        requires (ASYS::IsOneOfV<_State,
        QueryState::None, QueryState::Select>)
    {
        auto outputs = std::tie(columns...);

        return Query
        <
            decltype(outputs), _Table,
            _QueryParamPack, QueryState::Select
        >
        {
            outputs
        };
    }

    ADBCQueryTemplates
    consteval auto ADBCQueryMethod From(_Table table)
        requires (ASYS::IsOneOfV<_State,
        QueryState::Select, QueryState::From>)
    {
        return Query
        <
            _QueryOutputPack, _Table,
            _QueryParamPack, QueryState::From
        >
        {
            std::move(m_Outputs), table
        };
    }

    ADBCQueryTemplates
    consteval auto ADBCQueryMethod GetOutputPack()
        const noexcept -> const _QueryOutputPack&
    {
        return m_Outputs;
    }

    ADBCQueryTemplates
    consteval auto ADBCQueryMethod GetOutputPack()
        noexcept -> _QueryOutputPack&
    {
        return m_Outputs;
    }

    ADBCQueryTemplates
    consteval auto ADBC::ADBCQueryMethod GetTable()
        const noexcept -> const _Table&
    {
        return m_Table;
    }

    ADBCQueryTemplates
    consteval auto ADBC::ADBCQueryMethod GetTable()
        noexcept -> _Table&
    {
        return m_Table;
    }

    ADBCQueryTemplates
    consteval auto ADBCQueryMethod GetParamPack()
        const noexcept -> const _QueryParamPack&
    {
        return m_Params;
    }

    ADBCQueryTemplates
    consteval auto ADBCQueryMethod GetParamPack()
        noexcept -> _QueryParamPack&
    {
        return m_Params;
    }

    ADBCQueryTemplates
    consteval auto ADBCQueryMethod GetState() const noexcept
        -> QueryState
    {
        return _State;
    }

    ADBCQueryTemplates
    consteval ADBCQueryMethod Query(_QueryOutputPack outputs)
        : m_Outputs{ std::move(outputs) } {}

    ADBCQueryTemplates
    consteval ADBCQueryMethod Query(_QueryOutputPack outputs,
        _Table table) : m_Outputs{ std::move(outputs) },
        m_Table{ std::forward<_Table>(table) } {}

    ADBCQueryTemplates
    consteval ADBCQueryMethod Query(_QueryOutputPack outputs,
        _Table table, _QueryParamPack params)
        : m_Outputs{ std::move(outputs) },
        m_Table{ std::forward<_Table>(table) },
        m_Params{ std::move(params) } {}
};
