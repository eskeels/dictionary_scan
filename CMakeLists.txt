message("source dir is ${CMAKE_SOURCE_DIR}")
include_directories(${CMAKE_SOURCE_DIR}/../googletest/googletest/include)
link_directories(${CMAKE_SOURCE_DIR}/../googletest/build/lib)

add_executable(ut_dictionaryparser ut/ut_dictionaryparser.cpp dictionaryparser.cpp dictionaryitemfactory.cpp dictionary.cpp dictionaries.cpp json11.cpp gmain.cpp)
target_link_libraries(ut_dictionaryparser libgtest.a pthread)


add_executable(ut_itemfactory ut/ut_itemfactory.cpp dictionaryitemfactory.cpp gmain.cpp)
target_link_libraries(ut_itemfactory libgtest.a pthread)

add_executable(ut_dictionarytermid ut/ut_dictionarytermid.cpp gmain.cpp)
target_link_libraries(ut_dictionarytermid libgtest.a pthread)

add_executable(ut_dictionaryscanmatches ut/ut_dictionaryscanmatches.cpp dictionaryitemfactory.cpp dictionary.cpp dictionaries.cpp dictionaryscanmatches.cpp gmain.cpp)
target_link_libraries(ut_dictionaryscanmatches libgtest.a pthread)

add_executable(ut_dictionaryscanner ut/ut_dictionaryscanner.cpp dictionaryscanstate.cpp dictionaryscanner.cpp hsregexscanstate.cpp litregexengine.cpp hsbaseregexengine.cpp dictionaries.cpp dictionaryscanmatches.cpp dictionary.cpp dictionaryitemfactory.cpp hsregexengine.cpp regexengine.cpp gmain.cpp)
target_link_libraries(ut_dictionaryscanner libgtest.a pthread hs)

target_compile_options(ut_dictionaryscanner PRIVATE -Wall -Wextra -Wpedantic -Werror)

add_executable(simplegrep2 simplegrep.cpp dictionaryscanmatches.cpp dictionaryscanstate.cpp dictionaryscanner.cpp hsregexscanstate.cpp litregexengine.cpp hsbaseregexengine.cpp hsregexengine.cpp regexengine.cpp dictionaryitemfactory.cpp dictionary.cpp dictionaries.cpp)
set_source_files_properties(dictionary.cpp dictionaryitemfactory.cpp simplegrep.cpp PROPERTIES COMPILE_FLAGS
    "-Wall -Wno-unused-parameter")
target_link_libraries(simplegrep2 hs)

