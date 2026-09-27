#pragma once

#include <ASYS/String/StringLiteral.hpp>
#include <ASYS/Traits/Traits.hpp>

#include "Core/DBConnection.hpp"
#include "Schema/Column.hpp"

namespace ADBC
{
    template <class ... _Types>
    using QueryOutputPack = std::tuple<_Types&...>;

    template <class ... _Types>
    using QueryParamPack = std::tuple<_Types...>;

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

        auto Where() requires (ASYS::IsOneOfV<_State,
            QueryState::From>);

        auto GetText() const noexcept -> const std::string&;

        auto GetOutputPack() const noexcept -> const _QueryOutputPack&;
        auto GetParamPack() const noexcept -> const _QueryParamPack&;

    private:
        Query(const Query&) = delete;

        Query(std::string&& text, _QueryOutputPack outputs,
            _QueryParamPack params);

        auto operator = (const Query&) = delete;

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
    auto Query<_QueryOutputPack, _QueryParamPack, _State>::Where()
        requires (ASYS::IsOneOfV<_State, QueryState::From>)
    {
        m_Text += "WHERE 1 = 1\n";

        return Query<_QueryOutputPack, _QueryParamPack, QueryState::Where>
        {
            std::move(m_Text), std::move(m_Outputs), std::move(m_Params)
        };
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
};
