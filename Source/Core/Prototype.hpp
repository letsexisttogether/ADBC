#pragma once 

#include <iostream>

#include <ADBC/Core/DBConnection.hpp>

struct Users
{
    std::int32_t ID{};
    std::string Name{};
    std::string Email{};
};

inline auto OldMain(ADBC::SQLite3Database& db) -> void
{
    auto user = Users{};

    auto users = std::vector<Users>{};

    db.ExecuteRawQuery(ASYS::SL{ "SELECT * FROM Users WHERE ID != ?;" },
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
