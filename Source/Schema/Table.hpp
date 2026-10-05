#pragma once 

#include <cstdint>
#include <string>

#include "Schema/Column.hpp"

namespace ADBC
{
    template <ASYS::StringLiteral _Name>
    struct Table
    {
        static constexpr auto Name = _Name;
    };

    template <ASYS::StringLiteral _Name>
    constexpr auto Tbl()
    {
        return Table<_Name>{};
    }

    template<class _Type>
    concept TableType = requires
    {
        std::remove_cvref_t<_Type>::Name;
    };

    struct UsersTable 
    {
        struct Data 
        {
            std::int32_t ID{};
            std::string Name{};
            std::string Email{};
        };

        static Data Entity;

        inline static constexpr auto ID =
            Col<ASYS::SL{ "ID" }>(Entity.ID);

        inline static constexpr auto Name =
            Col<ASYS::SL{ "Name" }>(Entity.Name);

        inline static constexpr auto Email =
            Col<ASYS::SL{ "Email" }>(Entity.Email);
    };

    inline UsersTable::Data UsersTable::Entity{};
};
