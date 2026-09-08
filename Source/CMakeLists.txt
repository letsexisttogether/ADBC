set(ADBC_APP ADBC_APP)

add_executable(${ADBC_APP}
    Main.cpp)

target_compile_features(${ADBC_APP} PRIVATE cxx_std_23)

target_link_libraries(${ADBC_APP} PRIVATE SQLite::SQLite3)
