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

#include "oatpp/utils/Conversion.hpp"
#include "oatpp/data/stream/BufferStream.hpp"
#include "oatpp/base/Log.hpp"

#include <limits>

namespace oatpp { namespace swagger {

oatpp::String Generator::getEnumSchemaName(const Type* type) {

  auto polymorphicDispatcher = static_cast<const data::type::__class::AbstractEnum::PolymorphicDispatcher*>(
    type->polymorphicDispatcher
  );

  Type* interType = polymorphicDispatcher->getInterpretationType();

  data::stream::BufferOutputStream stream;
  stream << type->nameQualifier << "_" << interType->classId.name;
  return stream.toString();

}

oatpp::Object<oas3::Schema> Generator::generateSchemaForSimpleType(const Type* type, oatpp::BaseObject::Property* property, const oatpp::Void& defaultValue) {

  OATPP_ASSERT(type && "[oatpp-swagger::oas3::Generator::generateSchemaForSimpleType()]: Error. Type should not be null.");

  auto result = oas3::Schema::createShared();

  auto classId = type->classId.id;
  if(classId == oatpp::data::type::__class::String::CLASS_ID.id) {
    result->type = "string";
    if(property != nullptr && !property->info.pattern.empty()) {
      result->pattern = property->info.pattern.c_str();
    }
  } else if(classId == oatpp::data::type::__class::Int8::CLASS_ID.id) {
    result->type = "integer";
    result->minimum = std::numeric_limits<v_int8>::min();
    result->maximum = std::numeric_limits<v_int8>::max();
  } else if(classId == oatpp::data::type::__class::UInt8::CLASS_ID.id) {
    result->type = "integer";
    result->minimum = std::numeric_limits<v_uint8>::min();
    result->maximum = std::numeric_limits<v_uint8>::max();
  } else if(classId == oatpp::data::type::__class::Int16::CLASS_ID.id) {
    result->type = "integer";
    result->minimum = std::numeric_limits<v_int16>::min();
    result->maximum = std::numeric_limits<v_int16>::max();
  } else if(classId == oatpp::data::type::__class::UInt16::CLASS_ID.id) {
    result->type = "integer";
    result->minimum = std::numeric_limits<v_uint16>::min();
    result->maximum = std::numeric_limits<v_uint16>::max();
  } else if(classId == oatpp::data::type::__class::Int32::CLASS_ID.id) {
    result->type = "integer";
    result->minimum = std::numeric_limits<v_int32>::min();
    result->maximum = std::numeric_limits<v_int32>::max();
  } else if(classId == oatpp::data::type::__class::UInt32::CLASS_ID.id) {
    result->type = "integer";
    result->minimum = std::numeric_limits<v_uint32>::min();
    result->maximum = std::numeric_limits<v_uint32>::max();
  } else if(classId == oatpp::data::type::__class::Int64::CLASS_ID.id) {
    result->type = "integer";
    result->format = "int64";
  } else if(classId == oatpp::data::type::__class::UInt64::CLASS_ID.id) {
    result->type = "integer";
  } else if(classId == oatpp::data::type::__class::Float32::CLASS_ID.id) {
    result->type = "number";
    result->format = "float";
  } else if(classId == oatpp::data::type::__class::Float64::CLASS_ID.id) {
    result->type = "number";
    result->format = "double";
  } else if(classId == oatpp::data::type::__class::Boolean::CLASS_ID.id) {
    result->type = "boolean";
  } else {
    return nullptr; // Unknown simple type;
  }

  return result;

}

oatpp::Object<oas3::Schema> Generator::generateSchemaForTypeObject(const Type* type, bool linkSchema, UsedTypes& usedTypes) {

  OATPP_ASSERT(type && "[oatpp-swagger::oas3::Generator::generateSchemaForTypeObject()]: Error. Type should not be null.");

  auto result = oas3::Schema::createShared();
  if(linkSchema) {

    result->ref = oatpp::String("#/components/schemas/") + type->nameQualifier;
    usedTypes[type->nameQualifier] = type;
    return result;

  } else {

    result->type = "object";
    result->properties = {};

    auto polymorphicDispatcher = static_cast<const oatpp::data::type::__class::AbstractObject::PolymorphicDispatcher*>(
      type->polymorphicDispatcher
    );
    auto instance = polymorphicDispatcher->createObject();
    auto properties = polymorphicDispatcher->getProperties();

    for(auto* p : properties->getList()) {
      const auto& defaultValue = p->get(static_cast<oatpp::BaseObject*>(instance.get()));
      result->properties[p->name] = generateSchemaForType(p->type, true, usedTypes, p, defaultValue);
    }

    result->required = oatpp::List<oatpp::String>::createShared();
    for (auto* p : properties->getList()) {
      if (p->info.required) {
        result->required->push_back(p->name);
      }
    }

    return result;
  }

}

oatpp::Object<oas3::Schema> Generator::generateSchemaForCollection_1D(const Type* type, bool linkSchema, UsedTypes& usedTypes, bool uniqueItems) {
  OATPP_ASSERT(type && "[oatpp-swagger::oas3::Generator::generateSchemaForCollection_1D()]: Error. Type should not be null.");
  auto result = oas3::Schema::createShared();
  result->type = "array";
  result->items = generateSchemaForType(*type->params.begin(), linkSchema, usedTypes);
  if(uniqueItems) {
    result->uniqueItems = true;
  }
  return result;
}

oatpp::Object<oas3::Schema> Generator::generateSchemaForEnum(const Type* type, bool linkSchema, UsedTypes& usedTypes, oatpp::BaseObject::Property* property) {
  OATPP_ASSERT(type && "[oatpp-swagger::oas3::Generator::generateSchemaForEnum()]: Error. Type should not be null.");

  if(linkSchema) {

    auto result = oas3::Schema::createShared();
    result->ref = oatpp::String("#/components/schemas/") + getEnumSchemaName(type);
    usedTypes[getEnumSchemaName(type)] = type;
    return result;

  }

  auto polymorphicDispatcher = static_cast<const data::type::__class::AbstractEnum::PolymorphicDispatcher*>(
    type->polymorphicDispatcher
  );

  Type* interType = polymorphicDispatcher->getInterpretationType();
  auto result = generateSchemaForType(interType, linkSchema, usedTypes);

  result->enumValues = oatpp::List<oatpp::Any>::createShared();
  auto interEnum = polymorphicDispatcher->getInterpretedEnum(false);
  for(auto& v : interEnum) {
    result->enumValues->push_back(v);
  }

  return result;

}

oatpp::Object<oas3::Schema> Generator::generateSchemaForAbstractPairList(const Type* type, bool linkSchema, UsedTypes& usedTypes, oatpp::BaseObject::Property* property ) {
  OATPP_ASSERT(type && "[oatpp-swagger::oas3::Generator::generateSchemaForAbstractPairList()]: Error. Type should not be null.");

  auto result = oas3::Schema::createShared();

  // A PairList<String, T> is a Fields<T> and a Fields<T> is a simple JSON object
  if (type->params.front()->classId.id == oatpp::String::Class::CLASS_ID.id) {
    result->type = "object";
    result->additionalProperties = generateSchemaForType(type->params.back(), linkSchema, usedTypes);
  }

  return result;
}

oatpp::Object<oas3::Schema> Generator::generateSchemaForType(const Type* type, bool linkSchema, UsedTypes& usedTypes, oatpp::BaseObject::Property* property, const oatpp::Void& defaultValue) {

  OATPP_ASSERT(type && "[oatpp-swagger::oas3::Generator::generateSchemaForType()]: Error. Type should not be null.");

  oatpp::Object<oas3::Schema> result;

  auto classId = type->classId.id;

  if(classId == oatpp::data::type::__class::AbstractObject::CLASS_ID.id) {
    result = generateSchemaForTypeObject(type, linkSchema, usedTypes);
  } else if(classId == oatpp::data::type::__class::AbstractVector::CLASS_ID.id) {
    result = generateSchemaForCollection_1D(type, linkSchema, usedTypes, false);
  } else if(classId == oatpp::data::type::__class::AbstractList::CLASS_ID.id) {
    result = generateSchemaForCollection_1D(type, linkSchema, usedTypes, false);
  } else if(classId == oatpp::data::type::__class::AbstractUnorderedSet::CLASS_ID.id) {
    result = generateSchemaForCollection_1D(type, linkSchema, usedTypes, true);
  } else if(classId == oatpp::data::type::__class::AbstractPairList::CLASS_ID.id) {
    result = generateSchemaForAbstractPairList(type, linkSchema, usedTypes, property);
  } else if(classId == oatpp::data::type::__class::AbstractEnum::CLASS_ID.id) {
    result = generateSchemaForEnum(type, linkSchema, usedTypes, property);
  } else {
    result = generateSchemaForSimpleType(type, property, defaultValue);
  }

  if(!result) {
    auto interpretation = type->findInterpretation(m_config->enableInterpretations);
    if(interpretation) {
      result = generateSchemaForType(interpretation->getInterpretationType(), linkSchema, usedTypes);
    }
  }

  if(!result) {
    result = oas3::Schema::createShared();
    result->type = type->classId.name;
    if(type->nameQualifier) {
      result->format = type->nameQualifier;
    }
  }

  if(property != nullptr) {
    if(!property->info.description.empty()) {
      result->description = property->info.description.c_str();
    }
    if (defaultValue) {
      result->defaultValue = defaultValue;
    }
  }

  return result;
}

void Generator::addParamsToParametersList(const PathItemParameters& paramsList,
                                          Endpoint::Info::Params& params,
                                          const oatpp::String& inType,
                                          UsedTypes& usedTypes)
{

  for(auto& paramName : params.getOrder()) {
    auto param = params[paramName];
    auto parameter = oas3::PathItemParameter::createShared();
    parameter->in = inType;
    parameter->name = param.name;
    parameter->description = param.description;
    parameter->required = param.required;
    parameter->deprecated = param.deprecated;
    parameter->schema = generateSchemaForType(param.type, true, usedTypes);

    for(auto& ex : param.examples) {
      parameter->addExample(ex.first, ex.second);
    }

    paramsList->push_back(parameter);
  }

}

oatpp::Object<oas3::RequestBody> Generator::generateRequestBody(const Endpoint::Info& endpointInfo, bool linkSchema, UsedTypes& usedTypes) {

  if(endpointInfo.consumes.size() > 0) {

    auto body = oas3::RequestBody::createShared();
    body->description = endpointInfo.body.description;
    body->required = endpointInfo.body.required;
    body->content = body->content.createShared();

    for(auto& hint : endpointInfo.consumes) {
      auto mediaType = oas3::MediaTypeObject::createShared();

      mediaType->schema = generateSchemaForType(hint.schema, linkSchema, usedTypes);

      if(hint.examples.size() > 0) {
        for(auto& ex : hint.examples) {
          mediaType->addExample(ex.first, ex.second);
        }
      } else {
        for(auto& ex : endpointInfo.body.examples) {
          mediaType->addExample(ex.first, ex.second);
        }
      }

      body->content[hint.contentType] = mediaType;
    }

    return body;

  } else {

    if(endpointInfo.body.name != nullptr && endpointInfo.body.type != nullptr) {

      auto body = oas3::RequestBody::createShared();
      body->description = endpointInfo.body.description;
      body->required = endpointInfo.body.required;

      auto mediaType = oas3::MediaTypeObject::createShared();
      mediaType->schema = generateSchemaForType(endpointInfo.body.type, linkSchema, usedTypes);
      for(auto& ex : endpointInfo.body.examples) {
        mediaType->addExample(ex.first, ex.second);
      }

      body->content = body->content.createShared();

      if(endpointInfo.bodyContentType != nullptr) {
        body->content[endpointInfo.bodyContentType] = mediaType;
      } else {
        body->content["text/plain"] = mediaType;
      }

      return body;

    }

  }

  return nullptr;

}

oatpp::Fields<Object<oas3::OperationResponse>> Generator::generateResponses(const Endpoint::Info& endpointInfo, bool linkSchema, UsedTypes& usedTypes) {

  auto responses = Fields<Object<oas3::OperationResponse>>::createShared();

  if(endpointInfo.responses.size() > 0) {

    for(auto& hint : endpointInfo.responses) {

      auto response = oas3::OperationResponse::createShared();

      if(hint.second.schema != nullptr) {

        auto mediaType = oas3::MediaTypeObject::createShared();
        mediaType->schema = generateSchemaForType(hint.second.schema, linkSchema, usedTypes);

        for(auto& ex : hint.second.examples) {
          mediaType->addExample(ex.first, ex.second);
        }

        response->content = {};
        response->content[hint.second.contentType] = mediaType;

      }

      response->description = hint.second.description.get() == nullptr ? hint.first.description : hint.second.description;
      responses[oatpp::utils::Conversion::int32ToStr(hint.first.code)] = response;

    }

  } else {

    auto response = oas3::OperationResponse::createShared();
    response->description = "success";
    responses["200"] = response;

  }

  return responses;

}

void Generator::generatePathItemData(const std::shared_ptr<Endpoint>& endpoint, const oatpp::Object<oas3::PathItem>& pathItem, UsedTypes& usedTypes, UsedSecuritySchemes &usedSecuritySchemes, UsedTags &usedTags) {

  auto info = endpoint->info();

  if(info) {

    auto operation = oas3::PathItemOperation::createShared();
    operation->operationId = info->name;
    operation->summary = info->summary;
    operation->description = info->description;

    if(info->tags.size() > 0) {
      operation->tags = oatpp::List<String>({});
      for(auto& tag : info->tags) {
        operation->tags->push_back(tag);
        usedTags[tag] = true;
      }
    }

    if(info->method.equalsCI_ASCII("get")) {
      pathItem->operationGet = operation;
    } else if(info->method.equalsCI_ASCII("put")) {
      pathItem->operationPut = operation;
    } else if(info->method.equalsCI_ASCII("post")) {
      pathItem->operationPost = operation;
    } else if(info->method.equalsCI_ASCII("delete")) {
      pathItem->operationDelete = operation;
    } else if(info->method.equalsCI_ASCII("options")) {
      pathItem->operationOptions = operation;
    } else if(info->method.equalsCI_ASCII("head")) {
      pathItem->operationHead = operation;
    } else if(info->method.equalsCI_ASCII("patch")) {
      pathItem->operationPatch = operation;
    } else if(info->method.equalsCI_ASCII("trace")) {
      pathItem->operationTrace = operation;
    }

    operation->responses = generateResponses(*info, true, usedTypes);
    operation->requestBody = generateRequestBody(*info, true, usedTypes);

    if(!operation->parameters) {

      operation->parameters = {};

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

        operation->security = oatpp::List<Fields<List<String>>>({});

        for (const auto &sec : info->securityRequirements) {

          usedSecuritySchemes[sec.first] = true;
          if (sec.second == nullptr) {

            // who ever came up to define "security" as an array of objects of array of strings
            auto fields = Fields<oatpp::List<String>>({});
            fields[sec.first] = oatpp::List<String>({});
            operation->security->push_back(fields);

          } else {

            auto fields = Fields<List<String>>::createShared();
            auto sro = List<String>::createShared();
            for (const auto &sr : *sec.second) {
              sro->push_back(sr);
            }
            fields[sec.first] = sro;
            operation->security->push_back(fields);

          }
        }
      }
    }
  }
}

Generator::Paths Generator::generatePaths(const Endpoints& endpoints, UsedTypes& usedTypes, UsedSecuritySchemes &usedSecuritySchemes, UsedTags &usedTags) {

  auto result = Paths::createShared();

  for(auto& endpoint : endpoints.list) {
    if(endpoint->info() && !endpoint->info()->hide) {
      oatpp::String path = endpoint->info()->path;
      if(path->size() == 0) {
        continue;
      }
      if(path->data()[0] != '/') {
        path = "/" + path;
      }

      auto& pathItem = result[path];
      if(!pathItem) {
        pathItem = oas3::PathItem::createShared();
      }

      generatePathItemData(endpoint, pathItem, usedTypes, usedSecuritySchemes, usedTags);
    }
  }

  return result;

}

void Generator::decomposeObject(const Type* type, UsedTypes& decomposedTypes) {

  OATPP_ASSERT(type && "[oatpp-swagger::oas3::Generator::decomposeObject()]: Error. Type should not be null.");

  auto schemaIt = decomposedTypes.find(type->nameQualifier);
  if(schemaIt != decomposedTypes.end()) {
    return;
  }

  decomposedTypes[type->nameQualifier] = type;

  auto polymorphicDispatcher = static_cast<const oatpp::data::type::__class::AbstractObject::PolymorphicDispatcher*>(
    type->polymorphicDispatcher
  );
  auto properties = polymorphicDispatcher->getProperties();

  for(auto* p : properties->getList()) {
    decomposeType(p->type, decomposedTypes);
  }

}

void Generator::decomposeCollection_1D(const Type* type, UsedTypes& decomposedTypes) {
  OATPP_ASSERT(type && "[oatpp-swagger::oas3::Generator::Collection_1D()]: Error. Type should not be null.");
  decomposeType(*type->params.begin(), decomposedTypes);
}

void Generator::decomposeMap(const Type* type, UsedTypes& decomposedTypes) {
  OATPP_ASSERT(type && "[oatpp-swagger::oas3::Generator::decomposeMap()]: Error. Type should not be null.");

  // The only possible JSON representation of a PairList<A, B> is A being a String, even if an numeric one
  if (type->params.front()->classId.id == oatpp::data::type::__class::String::CLASS_ID.id) {
    decomposeType(type->params.back(), decomposedTypes);
  }

  // TODO - more advanced cases with complex key types
}

void Generator::decomposeEnum(const Type* type, UsedTypes& decomposedTypes) {
  auto schemaIt = decomposedTypes.find(getEnumSchemaName(type));
  if(schemaIt != decomposedTypes.end()) {
    return;
  }
  decomposedTypes[getEnumSchemaName(type)] = type;
}

void Generator::decomposeType(const Type* type, UsedTypes& decomposedTypes) {
  OATPP_ASSERT(type && "[oatpp-swagger::oas3::Generator::decomposeType()]: Error. Type should not be null.");
  auto classId = type->classId.id;
  if(classId == oatpp::data::type::__class::AbstractObject::CLASS_ID.id){
    decomposeObject(type, decomposedTypes);
  } else if(classId == oatpp::data::type::__class::AbstractVector::CLASS_ID.id){
    decomposeCollection_1D(type, decomposedTypes);
  } else if(classId == oatpp::data::type::__class::AbstractList::CLASS_ID.id){
    decomposeCollection_1D(type, decomposedTypes);
  } else if(classId == oatpp::data::type::__class::AbstractUnorderedSet::CLASS_ID.id){
    decomposeCollection_1D(type, decomposedTypes);
  } else if(classId == oatpp::data::type::__class::AbstractPairList::CLASS_ID.id){
    decomposeMap(type, decomposedTypes);
  } else if(classId == oatpp::data::type::__class::AbstractEnum::CLASS_ID.id){
    decomposeEnum(type, decomposedTypes);
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

oatpp::Object<oas3::Components> Generator::generateComponents(const UsedTypes &decomposedTypes,
                                                              const std::shared_ptr<std::unordered_map<oatpp::String,std::shared_ptr<oatpp::swagger::SecurityScheme>>> &securitySchemes,
                                                              UsedSecuritySchemes &usedSecuritySchemes) {
  
  auto result = oas3::Components::createShared();
  result->schemas = {};
  
  auto it = decomposedTypes.begin();
  while (it != decomposedTypes.end()) {
    UsedTypes schemas; ///< dummy
    result->schemas[it->first] = generateSchemaForType(it->second, false, schemas);
    it ++;
  }

  if(securitySchemes) {
    result->securitySchemes = {};
    for (const auto &ss : usedSecuritySchemes) {
        OATPP_ASSERT(securitySchemes->find(ss.first) != securitySchemes->end() && "[oatpp-swagger::oas3::Generator::generateComponents()]: Error. Requested unknown security requirement.");
        result->securitySchemes[ss.first] = generateSecurityScheme(securitySchemes->at(ss.first));
    }
  }

  return result;
  
}

Generator::Tags Generator::generateTags(const std::shared_ptr<std::list<std::shared_ptr<oatpp::swagger::Tag>>> &tags,
                             const UsedTags &usedTags) {
  auto result = Tags::createShared();

  UsedTags missingTags{ usedTags};

  if (tags) {
    for (const auto &tag : *tags) {
      if (!tag || !tag->name) {
        continue;
      }

      if ( usedTags.find(tag->name)== usedTags.end()) {
        continue;
      }

      missingTags.erase(tag->name);

      auto oasTagItem = oatpp::swagger::oas3::TagItem::createShared();
      oasTagItem->name = tag->name;
      oasTagItem->description = tag->description;
      if (tag->externalDocs) {
        oasTagItem->externalDocs = generateExternalDocs(*tag->externalDocs);
      }

      result->push_back(oasTagItem);
    }
  }

  // Add placeholder tags that where discovered but were missing in the declaration.
  for (const auto &missingTag : missingTags) {
    auto oasTagItem = oatpp::swagger::oas3::TagItem::createShared();
    oasTagItem->name = missingTag.first;
    result->push_back(oasTagItem);
  }

  return result;
}

oatpp::Object<oas3::ExternalDocumentation> Generator::generateExternalDocs(const ExternalDocumentation &externalDocs) {

  auto oasExternalDocumentation = oatpp::swagger::oas3::ExternalDocumentation::createShared();
  oasExternalDocumentation->url = externalDocs.url;
  oasExternalDocumentation->description = externalDocs.description;

  return oasExternalDocumentation;
}


oatpp::Object<oas3::SecurityScheme> Generator::generateSecurityScheme(const std::shared_ptr<oatpp::swagger::SecurityScheme> &ss) {
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
      oasSS->flows->implicit = oas3::OAuthFlow::createShared();
      oasSS->flows->implicit->tokenUrl = ss->flows->implicit->tokenUrl;
      oasSS->flows->implicit->refreshUrl = ss->flows->implicit->refreshUrl;
      oasSS->flows->implicit->authorizationUrl = ss->flows->implicit->authorizationUrl;
      if(ss->flows->implicit->scopes) {
        oasSS->flows->implicit->scopes = oatpp::Fields<String>({});
        for(const auto &scope : *ss->flows->implicit->scopes) {
          oasSS->flows->implicit->scopes[scope.first] = scope.second;
        }
      }
    }
    if(ss->flows->password) {
      oasSS->flows->password = oas3::OAuthFlow::createShared();
      oasSS->flows->password->tokenUrl = ss->flows->password->tokenUrl;
      oasSS->flows->password->refreshUrl = ss->flows->password->refreshUrl;
      oasSS->flows->password->authorizationUrl = ss->flows->password->authorizationUrl;
      if(ss->flows->password->scopes) {
        oasSS->flows->password->scopes = oatpp::Fields<String>({});
        for(const auto &scope : *ss->flows->password->scopes) {
          oasSS->flows->password->scopes[scope.first] = scope.second;
        }
      }
    }
    if(ss->flows->clientCredentials) {
      oasSS->flows->clientCredentials = oas3::OAuthFlow::createShared();
      oasSS->flows->clientCredentials->tokenUrl = ss->flows->clientCredentials->tokenUrl;
      oasSS->flows->clientCredentials->refreshUrl = ss->flows->clientCredentials->refreshUrl;
      oasSS->flows->clientCredentials->authorizationUrl = ss->flows->clientCredentials->authorizationUrl;
      if(ss->flows->clientCredentials->scopes) {
        oasSS->flows->clientCredentials->scopes = oatpp::Fields<String>({});
        for(const auto &scope : *ss->flows->clientCredentials->scopes) {
          oasSS->flows->clientCredentials->scopes[scope.first] = scope.second;
        }
      }
    }
    if(ss->flows->authorizationCode) {
      oasSS->flows->authorizationCode = oas3::OAuthFlow::createShared();
      oasSS->flows->authorizationCode->tokenUrl = ss->flows->authorizationCode->tokenUrl;
      oasSS->flows->authorizationCode->refreshUrl = ss->flows->authorizationCode->refreshUrl;
      oasSS->flows->authorizationCode->authorizationUrl = ss->flows->authorizationCode->authorizationUrl;
      if(ss->flows->authorizationCode->scopes) {
        oasSS->flows->authorizationCode->scopes = oatpp::Fields<String>({});
        for(const auto &scope : *ss->flows->authorizationCode->scopes) {
          oasSS->flows->authorizationCode->scopes[scope.first] = scope.second;
        }
      }
    }
  }
  return oasSS;
}

Generator::Generator(const std::shared_ptr<Config>& config)
  : m_config(config)
{}

oatpp::Object<oas3::Document> Generator::generateDocument(const std::shared_ptr<oatpp::swagger::DocumentInfo>& docInfo, const Endpoints& endpoints) {
  
  auto document = oas3::Document::createShared();
  document->info = oas3::Info::createFromBaseModel(docInfo->header);
  
  if(docInfo->servers) {
    document->servers = {};

    for(const auto &it : *docInfo->servers) {
      document->servers->push_back(oas3::Server::createFromBaseModel(it));
    }

  }
  
  if (docInfo->externalDocs) {
    document->externalDocs = generateExternalDocs(*docInfo->externalDocs);
  }

  UsedTypes usedTypes;
  UsedSecuritySchemes usedSecuritySchemes;
  UsedTags usedTags;

  document->paths = generatePaths(endpoints, usedTypes, usedSecuritySchemes, usedTags);
  document->tags = generateTags(docInfo->tags, usedTags);
  auto decomposedTypes = decomposeTypes(usedTypes);
  document->components = generateComponents(decomposedTypes, docInfo->securitySchemes, usedSecuritySchemes);

  return document;
  
}

}}
