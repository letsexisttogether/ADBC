#include <iostream>
#include <cstdint>

#include <ADBC/Core/DBConnection.hpp>
#include <ADBC/Core/PrototypeTables.hpp>
#include <ADBC/Query/Query.hpp>
#include <ADBC/Query/Operators.hpp>
#include <ADBC/Schema/Table.hpp>
#include <type_traits>

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

        ADBC::UsersTable::Entity.ID = 3;
        ADBC::UsersTable::Entity.Name = "SomeName";
        ADBC::UsersTable::Entity.Email = "SomeEmail@gmail.com";

        std::apply([] (auto&& ... column)
        {
            auto ColumnA = [] (auto&& column)
            {
                using ColumnType = std::remove_cvref_t<decltype(column)>;
                using ColumnValueType = std::remove_cvref_t<decltype(column.Value)>;

                if constexpr (std::is_same_v<ColumnValueType, std::int32_t>)
                {
                    column.Value = 300;
                }
                else
                {
                    column.Value = "Hello";
                }

                if (std::is_same_v<std::remove_reference_t
                    <decltype(column.Value)>, ColumnValueType>)
                {
                    std::cout << "It's possible to chagne the value\n";
                }

                std::cout << "Size: " << sizeof(column) << '\n';

                std::cout << ColumnType::Name << ' ' << column.Value << '\n';
            };

            ((ColumnA(std::forward<decltype(column)>(column)), ...));
        }, query.GetOutputPack());

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
