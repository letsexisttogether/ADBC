#pragma once

#include <ASYS/String/StringLiteral.hpp>
#include <ASYS/Traits/Traits.hpp>

#include "Schema/Column.hpp"
#include "Query/Operators.hpp"

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
        constexpr Query() = default;

        template <ColumnType... _Columns>
        auto Select(_Columns&&... columns)
            requires (ASYS::IsOneOfV<_State,
            QueryState::None, QueryState::Select>);

        auto From(_Table table)
            requires (ASYS::IsOneOfV<_State,
            QueryState::Select, QueryState::From>);

        /*
        template <class _Condition>
        auto Where(_Condition&& condition) 
            requires (ASYS::IsOneOfV<_State, QueryState::From>);

        template <class _Condition>
        auto And(_Condition&& condition) 
            requires (ASYS::IsOneOfV<_State, QueryState::Where>);

        template <class _Condition>
        auto Or(_Condition&& condition) 
            requires (ASYS::IsOneOfV<_State, QueryState::Where>);
        */

        auto GetOutputPack() const noexcept -> const _QueryOutputPack&;
        auto GetOutputPack() noexcept -> _QueryOutputPack&;

        auto GetTable() const noexcept -> const _Table&;
        auto GetTable() noexcept -> _Table&;

        auto GetParamPack() const noexcept -> const _QueryParamPack&;
        auto GetParamPack() noexcept -> _QueryParamPack&;

    private:
        Query(const Query&) = delete;
        auto operator = (const Query&) = delete;

        constexpr Query(_QueryOutputPack outputs);
        constexpr Query(_QueryOutputPack outputs, _Table table);
        constexpr Query(_QueryOutputPack outputs, _Table table,
            _QueryParamPack params);

        /*
        template <class _Condition>
        auto ApplyOperator(_Condition&& condition, std::string&& opText);
        */

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
    auto ADBCQueryMethod Select(_Columns&&... columns)
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
    auto ADBCQueryMethod From(_Table table)
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

    /*
    ADBCQueryTemplates
    template <class _Condition>
    auto ADBCQueryMethod Where(_Condition&& condition) 
        requires (ASYS::IsOneOfV<_State, QueryState::From>)
    {
        return ApplyOperator(condition, "WHERE");
    }

    ADBCQueryTemplates
    template <class _Condition>
    auto ADBCQueryMethod And(_Condition&& condition) 
        requires (ASYS::IsOneOfV<_State, QueryState::Where>)
    {
        return ApplyOperator(condition, "AND");
    }

    ADBCQueryTemplates
    template <class _Condition>
    auto ADBCQueryMethod Or(_Condition&& condition) 
        requires (ASYS::IsOneOfV<_State, QueryState::Where>)
    {
        return ApplyOperator(condition, "OR");
    }
    */

    ADBCQueryTemplates
    auto ADBCQueryMethod GetOutputPack() const noexcept
        -> const _QueryOutputPack&
    {
        return m_Outputs;
    }

    ADBCQueryTemplates
    auto ADBCQueryMethod GetOutputPack() noexcept
        -> _QueryOutputPack&
    {
        return m_Outputs;
    }

    ADBCQueryTemplates
    auto ADBC::ADBCQueryMethod GetTable() const noexcept
        -> const _Table&
    {
        return m_Table;
    }

    ADBCQueryTemplates
    auto ADBC::ADBCQueryMethod GetTable() noexcept
        -> _Table&
    {
        return m_Table;
    }

    ADBCQueryTemplates
    auto ADBCQueryMethod GetParamPack() const noexcept
        -> const _QueryParamPack&
    {
        return m_Params;
    }

    ADBCQueryTemplates
    auto ADBCQueryMethod GetParamPack() noexcept
        -> _QueryParamPack&
    {
        return m_Params;
    }

    ADBCQueryTemplates
    constexpr ADBCQueryMethod Query(_QueryOutputPack outputs)
        : m_Outputs{ std::move(outputs) } {}

    ADBCQueryTemplates
    constexpr ADBCQueryMethod Query(_QueryOutputPack outputs,
        _Table table) : m_Outputs{ std::move(outputs) },
        m_Table{ std::forward<_Table>(table) } {}

    ADBCQueryTemplates
    constexpr ADBCQueryMethod Query(_QueryOutputPack outputs,
        _Table table, _QueryParamPack params)
        : m_Outputs{ std::move(outputs) },
        m_Table{ std::forward<_Table>(table) },
        m_Params{ std::move(params) } {}

    /*
    ADBCQueryTemplates
    template <class _Condition>
    auto ADBCQueryMethod ApplyOperator(_Condition&& condition,
        std::string&& opText) 
    {
        auto params = std::tuple_cat(std::move(m_Params),
            std::forward<_Condition>(condition).Params);

        return Query<_QueryOutputPack, decltype(params), QueryState::Where>
        {
            std::move(params)
        };
    }
    */
};
