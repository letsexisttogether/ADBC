#pragma once

#include <cstdint>
#include <functional>
#include <stdexcept>
#include <tuple>
#include <type_traits>

#include <ASYS/String/StringLiteral.hpp>
#include <sqlite3.h>

namespace ADBC
{
    template <class ... _Args>
    struct SQLParamPack
    {
        std::tuple<_Args...> Values{};
    };

    template <class ... _Args>
    auto CreateSQLParams(_Args&& ... args) -> SQLParamPack
        <std::decay_t<_Args>...>
    {
        return SQLParamPack<std::decay_t<_Args>...>
        {
            std::tuple{ std::forward<_Args>(args) ... }
        };
    }

    template<class... _Args>
    struct SQLOutputPack
    {
        std::tuple<_Args&...> Values;
    };

    template<class... _Args>
    auto CreateSQLOutputs(_Args&... args)
    {
        return SQLOutputPack<_Args...>
        {
            std::tie(args...)
        };
    }

    class SQLite3Database
    {
    public:
        SQLite3Database(const SQLite3Database&) = delete;

        SQLite3Database(const char* const dbName);

        ~SQLite3Database();

        template <std::size_t _Size, class ... _Outputs, 
            class ... _Params, class Callback>
        auto ExecuteRawQuery(ASYS::StringLiteral<_Size> query,
            SQLOutputPack<_Outputs...> outputs, SQLParamPack<_Params...> params,
            Callback&& callback) -> void;

        template <class ... _Outputs, class ... _Params, class _Callback>
        auto Execute(const std::string& query, SQLOutputPack<_Outputs...> outputs,
            SQLParamPack<_Params...> params, _Callback&& callback) -> void;

        template <class _OutputPack, class _ParamPack, class _Callback>
        auto Execute(const std::string& query, _OutputPack& outputs,
            const _ParamPack& params, _Callback&& callback) -> void;

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

        /**
        * @brief Binds the value for the statement.
        *
        * @pre statement should not be null
        */
        template <class _Type>
        auto BindValue(const _Type& value, sqlite3_stmt* const statement,
            const ColumnID paramID) -> void;

    private:
        sqlite3* m_DB{};
    };

    template <std::size_t _Size, class ... _Outputs, 
        class ... _Params, class Callback>
    auto SQLite3Database::ExecuteRawQuery(ASYS::StringLiteral<_Size> query,
        SQLOutputPack<_Outputs...> outputs, SQLParamPack<_Params...> params,
        Callback&& callback) -> void
    {
        Execute(query, outputs, params, std::move(callback));
    }

    template <class ... _Outputs, class ... _Params, class _Callback>
    auto SQLite3Database::Execute(const std::string& query,
        SQLOutputPack<_Outputs...> outputs, SQLParamPack<_Params...> params,
        _Callback&& callback) -> void
    {
        sqlite3_stmt* statement{};

        if (sqlite3_prepare_v2(m_DB, query.c_str(), query.size(),
            &statement, nullptr) != SQLITE_OK)
        {
            throw std::runtime_error{ "[ADBC::SQLITE3Database] "
                "Can't create statement" };
        }

        if (const auto columnCount = sqlite3_column_count(statement);
            columnCount < sizeof ... (_Outputs)) 
        {
            throw std::runtime_error{ "[ADBC::SQLITE3Database] "
                "Column count is less than provided values" };
        }

        std::apply([&](auto&&... values)
        {
            auto paramID = ColumnID{ 1 };
            (BindValue(values, statement, paramID), ...);
        }, params.Values);

        for (auto stepResult = SQLITE_ROW; stepResult == SQLITE_ROW; )
        {
            stepResult = sqlite3_step(statement);

            if (stepResult == SQLITE_DONE)
            {
                return;
            }

            std::apply([&] (auto&... values)
            {
                auto columnID = ColumnID{};
                (ExtractValue(values, statement, columnID++), ...);

                std::invoke(std::forward<_Callback>(callback), values...);

            }, outputs.Values);
        }
    }

    template <class _OutputPack, class _ParamPack, class _Callback>
    auto SQLite3Database::Execute(const std::string& query, _OutputPack& outputs,
        const _ParamPack& params, _Callback&& callback) -> void
    {
        sqlite3_stmt* statement{};

        if (sqlite3_prepare_v2(m_DB, query.c_str(), query.size(),
            &statement, nullptr) != SQLITE_OK)
        {
            throw std::runtime_error{ "[ADBC::SQLITE3Database] "
                "Can't create statement" };
        }

        if (const auto columnCount = sqlite3_column_count(statement);
            columnCount < std::tuple_size_v<_OutputPack>) 
        {
            throw std::runtime_error{ "[ADBC::SQLITE3Database] "
                "Column count is less than provided values" };
        }

        std::apply([&](const auto&... params)
        {
            auto parameterId = 1;
            (
                BindValue(params, statement, parameterId++), ...
            );
        }, params);

        while (sqlite3_step(statement) == SQLITE_ROW)
        {
            std::apply([&](auto&... outputs)
            {
                auto columnId = 0;
                (
                    ExtractValue(outputs, statement, columnId++), ...
                );
                std::invoke(callback, outputs...);
            }, outputs);
        }

        sqlite3_finalize(statement);
    }
};
