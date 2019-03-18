cmake_minimum_required(VERSION 3.5)

project(yamr VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(Threads)
find_package(Boost COMPONENTS filesystem system REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

add_executable(yamr main.cpp)

target_link_libraries(yamr
	${CMAKE_THREAD_LIBS_INIT}
	${Boost_LIBRARIES}
)

set_target_properties(yamr PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON
	COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;"
)

install(TARGETS yamr RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT sava_test@mail.ru)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include (CPack)
