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
#include "oatpp-swagger/oas3/Generator.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/web/server/HttpError.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

namespace oatpp { namespace swagger {
  
class AsyncController : public oatpp::web::server::api::ApiController {
public:
  typedef AsyncController __ControllerType;
public:
  oas3::Document::ObjectWrapper m_document;
  std::shared_ptr<oatpp::swagger::Resources> m_resources;
protected:
  AsyncController(const std::shared_ptr<ObjectMapper>& objectMapper,
             const oas3::Document::ObjectWrapper& document,
             const std::shared_ptr<oatpp::swagger::Resources>& resources)
    : oatpp::web::server::api::ApiController(objectMapper)
    , m_document(document)
    , m_resources(resources)
  {}
public:
  
  static std::shared_ptr<AsyncController> createShared(const std::shared_ptr<Endpoints>& endpointsList,
                                                  OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, documentInfo),
                                                  OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, resources)){
    
    auto serializerConfig = oatpp::parser::json::mapping::Serializer::Config::createShared();
    serializerConfig->includeNullFields = false;
    
    auto deserializerConfig = oatpp::parser::json::mapping::Deserializer::Config::createShared();
    deserializerConfig->allowUnknownFields = false;
    
    auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared(serializerConfig, deserializerConfig);
    
    auto document = oas3::Generator::generateDocument(documentInfo, endpointsList);
    
    return std::make_shared<AsyncController>(objectMapper, document, resources);
  }
  
#include OATPP_CODEGEN_BEGIN(ApiController)
  
  ENDPOINT_ASYNC("GET", "/api-docs/oas-3.0.0.json", ApiDocs) {
    
    ENDPOINT_ASYNC_INIT(ApiDocs)
    
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
