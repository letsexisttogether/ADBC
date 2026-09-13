#pragma once

#include <cstdint>
#include <functional>
#include <stdexcept>

#include <ASYS/String/StringLiteral.hpp>
#include <sqlite3.h>

namespace ADBC
{
    class SQLite3Database
    {
    public:
        SQLite3Database(const SQLite3Database&) = delete;

        SQLite3Database(const char* const dbName);

        ~SQLite3Database();

        template <std::size_t _Size, class Callback, class ... _Args>
        auto ExecuteRawQuery(ASYS::StringLiteral<_Size> query,
            Callback&& callback, _Args& ... args) -> void; 

        auto operator = (const SQLite3Database&) = delete;

    private:
        using ColumnID = std::int32_t;

    private:
        /**
        * @brief Extracts the value from the statement.
        *
        * @pre statement should not be null
        */
        template <class _Type>
        auto ExtractValue(_Type& value, sqlite3_stmt* const statement,
            const ColumnID columnID) -> void;

    private:
        sqlite3* m_DB{};
    };

    template <std::size_t _Size, class Callback, class ... _Args>
    auto SQLite3Database::ExecuteRawQuery(ASYS::StringLiteral<_Size> query,
        Callback&& callback, _Args& ... args) -> void
    {
        sqlite3_stmt* statement{};

        if (sqlite3_prepare_v2(m_DB, query, query.GetSize(),
            &statement, nullptr) != SQLITE_OK)
        {
            throw std::runtime_error{ "[ADBC::SQLITE3Database] "
                "Can't open db" };
        }

        if (const auto columnCount = sqlite3_column_count(statement);
            columnCount < sizeof ... (_Args))
        {
            throw std::runtime_error{ "[ADBC::SQLITE3Database] "
                "Column count is less than provided values" };
        }

        for (auto stepResult = SQLITE_ROW; stepResult == SQLITE_ROW; )
        {
            stepResult = sqlite3_step(statement);

            if (stepResult == SQLITE_DONE)
            {
                return;
            }

            auto columnID = ColumnID{};

            (ExtractValue(args, statement, columnID++), ...);

            std::invoke(std::forward<Callback>(callback), args...);
        }
    }
};
