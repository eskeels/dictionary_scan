message("source dir is ${CMAKE_SOURCE_DIR}")
include_directories(${CMAKE_SOURCE_DIR}/../googletest/googletest/include)
link_directories(${CMAKE_SOURCE_DIR}/../googletest/build/lib)

add_executable(ut_itemfactory ut/ut_itemfactory.cpp dictionaryitemfactory.cpp gmain.cpp)
target_link_libraries(ut_itemfactory libgtest.a pthread)

add_executable(ut_dictionarytermid ut/ut_dictionarytermid.cpp gmain.cpp)
target_link_libraries(ut_dictionarytermid libgtest.a pthread)


add_executable(simplegrep2 simplegrep.cpp regexengine.cpp dictionaryitemfactory.cpp dictionary.cpp dictionaries.cpp)
set_source_files_properties(dictionary.cpp dictionaryitemfactory.cpp simplegrep.cpp PROPERTIES COMPILE_FLAGS
    "-Wall -Wno-unused-parameter")
target_link_libraries(simplegrep2 hs)
