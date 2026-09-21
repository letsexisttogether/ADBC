#include <iostream>
#include <cstdint>

// #include "Core/DBConnection.hpp"

#include <ADBC/Core/DBConnection.hpp>
#include <ADBC/Core/Builder.hpp>
#include <ADBC/Core/Prototype.hpp>

auto main() -> std::int32_t
{
    std::cout << "Hello, ADBC" << std::endl;

    try
    {
        ADBC::SQLite3Database db{ "Data/SomeDatabase.db" }; 

        auto query = ADBC::Query{}
            .Select(ASYS::SL{ "ID" }, ASYS::SL{ "Name" },
                ASYS::SL{ "Email" })
            .From(ASYS::SL{ "Users" })
            .Where()
            .Operator("AND", "!=", "ID");

        std::cout << static_cast<std::string&>(query).c_str() << std::endl;

        auto user = Users{};
        auto users = std::vector<Users>{};

        db.ExecuteRawQuery(query,
            ADBC::CreateSQLOutputs(user.ID, user.Name, user.Email), 
            ADBC::CreateSQLParams(3),
            [&] (std::int32_t& id, std::string& name, std::string& email)
        {
            users.push_back(user);
        });

        for (const auto& [id, name, email] : users)
        {
            std::cout << id << ' ' << name << ' '
                << email << std::endl;
        }
    }
    catch (std::exception& exp)
    {
        std::cerr << exp.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
