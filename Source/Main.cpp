#include <iostream>
#include <cstdint>

#include <ADBC/Core/DBConnection.hpp>
#include <ADBC/Core/PrototypeTables.hpp>
#include <ADBC/Query/Query.hpp>
#include <ADBC/Query/Operators.hpp>

auto main() -> std::int32_t
{
    std::cout << "Hello, ADBC" << std::endl;

    try
    {
        ADBC::SQLite3Database db{ "Data/SomeDatabase.db" }; 

        auto user = Users{};
        auto users = std::vector<Users>{};

        auto query = ADBC::Query{}
            .Select(ADBC::Col<"ID">(user.ID),
                ADBC::Col<"Name">(user.Name), ADBC::Col<"Email">(user.Email))
            .From(ASYS::SL{ "Users" })
            .Where(ADBC::OPS::NotEquals(ADBC::Col<"ID">(user.ID), 2));

        query.Execute(db, [&] (std::int32_t& id,
            std::string& name, std::string& email)
        {
            users.push_back(std::move(user));
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
