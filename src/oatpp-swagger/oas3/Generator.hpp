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

  typedef oatpp::data::mapping::type::Type Type;

  /**
   * Convenience typedef for &id:oatpp::web::server::api::Endpoint;.
   */
  typedef oatpp::web::server::api::Endpoint Endpoint;

  /**
   * Convenience typedef for list of &l:Generator::Endpoint;.
   */
  typedef oatpp::collection::LinkedList<std::shared_ptr<Endpoint>> Endpoints;

  typedef oatpp::List<Object<PathItemParameter>> PathItemParameters;
  
  typedef Fields<Object<PathItem>> Paths;
  
  typedef std::unordered_map<oatpp::String, const Type*> UsedTypes;

  typedef std::unordered_map<oatpp::String, bool> UsedSecuritySchemes;

private:
  static void addParamsToParametersList(const PathItemParameters& paramsList,
                                        Endpoint::Info::Params& params,
                                        const oatpp::String& inType,
                                        UsedTypes& usedTypes);
private:

  static oatpp::String getEnumSchemaName(const Type* type);

  static oatpp::Object<Schema> generateSchemaForSimpleType(const Type* type, Type::Property* property = nullptr, const oatpp::Void& defaultValue = nullptr);
  static oatpp::Object<Schema> generateSchemaForCollection_1D(const Type* type, bool linkSchema, UsedTypes& usedTypes, bool uniqueItems);
  static oatpp::Object<Schema> generateSchemaForTypeObject(const Type* type, bool linkSchema, UsedTypes& usedTypes);
  static oatpp::Object<Schema> generateSchemaForEnum(const Type* type, bool linkSchema, UsedTypes& usedTypes, Type::Property* property = nullptr);
  static oatpp::Object<Schema> generateSchemaForType(const Type* type, bool linkSchema, UsedTypes& usedTypes, Type::Property* property = nullptr, const oatpp::Void& defaultValue = nullptr);

  static oatpp::Object<RequestBody> generateRequestBody(const Endpoint::Info& endpointInfo, bool linkSchema, UsedTypes& usedTypes);
  static Fields<Object<OperationResponse>> generateResponses(const Endpoint::Info& endpointInfo, bool linkSchema, UsedTypes& usedTypes);
  static void generatePathItemData(const std::shared_ptr<Endpoint>& endpoint, const oatpp::Object<PathItem>& pathItem, UsedTypes& usedTypes, UsedSecuritySchemes &usedSecuritySchemes);
  
  /*
   *  UsedTypes& usedTypes is used to put Types of objects whos schema should be reused
   */
  static Paths generatePaths(const std::shared_ptr<Endpoints>& endpoints, UsedTypes& usedTypes, UsedSecuritySchemes &usedSecuritySchemes);

  static oatpp::Object<SecurityScheme> generateSecurityScheme(const std::shared_ptr<oatpp::swagger::SecurityScheme> &ss);

  static void decomposeObject(const Type* type, UsedTypes& decomposedTypes);
  static void decomposeCollection_1D(const Type* type, UsedTypes& decomposedTypes);
  static void decomposeMap(const Type* type, UsedTypes& decomposedTypes);
  static void decomposeEnum(const Type* type, UsedTypes& decomposedTypes);
  static void decomposeType(const Type* type, UsedTypes& decomposedTypes);
  static UsedTypes decomposeTypes(UsedTypes& usedTypes);
  
  static oatpp::Object<Components> generateComponents(const UsedTypes &decomposedTypes,
                                                                    const std::shared_ptr<std::unordered_map<oatpp::String,std::shared_ptr<oatpp::swagger::SecurityScheme>>> &securitySchemes,
                                                                    UsedSecuritySchemes &usedSecuritySchemes);

public:

  /**
   * Generate Open Api Specification 3.0.0. from &l:Generator::Endpoints;.
   * @param docInfo - &id:oatpp::swagger::DocumentInfo;.
   * @param endpoints &l:Generator::Endpoints;.
   * @return - &id:oatpp::swagger::oas3::Document;.
   */
  static oatpp::Object<Document> generateDocument(const std::shared_ptr<oatpp::swagger::DocumentInfo>& docInfo, const std::shared_ptr<Endpoints>& endpoints);

};
  
}}}

#endif /* oatpp_swagger_oas3_Generator_hpp */
