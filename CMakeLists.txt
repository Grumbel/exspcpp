# exsp - Spawning subprocesses in C++
# Copyright (C) 2008-2020 Ingo Ruhnke <grumbel@gmail.com>
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or (at your
# option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
# License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.6)
project(exsp VERSION 0.0.0)

include(mk/cmake/TinyCMMC.cmake)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)

file(GLOB EXSP_HEADER_SOURCES "include/exsp/*.hpp")
file(GLOB EXSP_LIBRARY_SOURCES "src/*.cpp")
file(GLOB TEST_EXSP_SOURCES test/*_test.cpp)

add_library(exsp ${EXSP_LIBRARY_SOURCES})
target_include_directories(exsp PRIVATE src/)
target_include_directories(exsp PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/exsp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
set_target_properties(exsp PROPERTIES
  PUBLIC_HEADER "${EXSP_HEADER_SOURCES}")

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/exsp.pc.in"
  "${CMAKE_CURRENT_BINARY_DIR}/exsp.pc"
  @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/exsp.pc"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

install(TARGETS exsp
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/exsp/")

if(BUILD_TESTS)
  enable_testing()
  find_package(GTest REQUIRED)

  # build automatic tests
  add_executable(test_exsp ${TEST_EXSP_SOURCES})
  target_compile_options(test_exsp PRIVATE ${TINYCMMC_WARNINGS_CXX_FLAGS})
  target_link_libraries(test_exsp
    GTest::GTest
    GTest::Main
    exsp)

  set(ARGV "-V")
  add_test(NAME test_exsp
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND test_exsp)
endif()

tinycmmc_export_and_install_library(exsp)

# EOF #
