
add_library(${OATPP_THIS_MODULE_NAME}
        oatpp-swagger/AsyncController.hpp
        oatpp-swagger/Controller.hpp
        oatpp-swagger/Model.hpp
        oatpp-swagger/Resources.cpp
        oatpp-swagger/Resources.hpp
        oatpp-swagger/Types.cpp
        oatpp-swagger/Types.hpp
        oatpp-swagger/oas3/Generator.cpp
        oatpp-swagger/oas3/Generator.hpp
        oatpp-swagger/oas3/Model.hpp
)

if(MODULE_WAIT_DEPS)
    add_dependencies(${OATPP_THIS_MODULE_NAME} ${LIB_oatpp})
endif()

target_include_directories(${OATPP_THIS_MODULE_NAME}
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        PUBLIC ${OATPP_DIR_SRC}/src
)

target_link_directories(${OATPP_THIS_MODULE_NAME}
        PRIVATE ${OATPP_DIR_LIB}/src
)

target_link_libraries(${OATPP_THIS_MODULE_NAME}
        PRIVATE oatpp
        PRIVATE oatpp-test
)