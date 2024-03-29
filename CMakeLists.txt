message("source dir is ${CMAKE_SOURCE_DIR}")
enable_testing()

set(CMAKE_CXX_STANDARD 14)
include_directories(.)
link_directories(.)

include_directories(./verifications)

# https://github.com/JuliaStrings/utf8proc
include_directories(${CMAKE_SOURCE_DIR}/../utf8proc)
link_directories(${CMAKE_SOURCE_DIR}/../utf8proc/build)
# https://github.com/google/googletest
include_directories(${CMAKE_SOURCE_DIR}/../googletest/googletest/include)
link_directories(${CMAKE_SOURCE_DIR}/../googletest/build/lib)
# https://github.com/intel/hyperscan.git
include_directories(${CMAKE_SOURCE_DIR}/../hyperscan/src)
link_directories(${CMAKE_SOURCE_DIR}/../hyperscan/lib)

set(VERIFICATIONS
    verifications/verification.cpp
    verifications/verificationfactory.cpp
    verifications/ukni.cpp)

add_library(dictionary_scanner ${VERIFICATIONS} dictionaryscanstate.cpp dictionaryscanner.cpp hsregexscanstate.cpp litregexengine.cpp hsbaseregexengine.cpp dictionaries.cpp dictionaryscanmatches.cpp dictionary.cpp dictionaryitemfactory.cpp hsregexengine.cpp regexengine.cpp dictionaryparser.cpp json11.cpp normalizer.cpp)
target_link_libraries(dictionary_scanner  libutf8proc.a)
target_compile_options(dictionary_scanner PUBLIC -Wall -Wextra -Wpedantic -Werror)

SET(TEST_LIBS libdictionary_scanner.a hs libutf8proc.a libgtest.a pthread)

add_executable(ut_dictionaryparser ut/ut_dictionaryparser.cpp gmain.cpp)
target_link_libraries(ut_dictionaryparser ${TEST_LIBS})

add_dependencies(ut_dictionaryparser dictionary_scanner)
add_test(NAME ut_dictionaryparser COMMAND ut_dictionaryparser)
 
add_executable(ut_itemfactory ut/ut_itemfactory.cpp gmain.cpp)
target_link_libraries(ut_itemfactory ${TEST_LIBS})

add_dependencies(ut_itemfactory dictionary_scanner)
add_test(NAME ut_itemfactory COMMAND ut_itemfactory)

add_executable(ut_dictionarytermid ut/ut_dictionarytermid.cpp gmain.cpp)
target_link_libraries(ut_dictionarytermid ${TEST_LIBS})

add_dependencies(ut_dictionarytermid dictionary_scanner)
add_test(NAME ut_dictionarytermid COMMAND ut_dictionarytermid)

add_executable(ut_dictionaryscanmatches ut/ut_dictionaryscanmatches.cpp gmain.cpp)
target_link_libraries(ut_dictionaryscanmatches ${TEST_LIBS})
add_dependencies(ut_dictionaryscanmatches dictionary_scanner)
add_test(NAME ut_dictionaryscanmatches COMMAND ut_dictionaryscanmatches)

add_executable(ut_dictionaryscanner ut/ut_dictionaryscanner.cpp gmain.cpp)
target_link_libraries(ut_dictionaryscanner ${TEST_LIBS})
add_dependencies(ut_dictionaryscanner dictionary_scanner)
add_test(NAME ut_dictionaryscanner COMMAND ut_dictionaryscanner)

add_executable(ut_dictionaryscanner_serialize ut/ut_dictionaryscanner_serialize.cpp gmain.cpp)
target_link_libraries(ut_dictionaryscanner_serialize ${TEST_LIBS})
add_dependencies(ut_dictionaryscanner_serialize dictionary_scanner)
add_test(NAME ut_dictionaryscanner_serialize COMMAND ut_dictionaryscanner_serialize)

add_executable(ut_normalizer ut/ut_normalizer.cpp gmain.cpp)
target_link_libraries(ut_normalizer ${TEST_LIBS})
add_dependencies(ut_normalizer dictionary_scanner)
add_test(NAME ut_normalizer COMMAND ut_normalizer)

add_executable(ut_verification ut/ut_verification.cpp gmain.cpp)
target_link_libraries(ut_verification ${TEST_LIBS})
add_dependencies(ut_verification dictionary_scanner)
add_test(NAME ut_verification COMMAND ut_verification)

add_executable(ut_dictionaryscanner_proximity ut/ut_dictionaryscanner_proximity.cpp gmain.cpp)
target_link_libraries(ut_dictionaryscanner_proximity ${TEST_LIBS})
add_dependencies(ut_dictionaryscanner_proximity dictionary_scanner)
add_test(NAME ut_dictionaryscanner_proximity COMMAND ut_dictionaryscanner_proximity)

add_executable(dictionaryscanner tools/main.cpp)
target_link_libraries(dictionaryscanner libdictionary_scanner.a utf8proc hs)
add_dependencies(dictionaryscanner dictionary_scanner)
add_test(NAME dictionaryscanner COMMAND dictionaryscanner)


