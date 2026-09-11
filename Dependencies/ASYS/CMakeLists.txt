include(FetchContent)

set(PACKAGE_NAME ASYS)

FetchContent_Declare(
    ${PACKAGE_NAME}
    GIT_REPOSITORY https://github.com/letsexisttogether/ASYS.git
)

FetchContent_MakeAvailable(${PACKAGE_NAME})
