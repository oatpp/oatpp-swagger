add_executable(module-tests
        oatpp-swagger/tests.cpp
        )

set_target_properties(module-tests PROPERTIES
        CXX_STANDARD 11
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)

target_include_directories(module-tests
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

if(MODULE_WAIT_DEPS)
    add_dependencies(module-tests ${LIB_oatpp})
endif()

add_dependencies(module-tests ${OATPP_THIS_MODULE_NAME})

target_link_directories(module-tests
        PRIVATE ${OATPP_DIR_LIB}/src
)

target_link_libraries(module-tests
        PRIVATE oatpp
        PRIVATE oatpp-test
        PRIVATE ${OATPP_THIS_MODULE_NAME}
)

add_test(module-tests module-tests)