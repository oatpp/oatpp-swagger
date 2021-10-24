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

#ifndef oatpp_swagger_Generator_hpp
#define oatpp_swagger_Generator_hpp

#include "oatpp-swagger/oas3/Model.hpp"

#include "oatpp/web/server/api/Endpoint.hpp"

#include <list>
#include <unordered_map>

namespace oatpp { namespace swagger {

/**
 * Generator of Open Api Specification 3.0.0.
 */
class Generator {
public:

  /**
   * Convenience typedef for &id:oatpp::web::server::api::Endpoint;.
   */
  typedef oatpp::web::server::api::Endpoint Endpoint;

  /**
   * Convenience typedef for list of &l:Generator::Endpoint;.
   */
  typedef oatpp::web::server::api::Endpoints Endpoints;

  typedef oatpp::List<Object<oas3::PathItemParameter>> PathItemParameters;
  
  typedef Fields<Object<oas3::PathItem>> Paths;
  
  typedef std::unordered_map<oatpp::String, const Type*> UsedTypes;

  typedef std::unordered_map<oatpp::String, bool> UsedSecuritySchemes;

private:
  void addParamsToParametersList(const PathItemParameters& paramsList,
                                        Endpoint::Info::Params& params,
                                        const oatpp::String& inType,
                                        UsedTypes& usedTypes);
private:

  oatpp::String getEnumSchemaName(const Type* type);

  oatpp::Object<oas3::Schema> generateSchemaForSimpleType(const Type* type, oatpp::BaseObject::Property* property = nullptr, const oatpp::Void& defaultValue = nullptr);
  oatpp::Object<oas3::Schema> generateSchemaForCollection_1D(const Type* type, bool linkSchema, UsedTypes& usedTypes, bool uniqueItems);
  oatpp::Object<oas3::Schema> generateSchemaForTypeObject(const Type* type, bool linkSchema, UsedTypes& usedTypes);
  oatpp::Object<oas3::Schema> generateSchemaForEnum(const Type* type, bool linkSchema, UsedTypes& usedTypes, oatpp::BaseObject::Property* property = nullptr);
  oatpp::Object<oas3::Schema> generateSchemaForAbstractPairList(const Type* type, bool linkSchema, UsedTypes& usedTypes, oatpp::BaseObject::Property* property = nullptr);
  oatpp::Object<oas3::Schema> generateSchemaForType(const Type* type, bool linkSchema, UsedTypes& usedTypes, oatpp::BaseObject::Property* property = nullptr, const oatpp::Void& defaultValue = nullptr);

  oatpp::Object<oas3::RequestBody> generateRequestBody(const Endpoint::Info& endpointInfo, bool linkSchema, UsedTypes& usedTypes);
  Fields<Object<oas3::OperationResponse>> generateResponses(const Endpoint::Info& endpointInfo, bool linkSchema, UsedTypes& usedTypes);
  void generatePathItemData(const std::shared_ptr<Endpoint>& endpoint, const oatpp::Object<oas3::PathItem>& pathItem, UsedTypes& usedTypes, UsedSecuritySchemes &usedSecuritySchemes);
  
  /*
   *  UsedTypes& usedTypes is used to put Types of objects whos schema should be reused
   */
  Paths generatePaths(const Endpoints& endpoints, UsedTypes& usedTypes, UsedSecuritySchemes &usedSecuritySchemes);

  oatpp::Object<oas3::SecurityScheme> generateSecurityScheme(const std::shared_ptr<oatpp::swagger::SecurityScheme> &ss);

  void decomposeObject(const Type* type, UsedTypes& decomposedTypes);
  void decomposeCollection_1D(const Type* type, UsedTypes& decomposedTypes);
  void decomposeMap(const Type* type, UsedTypes& decomposedTypes);
  void decomposeEnum(const Type* type, UsedTypes& decomposedTypes);
  void decomposeType(const Type* type, UsedTypes& decomposedTypes);
  UsedTypes decomposeTypes(UsedTypes& usedTypes);
  
  oatpp::Object<oas3::Components> generateComponents(const UsedTypes &decomposedTypes,
                                                     const std::shared_ptr<std::unordered_map<oatpp::String,std::shared_ptr<oatpp::swagger::SecurityScheme>>> &securitySchemes,
                                                     UsedSecuritySchemes &usedSecuritySchemes);

public:

  struct Config {

    /**
     * Enable type interpretations.
     */
    std::vector<std::string> enableInterpretations = {};

  };

private:
  std::shared_ptr<Config> m_config;
public:

  Generator(const std::shared_ptr<Config>& config);

  /**
   * Generate Open Api Specification 3.0.0. from &l:Generator::Endpoints;.
   * @param docInfo - &id:oatpp::swagger::DocumentInfo;.
   * @param endpoints &l:Generator::Endpoints;.
   * @return - &id:oatpp::swagger::oas3::Document;.
   */
  oatpp::Object<oas3::Document> generateDocument(const std::shared_ptr<oatpp::swagger::DocumentInfo>& docInfo, const Endpoints& endpoints);

};
  
}}

#endif /* oatpp_swagger_Generator_hpp */
