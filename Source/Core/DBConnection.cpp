#include "DBConnection.hpp"

#include <stdexcept>

namespace ADBC
{
    SQLite3Database::SQLite3Database(const char* const dbName)
    {
        if (sqlite3_open(dbName, &m_DB) != SQLITE_OK)
        {
            throw std::runtime_error{ "[ADBC::SQLITE3Database] "
                "Can't open db" };
        }
    }

    SQLite3Database::~SQLite3Database()
    {
        sqlite3_close(m_DB);
    }
    
    template <>
    auto SQLite3Database::ExtractValue(std::int32_t& value,
        sqlite3_stmt* const statement, const ColumnID columnID) -> void
    {
        value = sqlite3_column_int(statement, columnID);
    }

    template <>
    auto SQLite3Database::ExtractValue(std::int64_t& value,
        sqlite3_stmt* const statement, const ColumnID columnID) -> void
    {
        value = sqlite3_column_int64(statement, columnID);
    }

    template <>
    auto SQLite3Database::ExtractValue(double& value,
        sqlite3_stmt* const statement, const ColumnID columnID) -> void
    {
        value = sqlite3_column_double(statement, columnID);
    }

    template <>
    auto SQLite3Database::ExtractValue(std::string& value,
        sqlite3_stmt* const statement, const ColumnID columnID) -> void
    {
        value = reinterpret_cast<const char*>(sqlite3_column_text
            (statement, columnID));
    }

    template <>
    auto SQLite3Database::BindValue<std::int32_t>(const std::int32_t& value,
        sqlite3_stmt* const statement, const ColumnID paramID) -> void
    {
        sqlite3_bind_int(statement, paramID, value);
    }

    template <>
    auto SQLite3Database::BindValue<double>(const double& value,
        sqlite3_stmt* const statement, const ColumnID paramID) -> void
    {
        sqlite3_bind_double(statement, paramID, value);
    }

    template <>
    auto SQLite3Database::BindValue<std::string>(const std::string& value,
        sqlite3_stmt* const statement, const ColumnID paramID) -> void
    {
        sqlite3_bind_text(statement, paramID, value.c_str(),
            value.size(), SQLITE_STATIC);
    }
};
