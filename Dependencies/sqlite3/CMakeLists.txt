include(FetchContent)

set(PackageName sqlite3)

FetchContent_Declare(
    ${PackageName}
    URL
        https://sqlite.org/2026/sqlite-amalgamation-3530400.zip
)

FetchContent_MakeAvailable(${PackageName})

set(PACKAGE_SOURCE_DIR ${sqlite3_SOURCE_DIR})

add_library(ADBC_SQLite STATIC ${PACKAGE_SOURCE_DIR}/sqlite3.c)

add_library(SQLite::SQLite3 ALIAS ADBC_SQLite)

target_include_directories(ADBC_SQLite PUBLIC ${PACKAGE_SOURCE_DIR})
