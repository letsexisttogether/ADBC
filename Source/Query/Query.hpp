#pragma once

#include <ASYS/String/StringLiteral.hpp>
#include <ASYS/Traits/Traits.hpp>

#include "Schema/Column.hpp"
#include "Query/Operators.hpp"

namespace ADBC
{
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
        Query() = default;

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

        template <class _DB, class _Callback>
        auto Execute(_DB& db, _Callback&& callback) -> void;

        auto GetText() const noexcept -> const std::string&;

        auto GetOutputPack() const noexcept -> const _QueryOutputPack&;
        auto GetParamPack() const noexcept -> const _QueryParamPack&;

    private:
        Query(const Query&) = delete;
        auto operator = (const Query&) = delete;

        Query(std::string&& text, _QueryOutputPack outputs,
            _QueryParamPack params);

        template <class _Condition>
        auto ApplyOperator(_Condition&& condition, std::string&& opText);

    private:
        template <class, class, QueryState>
        friend class Query;

    private:
        std::string m_Text{};
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

    template <class _QueryOutputPack, class _QueryParamPack, QueryState _State>
    template <std::size_t _Size>
    auto Query<_QueryOutputPack, _QueryParamPack, _State>::
    From(ASYS::StringLiteral<_Size> table)
        requires (ASYS::IsOneOfV<_State,
        QueryState::Select, QueryState::From>)
    {
        m_Text += "FROM ";
        m_Text += table;
        m_Text += '\n';

        return Query<_QueryOutputPack, _QueryParamPack, QueryState::From>
        {
            std::move(m_Text), std::move(m_Outputs), std::move(m_Params)
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
    template <class _DB, class _Callback>
    auto Query<_QueryOutputPack, _QueryParamPack, _State>::
    Execute(_DB& db, _Callback&& callback) -> void
    {
        db.Execute(m_Text, m_Outputs, m_Params, callback);
    }

    template <class _QueryOutputPack, class _QueryParamPack, QueryState _State>
    auto Query<_QueryOutputPack, _QueryParamPack, _State>::GetText()
        const noexcept -> const std::string&
    {
        return m_Text;
    }

    template <class _QueryOutputPack, class _QueryParamPack, QueryState _State>
    auto Query<_QueryOutputPack, _QueryParamPack, _State>::GetOutputPack()
        const noexcept -> const _QueryOutputPack&
    {
        return m_Outputs;
    }

    template <class _QueryOutputPack, class _QueryParamPack, QueryState _State>
    auto Query<_QueryOutputPack, _QueryParamPack, _State>::GetParamPack()
        const noexcept -> const _QueryParamPack&
    {
        return m_Outputs;
    }

    template <class _QueryOutputPack, class _QueryParamPack, QueryState _State>
    Query<_QueryOutputPack, _QueryParamPack, _State>::Query(std::string&& text,
        _QueryOutputPack outputs, _QueryParamPack params)
        : m_Text{ std::move(text) }, m_Outputs{ std::move(outputs) },
        m_Params{ std::move(params) } {}

    template <class _QueryOutputPack, class _QueryParamPack, QueryState _State>
    template <class _Condition>
    auto Query<_QueryOutputPack, _QueryParamPack, _State>::
        ApplyOperator(_Condition&& condition, std::string&& opText) 
    {
        m_Text += std::move(opText);
        m_Text += ' ';
        m_Text += condition.Text;
        m_Text += '\n';

        auto params = std::tuple_cat(std::move(m_Params),
            std::forward<_Condition>(condition).Params);

        return Query<_QueryOutputPack, decltype(params), QueryState::Where>
        {
            std::move(m_Text),
            std::move(m_Outputs),
            std::move(params)
        };
    }

};
