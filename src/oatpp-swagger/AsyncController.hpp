/***************************************************************************
 *
 * Project         _____    __   ____   _      _
 *                (  _  )  /__\ (_  _)_| |_  _| |_
 *                 )(_)(  /(__)\  )( (_   _)(_   _)
 *                (_____)(__)(__)(__)  |_|    |_|
 *
 *
 * Copyright 2018-present, Leonid Stryzhevskyi, <lganzzzo@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************************/

#ifndef oatpp_swagger_AsyncController_hpp
#define oatpp_swagger_AsyncController_hpp

#include "oatpp-swagger/Resources.hpp"
#include "oatpp-swagger/Generator.hpp"

#include "oatpp/web/server/api/ApiController.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

namespace oatpp { namespace swagger {

/**
 * Asynchronous Controller for Swagger-UI. Extends &id:oatpp::web::server::api::ApiController;. <br>
 * Should be used with oatpp Async API and non-blocking I/O. <br>
 * For more information about integrating swagger-ui in oatpp application see [oatpp-swagger module](/docs/modules/oatpp-swagger/). <br>
 * Exposed endpoints:
 * <ul>
 *   <li>&id:ENDPOINT_ASYNC;("GET", "/api-docs/oas-3.0.0.json", Api) - Server Open API Specification.</li>
 *   <li>&id:ENDPOINT_ASYNC;("GET", "/swagger/ui", GetUIRoot) - Server Swagger-UI. (index.html).</li>
 *   <li>&id:ENDPOINT_ASYNC;("GET", "/swagger/{filename}", GetUIResource) - Serve Swagger-UI resources.</li>
 * </ul>
 */
class AsyncController : public oatpp::web::server::api::ApiController {
public:
  typedef AsyncController __ControllerType;
public:
  oatpp::Object<oas3::Document> m_document;
  std::shared_ptr<oatpp::swagger::Resources> m_resources;
public:
  AsyncController(const std::shared_ptr<ObjectMapper>& objectMapper,
             const oatpp::Object<oas3::Document>& document,
             const std::shared_ptr<oatpp::swagger::Resources>& resources)
    : oatpp::web::server::api::ApiController(objectMapper)
    , m_document(document)
    , m_resources(resources)
  {}
public:

  /**
   * Create shared AsyncController.
   * @param endpointsList - &id:oatpp::web::server::api::Endpoints;
   * @param documentInfo - &id:oatpp::swagger::DocumentInfo;.
   * @param resources - &id:oatpp::swagger::Resources;.
   * @return - AsyncController.
   */
  static std::shared_ptr<AsyncController> createShared(const web::server::api::Endpoints& endpointsList,
                                                       OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, documentInfo),
                                                       OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, resources)){
    
    auto serializerConfig = oatpp::parser::json::mapping::Serializer::Config::createShared();
    serializerConfig->includeNullFields = false;
    
    auto deserializerConfig = oatpp::parser::json::mapping::Deserializer::Config::createShared();
    deserializerConfig->allowUnknownFields = false;
    
    auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared(serializerConfig, deserializerConfig);

    std::shared_ptr<Generator::Config> generatorConfig;
    try {
      generatorConfig = OATPP_GET_COMPONENT(std::shared_ptr<Generator::Config>);
    } catch (std::runtime_error e) {
      generatorConfig = std::make_shared<Generator::Config>();
    }

    Generator generator(generatorConfig);
    auto document = generator.generateDocument(documentInfo, endpointsList);
    
    return std::make_shared<AsyncController>(objectMapper, document, resources);
  }
  
#include OATPP_CODEGEN_BEGIN(ApiController)
  
  ENDPOINT_ASYNC("GET", "/api-docs/oas-3.0.0.json", Api) {
    
    ENDPOINT_ASYNC_INIT(Api)
    
    Action act() override {
      return _return(controller->createDtoResponse(Status::CODE_200, controller->m_document));
    }
    
  };
  
  ENDPOINT_ASYNC("GET", "/swagger/ui", GetUIRoot) {
    
    ENDPOINT_ASYNC_INIT(GetUIRoot)
    
    Action act() override {
      return _return(controller->createResponse(Status::CODE_200, controller->m_resources->getResource("index.html")));
    }
    
  };
  
  ENDPOINT_ASYNC("GET", "/swagger/{filename}", GetUIResource) {
    
    ENDPOINT_ASYNC_INIT(GetUIResource)
    
    Action act() override {
      auto filename = request->getPathVariable("filename");
      OATPP_ASSERT_HTTP(filename, Status::CODE_400, "filename should not be null")
      return _return(controller->createResponse(Status::CODE_200, controller->m_resources->getResource(filename->c_str())));
    }
    
  };
  
#include OATPP_CODEGEN_END(ApiController)
  
};
  
}}

#endif /* oatpp_swagger_AsyncController_hpp */
