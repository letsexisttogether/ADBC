#pragma once

#include <cfloat>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>

#include <ASYS/String/StringLiteral.hpp>
#include <ADBC/Core/DBConnection.hpp>
#include <ADBC/Core/Table.hpp>

namespace ADBC 
{
    class SelectQuery;
    class FromQuery; 
    class WhereQuery;

    template <ColumnType... Columns>
    auto Select(Columns&&... columns) -> SelectQuery;

    class BaseQuery
    {
    protected:
        BaseQuery() = default;
        BaseQuery(const BaseQuery&) = delete;
        BaseQuery(std::string&& text);

        auto GetText() const -> const std::string&;

        auto operator = (const BaseQuery&) = delete;

    protected:
        std::string m_Text{};
    };

    class SelectQuery : private BaseQuery
    {
    public:
        using BaseQuery::GetText;

    public:
        auto From(std::string&& table) -> FromQuery;        

    private:
        using BaseQuery::BaseQuery;

    private:
        template <ColumnType... _Columns>
        friend auto Select(_Columns&&...) -> SelectQuery;
    };

    class FromQuery : private BaseQuery
    {
    public:
        using BaseQuery::GetText;

    public:
        auto Where(std::string&& condition) -> WhereQuery;

        auto Execute(SQLite3Database& db) -> void;

    private:
        using BaseQuery::BaseQuery;

    private:
        friend class SelectQuery;
    };

    class WhereQuery : private BaseQuery
    {
    public:
        using BaseQuery::GetText;

    public:
        auto Execute(SQLite3Database& db) -> void;

    private:
        using BaseQuery::BaseQuery;

    private:
        friend class FromQuery;
    };

    template <ColumnType... _Columns>
    auto Select(_Columns&&... columns) -> SelectQuery
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

        return SelectQuery{ std::move(text) };
    }
};
