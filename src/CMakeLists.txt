
add_library(${OATPP_THIS_MODULE_NAME}
        oatpp-swagger/AsyncController.hpp
        oatpp-swagger/Controller.hpp
        oatpp-swagger/ControllerPaths.hpp
        oatpp-swagger/Generator.cpp
        oatpp-swagger/Generator.hpp
        oatpp-swagger/Model.hpp
        oatpp-swagger/Resources.cpp
        oatpp-swagger/Resources.hpp
        oatpp-swagger/Types.cpp
        oatpp-swagger/Types.hpp
        oatpp-swagger/oas3/Model.hpp)

set_target_properties(${OATPP_THIS_MODULE_NAME} PROPERTIES
        CXX_STANDARD 11
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)

if(OATPP_MODULES_LOCATION STREQUAL OATPP_MODULES_LOCATION_EXTERNAL)
    add_dependencies(${OATPP_THIS_MODULE_NAME} ${LIB_OATPP_EXTERNAL})
endif()

target_link_oatpp(${OATPP_THIS_MODULE_NAME})

target_include_directories(${OATPP_THIS_MODULE_NAME}
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

## TODO link dependencies here (if some)

#######################################################################################################
## install targets

if(OATPP_INSTALL)
    include("../cmake/module-install.cmake")

    ## install swagger-ui files
    install(DIRECTORY ../res
            DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/oatpp-${OATPP_MODULE_VERSION}/bin/${OATPP_MODULE_NAME}"
            FILES_MATCHING PATTERN "*.*"
    )

    ## set environment variable to point to swagger-ui res files
    set(ENV{OATPP_SWAGGER_RES_PATH} "${CMAKE_INSTALL_INCLUDEDIR}/oatpp-${OATPP_MODULE_VERSION}/bin/${OATPP_MODULE_NAME}/res")

endif()
