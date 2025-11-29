add_library(utils STATIC
  Utils.cpp
)

add_library(treap STATIC
  Treap.cpp
)

add_library(integer STATIC 
  Integer.cpp
)

add_library(prime STATIC 
  PrimeFactorizer.cpp
)

add_library(math STATIC 
  MoreMath.cpp
)

target_include_directories(utils PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
)

target_include_directories(treap PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
)

target_include_directories(integer PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
)

target_include_directories(prime PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
)

target_include_directories(math PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
)
