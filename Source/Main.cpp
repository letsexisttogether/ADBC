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
        
        /*
        auto query = ADBC::Query{}
            .Select(ASYS::SL{ "ID" }, ASYS::SL{ "Name" },
                ASYS::SL{ "Email" })
            .From(ASYS::SL{ "Users" })
            .Where()
            .Operator("AND", "!=", "ID");
        */

        /*
            // strings for now, later also columns
            auto query = Select("ID", "Name", "Email", user.ID, user.Name, user.Email)
                .From("Users")
                .Where("ID = 3")
                .And("Name != 'SomeName'")
                .AsExecutable();
            query.Execute(db, lambda for rows interpretation);
         
            // later:
            auto query = Select(Users::ID, Users::Name, Users::Email)
                .From(Users)
                .Where(Users::ID == 3)
                .And(Users::Name != "SomeName")
                .AsExecutable();'
            query.Execute(db, lambda for rows interpretation);

        */

        auto ID = ADBC::Col<ASYS::SL{ "ID", }, std::int32_t>();
        auto Name = ADBC::Col<ASYS::SL{ "Name", }>(std::string{ "Something" });

        auto email = std::string{};
        auto Email = ADBC::Col<ASYS::SL{ "Email" }>(email);

        Email.Value = "SomeEmail@gmail.com";
        std::cout << Name.Value << ' ' << email << std::endl;

        auto query = ADBC::Select(ID, Name, Email);

        std::cout << query << std::endl;;
    }
    catch (std::exception& exp)
    {
        std::cerr << exp.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
