#include <iostream>
#include <cstdint>

#include "Core/DBConnection.hpp"

auto main() -> std::int32_t
{
    std::cout << "Hello, ADBC" << std::endl;

    try
    {
        ADBC::SQLite3Database db{ "Data/SomeDatabase.db" }; 


        struct Users
        {
            std::int32_t ID{};
            std::string Name{};
            std::string Email{};
        };
        auto user = Users{};

        auto users = std::vector<Users>{};

        db.ExecuteRawQuery(ASYS::SL{ "SELECT * FROM Users;" }, 
            [&] (std::int32_t& id, std::string& name, std::string& email)
        {
            users.push_back(user);
        }, user.ID, user.Name, user.Email);

        for (const auto& [id, name, email] : users)
        {
            std::cout << id << ' ' << name << ' '
                << email << std::endl;
        }
    }
    catch (std::exception& exp)
    {
        std::cerr << exp.what();
    }

    return EXIT_SUCCESS;
}
