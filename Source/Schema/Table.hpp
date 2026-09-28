#pragma once 

#include <cstdint>
#include <string>

#include "Schema/Column.hpp"

namespace ADBC
{
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
