#pragma once

#include <string>

#include "Core/DBConnection.hpp"

namespace ADBC
{
    class Query
    {
    public:
        Query(SQLite3Database& db, std::string&& text)
            : m_DB{ db }, m_Text{ std::move(text) } {}

        auto Execute() -> void
        {

        }

    private:
        std::string m_Text{};
        SQLite3Database& m_DB;
    };
};
