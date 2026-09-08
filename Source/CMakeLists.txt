set(ADBCAPP ADBC_APP)

add_executable(${ADBCAPP}
    Main.cpp)

target_compile_features(${ADBCAPP}
    PRIVATE cxx_std_23)
