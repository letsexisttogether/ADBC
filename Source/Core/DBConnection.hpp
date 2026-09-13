#pragma once

namespace ADBC
{
    class Database 
    {
    public:
        virtual ~Database() = default;

        virtual auto ExecuteRaw(const char* const query) -> bool = 0;
    };

    class SQLite3Database : public Database
    {
    public:
        SQLite3Database();

        ~SQLite3Database();
    };
};
