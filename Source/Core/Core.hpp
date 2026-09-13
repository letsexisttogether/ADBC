#pragma once

#include <cstdint>
#include <string>

#include <ASYS/String/StringLiteral.hpp>

namespace ADBC
{
    template <ASYS::StringLiteral _Name, class _Type>
    struct Column
    {
        constexpr static auto Name = _Name;
        using Type = _Type;
    };

    class Users
    {
        Column<"ID", std::uint64_t> ID{}; 
        Column<"Name", std::string> Name{}; 
        Column<"Email", std::string> Email{}; 
    };

    enum class DBType
    {
        SQLite3,
        PostgreSQL
    };

    class QueryExecutor
    {
    public:
        QueryExecutor() 

    private:
        DBType m_Type{};
    };
};
