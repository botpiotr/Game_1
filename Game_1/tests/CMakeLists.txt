# CMakeLists.txt du sous-dossier tests/
# Appelé depuis le CMakeLists.txt principal via add_subdirectory(tests)

find_package(Qt6 REQUIRED COMPONENTS Test)

add_executable(tests_game1
    test_basic.cpp
)
target_link_libraries(tests_game1 PRIVATE Qt6::Test Qt6::Widgets)

add_test(NAME tests_game1 COMMAND tests_game1)
