#include "Builder.hpp"

namespace ADBC
{
    BaseQuery::BaseQuery(std::string&& text)
        : m_Text{ std::move(text) } {}

    auto BaseQuery::GetText() const -> const std::string&
    {
        return m_Text;
    }
    
    auto SelectQuery::From(std::string&& table) -> FromQuery
    {
        m_Text += "FROM ";
        m_Text += table;
        m_Text += '\n';

        return FromQuery{ std::move(m_Text) };
    }

    auto Where(std::string&& condition) -> WhereQuery
    {
    };
};
