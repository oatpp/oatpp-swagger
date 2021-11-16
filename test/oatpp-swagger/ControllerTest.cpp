//
// Created by Leonid on 2019-03-09.
//

#include "ControllerTest.hpp"

#include "test-controllers/TestController.hpp"

#include "oatpp-swagger/Controller.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/data/stream/BufferStream.hpp"

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

        .addServer("http://localhost:8000", "server on localhost")

        .addSecurityScheme("basic_auth", oatpp::swagger::DocumentInfo::SecuritySchemeBuilder::DefaultBasicAuthorizationSecurityScheme());

      return builder.build();

    }());


    /**
     *  Swagger-Ui Resources (<oatpp-examples>/lib/oatpp-swagger/res)
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, swaggerResources)([] {
      // Make sure to specify correct full path to oatpp-swagger/res folder !!!
      return oatpp::swagger::Resources::streamResources(OATPP_SWAGGER_RES_PATH);
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Generator::Config>, generatorConfig)([] {
      auto config = std::make_shared<oatpp::swagger::Generator::Config>();
      config->enableInterpretations = {"test"};
      return config;
    }());

  };

}

void ControllerTest::onRun() {

  oatpp::data::stream::BufferOutputStream responseBuffer;

  // Create ObjectMapper
  auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();

  // Register swagger components
  SwaggerComponent swaggerComponent;

  // Create controllers and generate api docs
  oatpp::web::server::api::Endpoints docEndpoints;

  auto controller = TestController::createShared(objectMapper);
  docEndpoints.append(controller->getEndpoints());

  auto swaggerController = oatpp::swagger::Controller::createShared(docEndpoints);

  { // OAS-Doc

    // Get api json from swaggerController
    oatpp::data::stream::BufferOutputStream stream;
    stream.setOutputStreamIOMode(oatpp::data::stream::IOMode::BLOCKING);

    auto response = swaggerController->api();
    response->send(&stream, &responseBuffer, nullptr);

    std::cout << stream.toString()->c_str() << "\n\n";

    auto responseText = stream.toString();

    oatpp::parser::Caret caret(responseText);
    caret.findChar('{');

    auto document = objectMapper->readFromCaret<oatpp::Object<oatpp::swagger::oas3::Document>>(caret);

    if (caret.hasError()) {
      OATPP_LOGD(TAG, "error='%s', pos=%d", caret.getErrorMessage(), caret.getPosition());
    }
    OATPP_ASSERT(caret.hasError() == false);

  }

  { // index.html test
    // Get index.html from swaggerController
    oatpp::data::stream::BufferOutputStream stream;
    stream.setOutputStreamIOMode(oatpp::data::stream::IOMode::BLOCKING);

    auto response = swaggerController->getUIRoot();
    response->send(&stream, &responseBuffer, nullptr);

    auto responseText = stream.toString();
    OATPP_LOGD(TAG, responseText->c_str());

  }

  // TODO test generated document here
  OATPP_LOGV(TAG, "TODO implement test");

}

}}}