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

#ifndef oatpp_swagger_Controller_hpp
#define oatpp_swagger_Controller_hpp

#include "oatpp-swagger/ControllerPaths.hpp"
#include "oatpp-swagger/Resources.hpp"
#include "oatpp-swagger/Generator.hpp"

#include "oatpp/web/server/api/ApiController.hpp"

#include "oatpp/json/ObjectMapper.hpp"

#include "oatpp/web/protocol/http/outgoing/StreamingBody.hpp"
#include "oatpp/macro/codegen.hpp"
#include "oatpp/macro/component.hpp"

namespace oatpp { namespace swagger {

/**
 * Controller for Swagger-UI. Extends &id:oatpp::web::server::api::ApiController;. <br>
 * Should be used with oatpp Simple API and multithreaded server. <br>
 * For more information about integrating swagger-ui in oatpp application see [oatpp-swagger module](/docs/modules/oatpp-swagger/). <br>
 * Exposed endpoints:
 * <ul>
 *   <li>&id:ENDPOINT;("GET", "/api-docs/oas-3.0.0.json", api) - Server Open API Specification.</li>
 *   <li>&id:ENDPOINT;("GET", "/swagger/ui", getUIRoot) - Server Swagger-UI. (index.html).</li>
 *   <li>&id:ENDPOINT;("GET", "/swagger/{filename}", getUIResource) - Serve Swagger-UI resources.</li>
 * </ul>
 */
class Controller : public oatpp::web::server::api::ApiController {
private:
  oatpp::Object<oas3::Document> m_document;
  std::shared_ptr<oatpp::swagger::Resources> m_resources;
private:
  ControllerPaths m_paths;
public:
  Controller(const std::shared_ptr<ObjectMapper>& objectMapper,
             const oatpp::Object<oas3::Document>& document,
             const std::shared_ptr<oatpp::swagger::Resources>& resources,
             const ControllerPaths& paths)
    : oatpp::web::server::api::ApiController(objectMapper)
    , m_document(document)
    , m_resources(resources)
    , m_paths(paths)
  {}
public:

  /**
   * Create shared Controller.
   * @param endpointsList - &id:oatpp::web::server::api::Endpoints;
   * @param documentInfo - &id:oatpp::swagger::DocumentInfo;.
   * @param resources - &id:oatpp::swagger::Resources;.
   * @return - Controller.
   */
  static std::shared_ptr<Controller> createShared(const web::server::api::Endpoints& endpointsList,
                                                  OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, documentInfo),
                                                  OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, resources))
  {

    auto objectMapper = std::make_shared<oatpp::json::ObjectMapper>();
    objectMapper->serializerConfig().mapper.includeNullFields = false;
    objectMapper->deserializerConfig().mapper.allowUnknownFields = false;

    std::shared_ptr<Generator::Config> generatorConfig;
    try {
      generatorConfig = OATPP_GET_COMPONENT(std::shared_ptr<Generator::Config>);
    } catch (std::runtime_error&) {
      generatorConfig = std::make_shared<Generator::Config>();
    }

    Generator generator(generatorConfig);
    auto document = generator.generateDocument(documentInfo, endpointsList);

    ControllerPaths paths;
    try {
      auto ps = OATPP_GET_COMPONENT(std::shared_ptr<ControllerPaths>);
      if(ps) paths = *ps;
    } catch (std::runtime_error&) {
      // DO nothing.
    }

    return std::make_shared<Controller>(objectMapper, document, resources, paths);
  }
  
#include OATPP_CODEGEN_BEGIN(ApiController)
  
  ENDPOINT("GET", m_paths.apiJson, api) {
    return createDtoResponse(Status::CODE_200, m_document);
  }
  
  ENDPOINT("GET", m_paths.ui, getUIRoot) {
    return createResponse(Status::CODE_200, m_resources->getResourceData("index.html"));
  }

  ENDPOINT("GET", m_paths.initializer, getInitializer) {
    std::string ui = m_resources->getResourceData("swagger-initializer.js");
    ui.replace(ui.find("%%API.JSON%%"), 12, m_paths.apiJson);
    return createResponse(Status::CODE_200, ui);
  }
  
  ENDPOINT("GET", m_paths.uiResources, getUIResource, PATH(String, filename)) {
    auto body = std::make_shared<oatpp::web::protocol::http::outgoing::StreamingBody>(
      m_resources->getResource(filename)->openInputStream()
    );
    auto resp = OutgoingResponse::createShared(Status::CODE_200, body);
    resp->putHeader("Content-Type", m_resources->getMimeType(filename));
    return resp;
  }
  
#include OATPP_CODEGEN_END(ApiController)
  
};
  
}}

#endif /* oatpp_swagger_Controller_hpp */
