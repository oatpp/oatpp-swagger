//
// Created by Leonid on 2019-03-09.
//

#include "AsyncControllerTest.hpp"

#include "test-controllers/TestAsyncController.hpp"

#include "oatpp-swagger/AsyncController.hpp"
#include "oatpp/json/ObjectMapper.hpp"

#include <iostream>

namespace oatpp { namespace test { namespace swagger {

namespace {

  class SwaggerComponent {
  public:

    /**
     *  General API docs info
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, swaggerDocumentInfo)([] {

      oatpp::swagger::DocumentInfo::Builder builder;

      builder
        .setTitle("User entity service")
        .setDescription("CRUD API Example project with swagger docs")
        .setVersion("1.0")
        .setContactName("Ivan Ovsyanochka")
        .setContactUrl("https://oatpp.io/")

        .setLicenseName("Apache License, Version 2.0")
        .setLicenseUrl("http://www.apache.org/licenses/LICENSE-2.0")

        .addServer("http://localhost:8000", "server on localhost");

      return builder.build();

    }());


    /**
     *  Swagger-Ui Resources (<oatpp-examples>/lib/oatpp-swagger/res)
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, swaggerResources)([] {
      // Make sure to specify correct full path to oatpp-swagger/res folder !!!
      return oatpp::swagger::Resources::loadResources(OATPP_SWAGGER_RES_PATH);
    }());

  };

}

void AsyncControllerTest::onRun() {

  // Create ObjectMapper
  auto objectMapper = std::make_shared<oatpp::json::ObjectMapper>();

  // Register swagger components
  SwaggerComponent swaggerComponent;

  // Create controllers and generate api docs
  oatpp::web::server::api::Endpoints docEndpoints;

  auto controller = TestAsyncController::createShared(objectMapper);
  docEndpoints.append(controller->getEndpoints());

  auto swaggerController = oatpp::swagger::AsyncController::createShared(docEndpoints);

  // TODO test generated document here
  OATPP_LOGv(TAG, "TODO implement test");

}

}}}
