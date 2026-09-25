#pragma once

#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>

#include <ASYS/String/StringLiteral.hpp>
#include <ADBC/Core/DBConnection.hpp>
#include <ADBC/Core/Table.hpp>

namespace ADBC 
{
    class SelectQuery{}; // From()

    class FromQuery{}; // Where(), AsExecutable()

    class WhereQuery{}; // And(), Or(), AsExecutable()

    template <ColumnType... Columns>
    auto Select(Columns&&... columns) -> std::string 
    {
        auto text = std::string{ "SELECT " };

        auto isFirst = true;

        (
            (
                text += ((isFirst) ? (""): (", ")),
                isFirst = false,
                text += std::remove_cvref_t<Columns>::Name
            ),
            ...
        );

        return text;
    }

    class Query
    {
    public:
        template <class ... _Fields>
        auto Select(_Fields&& ... fields) -> Query&
        {
            m_State = State::Select;

            m_Text = "SELECT ";

            auto AddField = [&] (auto field)
            {
                m_Text += field;
                m_Text += ", ";
            };

            (AddField(fields), ...);

            if (!m_Text.empty() && m_Text.back() == ' ')
            {
                m_Text.pop_back();
                m_Text.pop_back();
            }

            m_Text += '\n';

            return *this;
        }

        template <class ... _Tables>
        auto From(_Tables&& ... tables) -> Query& 
        {
            if (m_State != State::Select)
            {
                throw std::runtime_error{ "[ADBC::Query] FROM "
                    "not after SELECT" };
            }

            m_State = State::From;

            m_Text += "FROM ";

            ((m_Text += tables), ...);

            m_Text += '\n';

            return *this;
        }

        auto Where() -> Query& 
        {
            if (m_State != State::From)
            {
                throw std::runtime_error{ "[ADBC::Query] WHERE "
                    "not after FROM" };
            }

            m_State = State::Where;

            m_Text += "WHERE 1 = 1\n";

            return *this;
        }

        template <class ... _Params>
        auto Operator(const char* const conjunction,
            const char* const operation, _Params&& ... params) -> Query& 
        {
            if (m_State != State::Where)
            {
                throw std::runtime_error{ "[ADBC::Query] Operation "
                    "not after WHERE" };
            }

            auto AddParam = [&] (auto param)
            {
                m_Text += conjunction;
                m_Text += ' ';
                m_Text += param;
                m_Text += ' ';
                m_Text += operation;
                m_Text += " ?\n";
            };

            (AddParam(params), ...);

            return *this;
        };

        operator std::string& () noexcept
        {
            return m_Text;
        }

    private:
        enum class State : std::uint8_t
        {
            NotInit,
            Select,
            From,
            Where
        };

    private:
        State m_State{};
        std::string m_Text{};
    };
};
