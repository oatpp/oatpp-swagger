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

#include "Generator.hpp"

#include "oatpp/core/utils/ConversionUtils.hpp"

#include <limits>

namespace oatpp { namespace swagger { namespace oas3 {

Schema::ObjectWrapper Generator::generateSchemaForTypeObject(const oatpp::data::mapping::type::Type* type, bool linkSchema, UsedTypes& usedTypes) {

  OATPP_ASSERT(type && "[oatpp-swagger::oas3::Generator::generateSchemaForTypeObject()]: Error. Type should not be null.");

  auto result = Schema::createShared();
  if(linkSchema) {
  
    result->ref = oatpp::String("#/components/schemas/") + type->nameQualifier;
    usedTypes[type->nameQualifier] = type;
    return result;
  
  } else {
    
    result->type = "object";
    result->properties = result->properties->createShared();
    
    auto properties = type->properties;
    if(properties->getList().size() == 0) {
      type->creator(); // init type by creating first instance of that type
    }
    
    auto it = properties->getList().begin();
    while (it != properties->getList().end()) {
      auto p = *it ++;
      result->properties->put(p->name, generateSchemaForType(p->type, true, usedTypes));
    }
    
    return result;
  }
  
}
  
Schema::ObjectWrapper Generator::generateSchemaForTypeList(const oatpp::data::mapping::type::Type* type, bool linkSchema, UsedTypes& usedTypes) {

  OATPP_ASSERT(type && "[oatpp-swagger::oas3::Generator::generateSchemaForTypeList()]: Error. Type should not be null.");

  auto result = Schema::createShared();
  result->type = "array";
  result->items = generateSchemaForType(*type->params.begin(), linkSchema, usedTypes);
  return result;
}
  
Schema::ObjectWrapper Generator::generateSchemaForType(const oatpp::data::mapping::type::Type* type, bool linkSchema, UsedTypes& usedTypes) {

  OATPP_ASSERT(type && "[oatpp-swagger::oas3::Generator::generateSchemaForType()]: Error. Type should not be null.");

  auto classId = type->classId.id;
  if(classId == oatpp::data::mapping::type::__class::String::CLASS_ID.id){
    auto result = Schema::createShared();
    result->type = "string";
    return result;
  } else if(classId == oatpp::data::mapping::type::__class::Int8::CLASS_ID.id){
    auto result = Schema::createShared();
    result->type = "integer";
    result->minimum = std::numeric_limits<v_int8>::min();
    result->maximum = std::numeric_limits<v_int8>::max();
    return result;
  } else if(classId == oatpp::data::mapping::type::__class::UInt8::CLASS_ID.id){
    auto result = Schema::createShared();
    result->type = "integer";
    result->minimum = std::numeric_limits<v_uint8>::min();
    result->maximum = std::numeric_limits<v_uint8>::max();
    return result;
  } else if(classId == oatpp::data::mapping::type::__class::Int16::CLASS_ID.id){
    auto result = Schema::createShared();
    result->type = "integer";
    result->minimum = std::numeric_limits<v_int16>::min();
    result->maximum = std::numeric_limits<v_int16>::max();
    return result;
  } else if(classId == oatpp::data::mapping::type::__class::UInt16::CLASS_ID.id){
    auto result = Schema::createShared();
    result->type = "integer";
    result->minimum = std::numeric_limits<v_uint16>::min();
    result->maximum = std::numeric_limits<v_uint16>::max();
    return result;
  } else if(classId == oatpp::data::mapping::type::__class::Int32::CLASS_ID.id){
    auto result = Schema::createShared();
    result->type = "integer";
    result->minimum = std::numeric_limits<v_int32>::min();
    result->maximum = std::numeric_limits<v_int32>::max();
    return result;
  } else if(classId == oatpp::data::mapping::type::__class::UInt32::CLASS_ID.id){
    auto result = Schema::createShared();
    result->type = "integer";
    result->minimum = std::numeric_limits<v_uint32>::min();
    result->maximum = std::numeric_limits<v_uint32>::max();
    return result;
  } else if(classId == oatpp::data::mapping::type::__class::Int64::CLASS_ID.id){
    auto result = Schema::createShared();
    result->type = "integer";
    result->format = "int64";
    return result;
  } else if(classId == oatpp::data::mapping::type::__class::UInt64::CLASS_ID.id){
    auto result = Schema::createShared();
    result->type = "integer";
    return result;
  } else if(classId == oatpp::data::mapping::type::__class::Float32::CLASS_ID.id){
    auto result = Schema::createShared();
    result->type = "number";
    result->format = "float";
    return result;
  } else if(classId == oatpp::data::mapping::type::__class::Float64::CLASS_ID.id){
    auto result = Schema::createShared();
    result->type = "number";
    result->format = "double";
    return result;
  } else if(classId == oatpp::data::mapping::type::__class::Boolean::CLASS_ID.id){
    auto result = Schema::createShared();
    result->type = "boolean";
    return result;
  } else if(classId == oatpp::data::mapping::type::__class::AbstractObject::CLASS_ID.id){
    return generateSchemaForTypeObject(type, linkSchema, usedTypes);
  } else if(classId == oatpp::data::mapping::type::__class::AbstractList::CLASS_ID.id){
    return generateSchemaForTypeList(type, linkSchema, usedTypes);
  } else if(classId == oatpp::data::mapping::type::__class::AbstractListMap::CLASS_ID.id){
    // TODO
  } else {
    auto result = Schema::createShared();
    result->type = type->classId.name;
    if(type->nameQualifier) {
      result->format = type->nameQualifier;
    }
    return result;
  }
  
  return Schema::createShared();
  
}

void Generator::addParamsToParametersList(const PathItemParameters::ObjectWrapper& paramsList,
                                          Endpoint::Info::Params& params,
                                          const oatpp::String& inType,
                                          UsedTypes& usedTypes)
{

  auto it = params.getOrder().begin();
  while (it != params.getOrder().end()) {
    auto param = params[*it++];
    auto parameter = PathItemParameter::createShared();
    parameter->in = inType;
    parameter->name = param.name;
    parameter->description = param.description;
    parameter->required = param.required;
    parameter->deprecated = param.deprecated;
    parameter->schema = generateSchemaForType(param.type, true, usedTypes);
    paramsList->pushBack(parameter);
  }

}

RequestBody::ObjectWrapper Generator::generateRequestBody(const Endpoint::Info& endpointInfo, bool linkSchema, UsedTypes& usedTypes) {

  if(endpointInfo.consumes.size() > 0) {

    auto body = RequestBody::createShared();
    body->description = "request body";
    body->content = body->content->createShared();
    
    auto it = endpointInfo.consumes.begin();
    while (it != endpointInfo.consumes.end()) {
      
      auto mediaType = MediaTypeObject::createShared();
      mediaType->schema = generateSchemaForType(it->schema, linkSchema, usedTypes);
      
      body->content->put(it->contentType, mediaType);
      
      it++;
    }

    return body;

  } else {
  
    if(endpointInfo.body.name != nullptr && endpointInfo.body.type != nullptr) {

      auto body = RequestBody::createShared();
      body->description = "request body";
      
      auto mediaType = MediaTypeObject::createShared();
      mediaType->schema = generateSchemaForType(endpointInfo.body.type, linkSchema, usedTypes);
      
      body->content = body->content->createShared();
      if(endpointInfo.bodyContentType != nullptr) {
        body->content->put(endpointInfo.bodyContentType, mediaType);
      } else {

        OATPP_ASSERT(endpointInfo.body.type && "[oatpp-swagger::oas3::Generator::generateRequestBody()]: Error. Type should not be null.");

        if(endpointInfo.body.type->classId.id == oatpp::data::mapping::type::__class::AbstractObject::CLASS_ID.id) {
          body->content->put("application/json", mediaType);
        } else if(endpointInfo.body.type->classId.id == oatpp::data::mapping::type::__class::AbstractList::CLASS_ID.id) {
          body->content->put("application/json", mediaType);
        } else if(endpointInfo.body.type->classId.id == oatpp::data::mapping::type::__class::AbstractListMap::CLASS_ID.id) {
          body->content->put("application/json", mediaType);
        } else {
          body->content->put("text/plain", mediaType);
        }
      }

      return body;

    }

  }
  
  return nullptr;

}

Generator::Fields<OperationResponse::ObjectWrapper>::ObjectWrapper Generator::generateResponses(const Endpoint::Info& endpointInfo, bool linkSchema, UsedTypes& usedTypes) {
  
  auto responses = Fields<OperationResponse::ObjectWrapper>::createShared();
  
  if(endpointInfo.responses.size() > 0) {
    
    auto it = endpointInfo.responses.begin();
    while (it != endpointInfo.responses.end()) {
      
      auto mediaType = MediaTypeObject::createShared();
      mediaType->schema = generateSchemaForType(it->second.schema, linkSchema, usedTypes);
      
      auto response = OperationResponse::createShared();
      response->description = it->first.description;
      response->content = response->content->createShared();
      response->content->put(it->second.contentType, mediaType);
      responses->put(oatpp::utils::conversion::int32ToStr(it->first.code), response);
      
      it++;
    }
    
  } else {
  
    auto mediaType = MediaTypeObject::createShared();
    mediaType->schema = generateSchemaForType(oatpp::String::Class::getType(), linkSchema, usedTypes);
  
    auto response = OperationResponse::createShared();
    response->description = "success";
    response->content = response->content->createShared();
    response->content->put("text/plain", mediaType);
    responses->put("200", response);
  
  }
  
  return responses;
    
}
  
void Generator::generatePathItemData(const std::shared_ptr<Endpoint>& endpoint, const PathItem::ObjectWrapper& pathItem, UsedTypes& usedTypes, UsedSecuritySchemes &usedSecuritySchemes) {
  
  auto info = endpoint->info();
  
  if(info) {
    
    auto operation = PathItemOperation::createShared();
    operation->operationId = info->name;
    operation->summary = info->summary;
    operation->description = info->description;

    if(info->tags.size() > 0) {
      operation->tags = operation->tags->createShared();
      for(auto& tag : info->tags) {
        operation->tags->pushBack(tag);
      }
    }
    
    if(oatpp::base::StrBuffer::equalsCI("get", info->method->c_str(), info->method->getSize())) {
      pathItem->operationGet = operation;
    } else if(oatpp::base::StrBuffer::equalsCI("put", info->method->c_str(), info->method->getSize())) {
      pathItem->operationPut = operation;
    } else if(oatpp::base::StrBuffer::equalsCI("post", info->method->c_str(), info->method->getSize())) {
      pathItem->operationPost = operation;
    } else if(oatpp::base::StrBuffer::equalsCI("delete", info->method->c_str(), info->method->getSize())) {
      pathItem->operationDelete = operation;
    } else if(oatpp::base::StrBuffer::equalsCI("options", info->method->c_str(), info->method->getSize())) {
      pathItem->operationOptions = operation;
    } else if(oatpp::base::StrBuffer::equalsCI("head", info->method->c_str(), info->method->getSize())) {
      pathItem->operationHead = operation;
    } else if(oatpp::base::StrBuffer::equalsCI("patch", info->method->c_str(), info->method->getSize())) {
      pathItem->operationPatch = operation;
    } else if(oatpp::base::StrBuffer::equalsCI("trace", info->method->c_str(), info->method->getSize())) {
      pathItem->operationTrace = operation;
    }
    
    operation->responses = generateResponses(*info, true, usedTypes);
    operation->requestBody = generateRequestBody(*info, true, usedTypes);

    if(!operation->parameters) {

      operation->parameters = operation->parameters->createShared();

      Endpoint::Info::Params filteredHeaders;
      if(!info->headers.getOrder().empty()) {
        for (const auto &header : info->headers.getOrder()) {
          // We don't want the Authorization header listed as Parameter. This should be done in ENDPOINT_INFO() { info->addSecurityRequirement( /* SecurityScheme-Name */ ); }
          if (header != oatpp::web::protocol::http::Header::AUTHORIZATION) {
            filteredHeaders[header] = info->headers[header];
          }
        }
      }

      addParamsToParametersList(operation->parameters, filteredHeaders, "header", usedTypes);
      addParamsToParametersList(operation->parameters, info->pathParams, "path", usedTypes);
      addParamsToParametersList(operation->parameters, info->queryParams, "query", usedTypes);

    }

    if(!info->securityRequirements.empty()) {
      OATPP_ASSERT(info->authorization && "[oatpp-swagger::oas3::Generator::generatePathItemData()]: Error. Endpoint has security requirement but is no authorized endpoint.");
    }

    if(info->authorization) {
      OATPP_ASSERT(!info->securityRequirements.empty() && "[oatpp-swagger::oas3::Generator::generatePathItemData()]: Error. Authorized endpoint with no security requirements (info->addSecurityRequirement()) set.");

      if (!info->securityRequirements.empty()) {

        operation->security = operation->security->createShared();

        for (const auto &sec : info->securityRequirements) {

          usedSecuritySchemes[sec.first] = true;
          if (sec.second == nullptr) {

            // who ever came up to define "security" as an array of objects of array of strings
            auto fields = Fields<Components::List<String>::ObjectWrapper>::createShared();
            fields->put(sec.first, Components::List<String>::createShared());
            operation->security->pushBack(fields);

          } else {

            auto fields = Fields<Components::List<String>::ObjectWrapper>::createShared();
            auto sro = Components::List<String>::createShared();
            for (const auto &sr : *sec.second) {
              sro->pushBack(sr);
            }
            fields->put(sec.first, sro);
            operation->security->pushBack(fields);

          }
        }
      }
    }
  }
}
  
Generator::Paths::ObjectWrapper Generator::generatePaths(const std::shared_ptr<Endpoints>& endpoints, UsedTypes& usedTypes, UsedSecuritySchemes &usedSecuritySchemes) {
  
  auto result = Paths::createShared();
  
  auto curr = endpoints->getFirstNode();
  while (curr != nullptr) {
    auto endpoint = curr->getData();
    
    if(endpoint->info() && !endpoint->info()->hide) {
      oatpp::String path = endpoint->info()->path;
      if(path->getSize() == 0) {
        continue;
      }
      if(path->getData()[0] != '/') {
        path = "/" + path;
      }

      auto pathItem = result->get(path, nullptr);
      if(!pathItem) {
        pathItem = PathItem::createShared();
        result->put(path, pathItem);
      }

      generatePathItemData(endpoint, pathItem, usedTypes, usedSecuritySchemes);
    }
    
    curr = curr->getNext();
  }
  
  return result;
  
}
  
void Generator::decomposeObject(const oatpp::data::mapping::type::Type* type, UsedTypes& decomposedTypes) {

  OATPP_ASSERT(type && "[oatpp-swagger::oas3::Generator::decomposeObject()]: Error. Type should not be null.");

  auto schemaIt = decomposedTypes.find(type->nameQualifier);
  if(schemaIt != decomposedTypes.end()) {
    return;
  }
  
  decomposedTypes[type->nameQualifier] = type;
  
  auto properties = type->properties;
  if(properties->getList().size() == 0) {
    type->creator(); // init type by creating first instance of that type
  }
  
  auto it = properties->getList().begin();
  while (it != properties->getList().end()) {
    auto p = *it ++;
    decomposeType(p->type, decomposedTypes);
  }
}

void Generator::decomposeList(const oatpp::data::mapping::type::Type* type, UsedTypes& decomposedTypes) {
  OATPP_ASSERT(type && "[oatpp-swagger::oas3::Generator::decomposeList()]: Error. Type should not be null.");
  decomposeType(*type->params.begin(), decomposedTypes);
}

void Generator::decomposeMap(const oatpp::data::mapping::type::Type* type, UsedTypes& decomposedTypes) {
  // TODO
}
  
void Generator::decomposeType(const oatpp::data::mapping::type::Type* type, UsedTypes& decomposedTypes) {
  OATPP_ASSERT(type && "[oatpp-swagger::oas3::Generator::decomposeType()]: Error. Type should not be null.");
  auto classId = type->classId.id;
  if(classId == oatpp::data::mapping::type::__class::AbstractObject::CLASS_ID.id){
    decomposeObject(type, decomposedTypes);
  } else if(classId == oatpp::data::mapping::type::__class::AbstractList::CLASS_ID.id){
    decomposeList(type, decomposedTypes);
  } else if(classId == oatpp::data::mapping::type::__class::AbstractListMap::CLASS_ID.id){
    decomposeMap(type, decomposedTypes);
  }
}
  
Generator::UsedTypes Generator::decomposeTypes(UsedTypes& usedTypes) {
  
  UsedTypes result; // decomposed schemas
  
  auto it = usedTypes.begin();
  while (it != usedTypes.end()) {
    decomposeType(it->second, result);
    result[it->first] = it->second;
    it ++;
  }
  
  return result;
  
}
  
Components::ObjectWrapper Generator::generateComponents(const UsedTypes &decomposedTypes,
                                                        const std::shared_ptr<std::unordered_map<oatpp::String,std::shared_ptr<oatpp::swagger::SecurityScheme>>> &securitySchemes,
                                                        UsedSecuritySchemes &usedSecuritySchemes) {
  
  auto result = Components::createShared();
  result->schemas = result->schemas->createShared();
  
  auto it = decomposedTypes.begin();
  while (it != decomposedTypes.end()) {
    UsedTypes schemas; ///< dummy
    result->schemas->put(it->first, generateSchemaForType(it->second, false, schemas));
    it ++;
  }

  if(securitySchemes) {
    result->securitySchemes = result->securitySchemes->createShared();
    for (const auto &ss : usedSecuritySchemes) {
        OATPP_ASSERT(securitySchemes->find(ss.first) != securitySchemes->end() && "[oatpp-swagger::oas3::Generator::generateComponents()]: Error. Requested unknown security requirement.");
        result->securitySchemes->put(ss.first, generateSecurityScheme(securitySchemes->at(ss.first)));
    }
  }

  return result;
  
}
  
Document::ObjectWrapper Generator::generateDocument(const std::shared_ptr<oatpp::swagger::DocumentInfo>& docInfo, const std::shared_ptr<Endpoints>& endpoints) {
  
  auto document = oas3::Document::createShared();
  document->info = Info::createFromBaseModel(docInfo->header);
  
  if(docInfo->servers) {
    document->servers = document->servers->createShared();

    for(const auto &it : *docInfo->servers) {
      document->servers->pushBack(Server::createFromBaseModel(it));
    }

  }
  
  UsedTypes usedTypes;
  UsedSecuritySchemes usedSecuritySchemes;
  document->paths = generatePaths(endpoints, usedTypes, usedSecuritySchemes);
  auto decomposedTypes = decomposeTypes(usedTypes);
  document->components = generateComponents(decomposedTypes, docInfo->securitySchemes, usedSecuritySchemes);

  return document;
  
}

SecurityScheme::ObjectWrapper Generator::generateSecurityScheme(const std::shared_ptr<oatpp::swagger::SecurityScheme> &ss) {
  auto oasSS = oatpp::swagger::oas3::SecurityScheme::createShared();

  oasSS->type = ss->type;
  oasSS->description = ss->description;
  oasSS->openIdConnectUrl = ss->openIdConnectUrl;
  oasSS->in = ss->in;
  oasSS->bearerFormat = ss->bearerFormat;
  oasSS->name = ss->name;
  oasSS->scheme = ss->scheme;

  if(ss->flows) {
    oasSS->flows = oasSS->flows->createShared();
    if(ss->flows->implicit) {
      oasSS->flows->implicit = oasSS->flows->implicit->createShared();
      oasSS->flows->implicit->tokenUrl = ss->flows->implicit->tokenUrl;
      oasSS->flows->implicit->refreshUrl = ss->flows->implicit->refreshUrl;
      oasSS->flows->implicit->authorizationUrl = ss->flows->implicit->authorizationUrl;
      if(ss->flows->implicit->scopes) {
        oasSS->flows->implicit->scopes->createShared();
        for(const auto &scope : *ss->flows->implicit->scopes) {
          oasSS->flows->implicit->scopes->put(scope.first, scope.second);
        }
      }
    }
    if(ss->flows->password) {
      oasSS->flows->password = oasSS->flows->password->createShared();
      oasSS->flows->password->tokenUrl = ss->flows->password->tokenUrl;
      oasSS->flows->password->refreshUrl = ss->flows->password->refreshUrl;
      oasSS->flows->password->authorizationUrl = ss->flows->password->authorizationUrl;
      if(ss->flows->password->scopes) {
        oasSS->flows->password->scopes->createShared();
        for(const auto &scope : *ss->flows->password->scopes) {
          oasSS->flows->password->scopes->put(scope.first, scope.second);
        }
      }
    }
    if(ss->flows->clientCredentials) {
      oasSS->flows->clientCredentials = oasSS->flows->clientCredentials->createShared();
      oasSS->flows->clientCredentials->tokenUrl = ss->flows->clientCredentials->tokenUrl;
      oasSS->flows->clientCredentials->refreshUrl = ss->flows->clientCredentials->refreshUrl;
      oasSS->flows->clientCredentials->authorizationUrl = ss->flows->clientCredentials->authorizationUrl;
      if(ss->flows->clientCredentials->scopes) {
        oasSS->flows->clientCredentials->scopes->createShared();
        for(const auto &scope : *ss->flows->clientCredentials->scopes) {
          oasSS->flows->clientCredentials->scopes->put(scope.first, scope.second);
        }
      }
    }
    if(ss->flows->authorizationCode) {
      oasSS->flows->authorizationCode = oasSS->flows->authorizationCode->createShared();
      oasSS->flows->authorizationCode->tokenUrl = ss->flows->authorizationCode->tokenUrl;
      oasSS->flows->authorizationCode->refreshUrl = ss->flows->authorizationCode->refreshUrl;
      oasSS->flows->authorizationCode->authorizationUrl = ss->flows->authorizationCode->authorizationUrl;
      if(ss->flows->authorizationCode->scopes) {
        oasSS->flows->authorizationCode->scopes->createShared();
        for(const auto &scope : *ss->flows->authorizationCode->scopes) {
          oasSS->flows->authorizationCode->scopes->put(scope.first, scope.second);
        }
      }
    }
  }
  return oasSS;
}

}}}
