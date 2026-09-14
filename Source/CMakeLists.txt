set(ADBC_LIB ADBC_LIB)

add_library(${ADBC_LIB} STATIC)
add_library(ADBC ALIAS ${ADBC_LIB})

target_compile_features(${ADBC_LIB} PUBLIC cxx_std_23)

add_subdirectory(Core)

target_include_directories(${ADBC_LIB} 
    PUBLIC
        $<BUILD_INTERFACE:${ADBC_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(${ADBC_LIB} 
    PUBLIC 
        SQLite::SQLite3
        ASYS::String
)

target_include_directories(${ADBC_LIB} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set(ADBC_APP ADBC_APP)

add_executable(${ADBC_APP} Main.cpp)

target_compile_features(${ADBC_APP} PUBLIC cxx_std_23)

target_link_libraries(${ADBC_APP} 
    PUBLIC 
        ADBC
)
