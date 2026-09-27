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

        auto query = ADBC::Query{}
            .Select(ADBC::Col<"ID">(user.ID),
                ADBC::Col<"Name">(user.Name), ADBC::Col<"Email">(user.Email))
            .From(ASYS::SL{ "Users" });

        std::cout << query.GetText() << std::endl;
    }
    catch (std::exception& exp)
    {
        std::cerr << exp.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
