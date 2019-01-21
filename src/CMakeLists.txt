
add_library(oatpp-swagger
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

target_include_directories(oatpp-swagger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}) ##${DIR_SRC_OATPP}/src)
if(BUILD_SHARED_LIBS)
    find_library(LIB_OATPP oatpp HINTS ${DIR_LIB_OATPP})
    target_link_libraries(oatpp-swagger PUBLIC ${LIB_OATPP})
endif()