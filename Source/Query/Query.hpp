#pragma once

#include <tuple>

#include <ASYS/String/StringLiteral.hpp>
#include <ASYS/Traits/Traits.hpp>
#include <type_traits>

#include "Schema/Table.hpp"

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
        class _Table = void,
        class _QueryParamPack = std::tuple<>,
        QueryState _State = QueryState::None
    >
    class Query
    {
    public:
        using QueryOutputPack = _QueryOutputPack;
        using Table = _Table;
        using QueryParamPack = _QueryParamPack;

    public:
        consteval Query() = default;

        template <ColumnType... _Columns>
        consteval auto Select(_Columns&&... columns)
            requires (ASYS::IsOneOfV<_State,
            QueryState::None, QueryState::Select>);

        template <TableType _FromTable>
        consteval auto From(_FromTable&&)
            requires (ASYS::IsOneOfV<_State,
            QueryState::Select, QueryState::From>);

        consteval auto GetOutputPack() const noexcept
            -> const _QueryOutputPack&;
        consteval auto GetOutputPack() noexcept
            -> _QueryOutputPack&;

        consteval auto GetParamPack() const noexcept
            -> const _QueryParamPack&;
        consteval auto GetParamPack() noexcept
            -> _QueryParamPack&;

        consteval auto GetState() const noexcept
            -> QueryState;

    public:
        static constexpr auto State = _State;

    private:
        Query(const Query&) = delete;
        auto operator = (const Query&) = delete;

        consteval Query(_QueryOutputPack outputs);
        consteval Query(_QueryOutputPack outputs, _QueryParamPack params);

    private:
        template <class, class, class, QueryState>
        friend class Query;

    private:
        _QueryOutputPack m_Outputs{};
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
    template <TableType _FromTable>
    consteval auto ADBCQueryMethod From(_FromTable&&)
        requires (ASYS::IsOneOfV<_State,
        QueryState::Select, QueryState::From>)
    {
        return Query
        <
            _QueryOutputPack, std::remove_cvref_t<_FromTable>,
            _QueryParamPack, QueryState::From
        >
        {
            std::move(m_Outputs)
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
        _QueryParamPack params) : m_Outputs{ std::move(outputs) },
        m_Params{ std::move(params) } {}
};
