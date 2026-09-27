#include <iostream>
#include <cstdint>

#include <ADBC/Core/DBConnection.hpp>
#include <ADBC/Core/PrototypeTables.hpp>
#include <ADBC/Query/Builder.hpp>

auto main() -> std::int32_t
{
    std::cout << "Hello, ADBC" << std::endl;

    try
    {
        ADBC::SQLite3Database db{ "Data/SomeDatabase.db" }; 

        auto user = Users{};

        auto query = ADBC::Query{ "SELECT * FROM Users", std::ignore, std::ignore };

        auto anotherQuery = ADBC::Select(ADBC::Col<"ID">(user.ID),
            ADBC::Col<"Name">(user.Name), ADBC::Col<"Email">(user.Email));

        std::cout << query.GetText() << '\n'
            << anotherQuery.GetText() << std::endl;
    }
    catch (std::exception& exp)
    {
        std::cerr << exp.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
