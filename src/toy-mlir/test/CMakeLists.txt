# 测试框架配置
enable_testing()
find_package(GTest REQUIRED)

# 添加测试子目录
add_subdirectory(ch1)

# 测试目标全局配置
target_include_directories(test_common INTERFACE
    ${GTEST_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)