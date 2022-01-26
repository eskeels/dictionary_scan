message("source dir is ${CMAKE_SOURCE_DIR}")
enable_testing()

set(CMAKE_CXX_STANDARD 14)
include_directories(.)
link_directories(.)

include_directories(${CMAKE_SOURCE_DIR}/../googletest/googletest/include)
link_directories(${CMAKE_SOURCE_DIR}/../googletest/build/lib)

include_directories(${CMAKE_SOURCE_DIR}/../hyperscan/src)
link_directories(${CMAKE_SOURCE_DIR}/../hyperscan/lib)

add_library(dictionary_scanner dictionaryscanstate.cpp dictionaryscanner.cpp hsregexscanstate.cpp litregexengine.cpp hsbaseregexengine.cpp dictionaries.cpp dictionaryscanmatches.cpp dictionary.cpp dictionaryitemfactory.cpp hsregexengine.cpp regexengine.cpp dictionaryparser.cpp json11.cpp)

target_compile_options(dictionary_scanner PRIVATE -Wall -Wextra -Wpedantic -Werror)

add_executable(ut_dictionaryparser ut/ut_dictionaryparser.cpp gmain.cpp)
target_link_libraries(ut_dictionaryparser libdictionary_scanner.a libgtest.a pthread)
add_dependencies(ut_dictionaryparser dictionary_scanner)
add_test(NAME ut_dictionaryparser COMMAND ut_dictionaryparser)
 
add_executable(ut_itemfactory ut/ut_itemfactory.cpp gmain.cpp)
target_link_libraries(ut_itemfactory libdictionary_scanner.a libgtest.a pthread)
add_dependencies(ut_itemfactory dictionary_scanner)
add_test(NAME ut_itemfactory COMMAND ut_itemfactory)

add_executable(ut_dictionarytermid ut/ut_dictionarytermid.cpp gmain.cpp)
target_link_libraries(ut_dictionarytermid libdictionary_scanner.a libgtest.a pthread)
add_dependencies(ut_dictionarytermid dictionary_scanner)
add_test(NAME ut_dictionarytermid COMMAND ut_dictionarytermid)

add_executable(ut_dictionaryscanmatches ut/ut_dictionaryscanmatches.cpp gmain.cpp)
target_link_libraries(ut_dictionaryscanmatches libdictionary_scanner.a libgtest.a pthread)
add_dependencies(ut_dictionaryscanmatches dictionary_scanner)
add_test(NAME ut_dictionaryscanmatches COMMAND ut_dictionaryscanmatches)

add_executable(ut_dictionaryscanner ut/ut_dictionaryscanner.cpp gmain.cpp)
target_link_libraries(ut_dictionaryscanner libdictionary_scanner.a libgtest.a pthread hs)
add_dependencies(ut_dictionaryscanner dictionary_scanner)
add_test(NAME ut_dictionaryscanner COMMAND ut_dictionaryscanner)


