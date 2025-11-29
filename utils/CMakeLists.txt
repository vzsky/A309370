add_library(utils STATIC
  main.cpp
)

add_library(integer STATIC 
  Integer.cpp
)

add_library(prime STATIC 
  PrimeFactorizer.cpp
)

target_include_directories(utils PUBLIC
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
