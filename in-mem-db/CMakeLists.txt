set(CMAKE_CXX_STANDARD 23)

add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Werror)
add_compile_options(-O3)

add_executable(test test.cpp)