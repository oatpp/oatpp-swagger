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

namespace oatpp { namespace swagger { namespace oas3 {
 
Schema::ObjectWrapper Generator::generateSchemaForTypeObject(const oatpp::data::mapping::type::Type* type, bool linkSchema, UsedTypes& usedTypes) {
  
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
  auto result = Schema::createShared();
  result->type = "array";
  result->items = generateSchemaForType(*type->params.begin(), linkSchema, usedTypes);
  return result;
}
  
Schema::ObjectWrapper Generator::generateSchemaForType(const oatpp::data::mapping::type::Type* type, bool linkSchema, UsedTypes& usedTypes) {
  
  auto typeName = type->name;
  if(typeName == oatpp::data::mapping::type::__class::String::CLASS_NAME){
    auto result = Schema::createShared();
    result->type = "string";
    return result;
  } else if(typeName == oatpp::data::mapping::type::__class::Int32::CLASS_NAME){
    auto result = Schema::createShared();
    result->type = "integer";
    result->format = "int32";
    return result;
  } else if(typeName == oatpp::data::mapping::type::__class::Int64::CLASS_NAME){
    auto result = Schema::createShared();
    result->type = "integer";
    result->format = "int64";
    return result;
  } else if(typeName == oatpp::data::mapping::type::__class::Float32::CLASS_NAME){
    auto result = Schema::createShared();
    result->type = "number";
    result->format = "float";
    return result;
  } else if(typeName == oatpp::data::mapping::type::__class::Float64::CLASS_NAME){
    auto result = Schema::createShared();
    result->type = "number";
    result->format = "double";
    return result;
  } else if(typeName == oatpp::data::mapping::type::__class::Boolean::CLASS_NAME){
    auto result = Schema::createShared();
    result->type = "boolean";
    return result;
  } else if(typeName == oatpp::data::mapping::type::__class::AbstractObject::CLASS_NAME){
    return generateSchemaForTypeObject(type, linkSchema, usedTypes);
  } else if(typeName == oatpp::data::mapping::type::__class::AbstractList::CLASS_NAME){
    return generateSchemaForTypeList(type, linkSchema, usedTypes);
  } else if(typeName == oatpp::data::mapping::type::__class::AbstractListMap::CLASS_NAME){
    // TODO
  } else {
    auto result = Schema::createShared();
    result->type = type->name;
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
        if(endpointInfo.body.type->name == oatpp::data::mapping::type::__class::AbstractObject::CLASS_NAME) {
          body->content->put("application/json", mediaType);
        } else if(endpointInfo.body.type->name == oatpp::data::mapping::type::__class::AbstractList::CLASS_NAME) {
          body->content->put("application/json", mediaType);
        } else if(endpointInfo.body.type->name == oatpp::data::mapping::type::__class::AbstractListMap::CLASS_NAME) {
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
  
void Generator::generatePathItemData(const std::shared_ptr<Endpoint>& endpoint, const PathItem::ObjectWrapper& pathItem, UsedTypes& usedTypes) {
  
  auto info = endpoint->info;
  
  if(info) {
    
    auto operation = PathItemOperation::createShared();
    operation->operationId = info->name;
    operation->summary = info->summary;
    operation->description = info->description;
    
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
    
    if(!pathItem->parameters) {
      
      pathItem->parameters = pathItem->parameters->createShared();

      addParamsToParametersList(pathItem->parameters, info->headers, "header", usedTypes);
      addParamsToParametersList(pathItem->parameters, info->pathParams, "path", usedTypes);
      addParamsToParametersList(pathItem->parameters, info->queryParams, "query", usedTypes);
      
    }
    
  }
  
}
  
Generator::Paths::ObjectWrapper Generator::generatePaths(const std::shared_ptr<Endpoints>& endpoints, UsedTypes& usedTypes) {
  
  auto result = Paths::createShared();
  
  auto curr = endpoints->getFirstNode();
  while (curr != nullptr) {
    auto endpoint = curr->getData();
    
    if(endpoint->info) {
      oatpp::String path = endpoint->info->path;
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
      generatePathItemData(endpoint, pathItem, usedTypes);
    }
    
    curr = curr->getNext();
  }
  
  return result;
  
}
  
void Generator::decomposeObject(const oatpp::data::mapping::type::Type* type, UsedTypes& decomposedTypes) {
  
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
  decomposeType(*type->params.begin(), decomposedTypes);
}

void Generator::decomposeMap(const oatpp::data::mapping::type::Type* type, UsedTypes& decomposedTypes) {
  // TODO
}
  
void Generator::decomposeType(const oatpp::data::mapping::type::Type* type, UsedTypes& decomposedTypes) {
  auto typeName = type->name;
  if(typeName == oatpp::data::mapping::type::__class::AbstractObject::CLASS_NAME){
    decomposeObject(type, decomposedTypes);
  } else if(typeName == oatpp::data::mapping::type::__class::AbstractList::CLASS_NAME){
    decomposeList(type, decomposedTypes);
  } else if(typeName == oatpp::data::mapping::type::__class::AbstractListMap::CLASS_NAME){
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
  
Components::ObjectWrapper Generator::generateComponents(const UsedTypes& decomposedTypes) {
  
  auto result = Components::createShared();
  result->schemas = result->schemas->createShared();
  
  auto it = decomposedTypes.begin();
  while (it != decomposedTypes.end()) {
    UsedTypes schemas; ///< dummy
    result->schemas->put(it->first, generateSchemaForType(it->second, false, schemas));
    it ++;
  }
  
  return result;
  
}
  
Document::ObjectWrapper Generator::generateDocument(const std::shared_ptr<oatpp::swagger::DocumentInfo>& docInfo, const std::shared_ptr<Endpoints>& endpoints) {
  
  auto document = oas3::Document::createShared();
  document->info = Info::createFromBaseModel(docInfo->header);
  
  if(docInfo->servers) {
    document->servers = document->servers->createShared();
    
    auto curr = docInfo->servers->getFirstNode();
    while (curr != nullptr) {
      document->servers->pushBack(Server::createFromBaseModel(curr->getData()));
      curr = curr->getNext();
    }
    
  }
  
  UsedTypes usedTypes;
  document->paths = generatePaths(endpoints, usedTypes);
  auto decomposedTypes = decomposeTypes(usedTypes);
  document->components = generateComponents(decomposedTypes);
  
  return document;
  
}
  
}}}
