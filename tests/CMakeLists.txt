set(UTILS_TESTS
  testPrime.cpp
  testBigInt.cpp
  testModInt.cpp
  testMoreMath.cpp
  testTreap.cpp
  testPrimeInt.cpp
)

enable_testing()

add_executable(utils_test ${UTILS_TESTS})
target_link_libraries(utils_test PRIVATE
  gtest_main
  utils 
  prime 
  primeint 
  bigint 
  modint 
  math
  treap
)


add_executable(exseq_test testExSeq.cpp)
target_link_libraries(exseq_test PRIVATE
  gtest_main
  A002326_lib
  A045345_lib
)

# ==========================================

add_test(NAME utils_test COMMAND utils_test)
add_test(NAME exseq_test COMMAND exseq_test)
