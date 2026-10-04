#include <iostream>
#include <cstdint>

#include <ADBC/Core/DBConnection.hpp>
#include <ADBC/Core/PrototypeTables.hpp>
#include <ADBC/Query/Query.hpp>
#include <ADBC/Query/Operators.hpp>
#include <ADBC/Schema/Table.hpp>

auto main() -> std::int32_t
{
    std::cout << "Hello, ADBC" << std::endl; 

    try
    {
        ADBC::SQLite3Database db{ "Data/SomeDatabase.db" }; 

        auto users = std::vector<ADBC::UsersTable::Data>{};

        auto query = ADBC::Query{}
            .Select
            (
                ADBC::UsersTable::ID,
                ADBC::UsersTable::Name,
                ADBC::UsersTable::Email
            )
            .From("Users");

        ADBC::UsersTable::Entity.ID = 3;
        ADBC::UsersTable::Entity.Name = "SomeName";
        ADBC::UsersTable::Entity.Email = "SomeEmail@gmail.com";

        for (const auto& user : users)
        {
            std::cout << user.ID << ' ' << user.Name
                << ' ' << user.Email << '\n';
        };
    }
    catch (std::exception& exp)
    {
        std::cerr << exp.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
