## define path to swagger-ui res folder

if (MSVC)
    # The MSVC compiler doesnt comply to c99 thus the OATPP_MACRO_HAS_ARGS macro always expands to 1
    # This screws up the DTO_FIELD and ENDPOINT macros. To force MSVC to c99 compliant mode we need to activate this
    # experimental switch. However, we can't enable it globally since the Windows-SDK isn't prepared for this and
    # throws a whole bunch of errors.
    add_compile_options(/experimental:preprocessor)
endif(MSVC)

add_definitions(-DOATPP_SWAGGER_RES_PATH="${CMAKE_CURRENT_LIST_DIR}/../res")

add_executable(module-tests
        oatpp-swagger/tests.cpp
        oatpp-swagger/test-controllers/TestController.hpp
        oatpp-swagger/test-controllers/TestAsyncController.hpp
        oatpp-swagger/ControllerTest.cpp
        oatpp-swagger/ControllerTest.hpp
        oatpp-swagger/AsyncControllerTest.cpp
        oatpp-swagger/AsyncControllerTest.hpp
)

set_target_properties(module-tests PROPERTIES
        CXX_STANDARD 11
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)

target_include_directories(module-tests
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

if(OATPP_MODULES_LOCATION STREQUAL OATPP_MODULES_LOCATION_EXTERNAL)
    add_dependencies(module-tests ${LIB_OATPP_EXTERNAL})
endif()

add_dependencies(module-tests ${OATPP_THIS_MODULE_NAME})

target_link_oatpp(module-tests)

target_link_libraries(module-tests
        PRIVATE ${OATPP_THIS_MODULE_NAME}
)

## TODO link dependencies here (if some)

add_test(module-tests module-tests)
