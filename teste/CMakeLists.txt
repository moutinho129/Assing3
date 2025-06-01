cmake_minimum_required(VERSION 3.20.0)
project(test_runner)

add_executable(test_runner
    unitycmd.c
    mock_cmd.c
    mock_task.c
    mock_zephyr.c
    ../unity/unity.c
)

target_include_directories(test_runner PRIVATE
    .
    ../unity
)

target_compile_definitions(test_runner PRIVATE UNITY_INCLUDE_PRINT_FORMATTED UNIT_TEST)

