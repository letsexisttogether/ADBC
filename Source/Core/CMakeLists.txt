target_sources(${ADBC_LIB}
    PUBLIC 
        DBConnection.cpp
)

file(CREATE_LINK
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ADBC_INCLUDE_DIR}/ADBC/Core
    SYMBOLIC
)
