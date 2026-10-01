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

    template
    <
        class _QueryOutputPack = std::tuple<>,
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

        template <std::size_t _Size>
        auto From(ASYS::StringLiteral<_Size> table)
            requires (ASYS::IsOneOfV<_State,
            QueryState::Select, QueryState::From>);

        template <class _Condition>
        auto Where(_Condition&& condition) 
            requires (ASYS::IsOneOfV<_State, QueryState::From>);

        template <class _Condition>
        auto And(_Condition&& condition) 
            requires (ASYS::IsOneOfV<_State, QueryState::Where>);

        template <class _Condition>
        auto Or(_Condition&& condition) 
            requires (ASYS::IsOneOfV<_State, QueryState::Where>);

        auto GetOutputPack() const noexcept -> const _QueryOutputPack&;
        auto GetOutputPack() noexcept -> _QueryOutputPack&;

        auto GetParamPack() const noexcept -> const _QueryParamPack&;
        auto GetParamPack() noexcept -> _QueryParamPack&;

    private:
        Query(const Query&) = delete;
        auto operator = (const Query&) = delete;

        Query(_QueryOutputPack outputs, _QueryParamPack params);

        template <class _Condition>
        auto ApplyOperator(_Condition&& condition, std::string&& opText);

    private:
        template <class, class, QueryState>
        friend class Query;

    private:
        _QueryOutputPack m_Outputs{};
        _QueryParamPack m_Params{};
    };

    template <class _QueryOutputPack, class _QueryParamPack, QueryState _State>
    template <ColumnType... _Columns>
    auto Query<_QueryOutputPack, _QueryParamPack, _State>::
    Select(_Columns&&... columns)
        requires (ASYS::IsOneOfV<_State,
        QueryState::None, QueryState::Select>)
    {
        auto outputs = std::tie(columns...);

        return Query<decltype(outputs), std::tuple<>, QueryState::Select>
        {
            outputs, {}
        };
    }

    template <class _QueryOutputPack, class _QueryParamPack, QueryState _State>
    template <std::size_t _Size>
    auto Query<_QueryOutputPack, _QueryParamPack, _State>::
    From(ASYS::StringLiteral<_Size> table)
        requires (ASYS::IsOneOfV<_State,
        QueryState::Select, QueryState::From>)
    {
        return Query<_QueryOutputPack, _QueryParamPack, QueryState::From>
        {
            std::move(m_Outputs), std::move(m_Params)
        };
    }

    template <class _QueryOutputPack, class _QueryParamPack, QueryState _State>
    template <class _Condition>
    auto Query<_QueryOutputPack, _QueryParamPack, _State>::
    Where(_Condition&& condition) 
        requires (ASYS::IsOneOfV<_State, QueryState::From>)
    {
        return ApplyOperator(condition, "WHERE");
    }

    template <class _QueryOutputPack, class _QueryParamPack, QueryState _State>
    template <class _Condition>
    auto Query<_QueryOutputPack, _QueryParamPack, _State>::
    And(_Condition&& condition) 
        requires (ASYS::IsOneOfV<_State, QueryState::Where>)
    {
        return ApplyOperator(condition, "AND");
    }

    template <class _QueryOutputPack, class _QueryParamPack, QueryState _State>
    template <class _Condition>
    auto Query<_QueryOutputPack, _QueryParamPack, _State>::
    Or(_Condition&& condition) 
        requires (ASYS::IsOneOfV<_State, QueryState::Where>)
    {
        return ApplyOperator(condition, "OR");
    }

    template <class _QueryOutputPack, class _QueryParamPack, QueryState _State>
    auto Query<_QueryOutputPack, _QueryParamPack, _State>::GetOutputPack()
        const noexcept -> const _QueryOutputPack&
    {
        return m_Outputs;
    }

    template <class _QueryOutputPack, class _QueryParamPack, QueryState _State>
    auto Query<_QueryOutputPack, _QueryParamPack, _State>::GetOutputPack()
        noexcept -> _QueryOutputPack&
    {
        return m_Outputs;
    }

    template <class _QueryOutputPack, class _QueryParamPack, QueryState _State>
    auto Query<_QueryOutputPack, _QueryParamPack, _State>::GetParamPack()
        const noexcept -> const _QueryParamPack&
    {
        return m_Params;
    }

    template <class _QueryOutputPack, class _QueryParamPack, QueryState _State>
    auto Query<_QueryOutputPack, _QueryParamPack, _State>::GetParamPack()
        noexcept -> _QueryParamPack&
    {
        return m_Params;
    }

    template <class _QueryOutputPack, class _QueryParamPack, QueryState _State>
    Query<_QueryOutputPack, _QueryParamPack, _State>::Query
        (_QueryOutputPack outputs, _QueryParamPack params)
        : m_Outputs{ std::move(outputs) }, m_Params{ std::move(params) } {}

    template <class _QueryOutputPack, class _QueryParamPack, QueryState _State>
    template <class _Condition>
    auto Query<_QueryOutputPack, _QueryParamPack, _State>::
        ApplyOperator(_Condition&& condition, std::string&& opText) 
    {
        auto params = std::tuple_cat(std::move(m_Params),
            std::forward<_Condition>(condition).Params);

        return Query<_QueryOutputPack, decltype(params), QueryState::Where>
        {
            std::move(params)
        };
    }

};
