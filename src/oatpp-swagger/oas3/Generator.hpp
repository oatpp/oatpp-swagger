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

#ifndef oatpp_swagger_oas3_Generator_hpp
#define oatpp_swagger_oas3_Generator_hpp

#include "oatpp-swagger/oas3/Model.hpp"

#include "oatpp/web/server/api/Endpoint.hpp"
#include "oatpp/core/collection/LinkedList.hpp"

#include <unordered_map>

namespace oatpp { namespace swagger { namespace oas3 {

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
  typedef oatpp::collection::LinkedList<std::shared_ptr<Endpoint>> Endpoints;

  typedef oatpp::data::mapping::type::List<PathItemParameter::ObjectWrapper> PathItemParameters;
  
  template <class Value>
  using Fields = oatpp::data::mapping::type::ListMap<String, Value>;
  
  typedef Fields<PathItem::ObjectWrapper> Paths;
  
  typedef std::unordered_map<oatpp::String, const oatpp::data::mapping::type::Type*> UsedTypes;

  typedef std::unordered_map<oatpp::String, bool> UsedSecuritySchemes;

private:
  static void addParamsToParametersList(const PathItemParameters::ObjectWrapper& paramsList,
                                        Endpoint::Info::Params& params,
                                        const oatpp::String& inType,
                                        UsedTypes& usedTypes);
private:

  static Schema::ObjectWrapper generateSchemaForTypeList(const oatpp::data::mapping::type::Type* type, bool linkSchema, UsedTypes& usedTypes);
  static Schema::ObjectWrapper generateSchemaForTypeObject(const oatpp::data::mapping::type::Type* type, bool linkSchema, UsedTypes& usedTypes);
  static Schema::ObjectWrapper generateSchemaForType(const oatpp::data::mapping::type::Type* type, bool linkSchema, UsedTypes& usedTypes);

  static RequestBody::ObjectWrapper generateRequestBody(const Endpoint::Info& endpointInfo, bool linkSchema, UsedTypes& usedTypes);
  static Fields<OperationResponse::ObjectWrapper>::ObjectWrapper generateResponses(const Endpoint::Info& endpointInfo, bool linkSchema, UsedTypes& usedTypes);
  static void generatePathItemData(const std::shared_ptr<Endpoint>& endpoint, const PathItem::ObjectWrapper& pathItem, UsedTypes& usedTypes, UsedSecuritySchemes &usedSecuritySchemes);
  
  /*
   *  UsedTypes& usedTypes is used to put Types of objects whos schema should be reused
   */
  static Paths::ObjectWrapper generatePaths(const std::shared_ptr<Endpoints>& endpoints, UsedTypes& usedTypes, UsedSecuritySchemes &usedSecuritySchemes);

  static SecurityScheme::ObjectWrapper generateSecurityScheme(const std::shared_ptr<oatpp::swagger::SecurityScheme> &ss);

  static void decomposeObject(const oatpp::data::mapping::type::Type* type, UsedTypes& decomposedTypes);
  static void decomposeList(const oatpp::data::mapping::type::Type* type, UsedTypes& decomposedTypes);
  static void decomposeMap(const oatpp::data::mapping::type::Type* type, UsedTypes& decomposedTypes);
  static void decomposeType(const oatpp::data::mapping::type::Type* type, UsedTypes& decomposedTypes);
  static UsedTypes decomposeTypes(UsedTypes& usedTypes);
  
  static Components::ObjectWrapper generateComponents(const UsedTypes &decomposedTypes,
                                                      const std::shared_ptr<std::unordered_map<oatpp::String,std::shared_ptr<oatpp::swagger::SecurityScheme>>> &securitySchemes,
                                                      UsedSecuritySchemes &usedSecuritySchemes);

public:

  /**
   * Generate Open Api Specification 3.0.0. from &l:Generator::Endpoints;.
   * @param docInfo - &id:oatpp::swagger::DocumentInfo;.
   * @param endpoints &l:Generator::Endpoints;.
   * @return - &id:oatpp::swagger::oas3::Document;.
   */
  static Document::ObjectWrapper generateDocument(const std::shared_ptr<oatpp::swagger::DocumentInfo>& docInfo, const std::shared_ptr<Endpoints>& endpoints);

};
  
}}}

#endif /* oatpp_swagger_oas3_Generator_hpp */
