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
            .From(ASYS::SL{ "Users" });

        query.Execute(db, [&] (std::int32_t& id,
            std::string& name, std::string& email)
        {
            users.push_back(ADBC::UsersTable::Entity);
        });

        for (const auto& user : users)
        {
            std::cout << user.ID << ' ' << user.Name
                << ' ' << user.Email << '\n';
        };

        std::cout << query.GetText() << std::endl;
    }
    catch (std::exception& exp)
    {
        std::cerr << exp.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
