set(UTILS_LIBS
    utils    Utils.cpp
    treap    Treap.cpp
    integer  Integer.cpp
    prime    PrimeFactorizer.cpp
    math     MoreMath.cpp
)

set(TEST_TARGETS
    treap tests/testTreap.cpp
    math  tests/testMath.cpp
    prime tests/testPrime.cpp
    integer tests/testInteger.cpp
)

# -------------------------------------------
# Libraries
# -------------------------------------------

list(LENGTH UTILS_LIBS _utils_len)
math(EXPR _utils_len "${_utils_len} - 1")

foreach(i RANGE 0 ${_utils_len} 2)

  math(EXPR j "${i} + 1")

  list(GET UTILS_LIBS ${i} libname)
  list(GET UTILS_LIBS ${j} src)

  add_library(${libname} STATIC ${src})

  target_include_directories(${libname} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}
  )
endforeach()


# -------------------------------------------
# GoogleTest
# -------------------------------------------

enable_testing()

list(LENGTH TEST_TARGETS _test_len)
math(EXPR _test_len "${_test_len} - 1")

foreach(i RANGE 0 ${_test_len} 2)

  math(EXPR j "${i} + 1")

  list(GET TEST_TARGETS ${i} libname)
  list(GET TEST_TARGETS ${j} testfile)

  set(testname ${libname}_tests)

  add_executable(${testname} ${testfile})
  target_link_libraries(${testname}
        PRIVATE ${libname} gtest_main
  )

  add_test(NAME ${testname} COMMAND ${testname})
endforeach()

set(ALL_TEST_NAMES)

foreach(i RANGE 0 ${_test_len} 2)
  math(EXPR j "${i} + 1")

  list(GET TEST_TARGETS ${i} libname)
  set(testname ${libname}_tests)
  list(APPEND ALL_TEST_NAMES ${testname})
endforeach()

add_custom_target(testall
  DEPENDS ${ALL_TEST_NAMES}
  COMMENT "Building all test executables"
)

add_custom_command(TARGET testall
  POST_BUILD
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
)
