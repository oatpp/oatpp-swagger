
#include "TestController.hpp"

#include "Logger.hpp"

#include "oatpp-swagger/Controller.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp-test/UnitTest.hpp"


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

class Test : public oatpp::test::UnitTest {
public:
  Test() : oatpp::test::UnitTest("oatpp-swagger::Test")
  {}

  void onRun() override {

    // Create ObjectMapper
    auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();

    // Register swagger components
    SwaggerComponent swaggerComponent;

    // Create controllers and generate api docs
    auto docEndpoints = oatpp::swagger::Controller::Endpoints::createShared();

    auto controller = TestController::createShared(objectMapper);
    docEndpoints->pushBackAll(controller->getEndpoints());

    auto swaggerController = oatpp::swagger::Controller::createShared(docEndpoints);

    // Get api json from swaggerController
    auto stream = oatpp::data::stream::ChunkedBuffer::createShared();

    auto response = swaggerController->api();
    response->send(stream);

    std::cout << stream->toString()->c_str() << "\n\n";

    oatpp::parser::Caret caret(stream->toString());
    caret.findChar('{');

    auto document = objectMapper->readFromCaret<oatpp::swagger::oas3::Document>(caret);

    OATPP_ASSERT(caret.hasError() == false);

    // TODO test generated document here

  }
};

void runTests() {
  OATPP_RUN_TEST(Test);
}

}

int main() {

  oatpp::base::Environment::init();
  oatpp::base::Environment::setLogger(new Logger());

  runTests();

  oatpp::base::Environment::setLogger(nullptr);
  oatpp::base::Environment::destroy();

  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";

  OATPP_ASSERT(oatpp::base::Environment::getObjectsCount() == 0);

  oatpp::base::Environment::destroy();

  return 0;
}
