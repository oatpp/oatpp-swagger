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
 
Schema::ObjectWrapper Generator::generateSchemaForTypeObject(const oatpp::data::mapping::type::Type* type, bool linkSchema, UsedSchemas& usedSchemas) {
  
  auto result = Schema::createShared();
  if(linkSchema) {
  
    result->ref = oatpp::String("#/components/schemas/") + type->nameQualifier;
    usedSchemas[type->nameQualifier] = type;
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
      result->properties->put(p->name, generateSchemaForType(p->type, linkSchema, usedSchemas));
    }
    
    return result;
  }
  
}
  
Schema::ObjectWrapper Generator::generateSchemaForTypeList(const oatpp::data::mapping::type::Type* type, bool linkSchema, UsedSchemas& usedSchemas) {
  auto result = Schema::createShared();
  result->type = "array";
  result->items = generateSchemaForType(*type->params.begin(), linkSchema, usedSchemas);
  return result;
}
  
Schema::ObjectWrapper Generator::generateSchemaForType(const oatpp::data::mapping::type::Type* type, bool linkSchema, UsedSchemas& usedSchemas) {
  
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
    return generateSchemaForTypeObject(type, linkSchema, usedSchemas);
  } else if(typeName == oatpp::data::mapping::type::__class::AbstractList::CLASS_NAME){
    return generateSchemaForTypeList(type, linkSchema, usedSchemas);
  } else if(typeName == oatpp::data::mapping::type::__class::AbstractListMap::CLASS_NAME){
    // TODO
  }
  
  return Schema::createShared();
  
}
  
RequestBody::ObjectWrapper Generator::generateRequestBody(const Endpoint::Info& endpointInfo, bool linkSchema, UsedSchemas& usedSchemas) {
  
  if(endpointInfo.consumes.size() > 0) {
    
    auto body = RequestBody::createShared();
    body->description = "request body";
    body->content = body->content->createShared();
    
    auto it = endpointInfo.consumes.begin();
    while (it != endpointInfo.consumes.end()) {
      
      auto mediaType = MediaTypeObject::createShared();
      mediaType->schema = generateSchemaForType(it->schema, linkSchema, usedSchemas);
      
      body->content->put(it->contentType, mediaType);
      
      it++;
    }
    
    return body;
    
  } else {
  
    if(endpointInfo.body.name != nullptr && endpointInfo.body.type != nullptr) {
      
      auto body = RequestBody::createShared();
      body->description = "request body";
      
      auto mediaType = MediaTypeObject::createShared();
      mediaType->schema = generateSchemaForType(endpointInfo.body.type, linkSchema, usedSchemas);
      
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
  
Generator::Fields<OperationResponse::ObjectWrapper>::ObjectWrapper Generator::generateResponses(const Endpoint::Info& endpointInfo, bool linkSchema, UsedSchemas& usedSchemas) {
  
  auto responses = Fields<OperationResponse::ObjectWrapper>::createShared();
  
  if(endpointInfo.responses.size() > 0) {
    
    auto it = endpointInfo.responses.begin();
    while (it != endpointInfo.responses.end()) {
      
      auto mediaType = MediaTypeObject::createShared();
      mediaType->schema = generateSchemaForType(it->second.schema, linkSchema, usedSchemas);
      
      auto response = OperationResponse::createShared();
      response->description = it->first.description;
      response->content = response->content->createShared();
      response->content->put(it->second.contentType, mediaType);
      responses->put(oatpp::utils::conversion::int32ToStr(it->first.code), response);
      
      it++;
    }
    
  } else {
  
    auto mediaType = MediaTypeObject::createShared();
    mediaType->schema = generateSchemaForType(oatpp::String::Class::getType(), linkSchema, usedSchemas);
  
    auto response = OperationResponse::createShared();
    response->description = "success";
    response->content = response->content->createShared();
    response->content->put("test/plain", mediaType);
    responses->put("200", response);
  
  }
  
  return responses;
    
}
  
void Generator::generatePathItemData(const std::shared_ptr<Endpoint>& endpoint, const PathItem::ObjectWrapper& pathItem, UsedSchemas& usedSchemas) {
  
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
    
    operation->responses = generateResponses(*info, true, usedSchemas);
    operation->requestBody = generateRequestBody(*info, true, usedSchemas);
    
    if(!pathItem->parameters) {
      
      pathItem->parameters = pathItem->parameters->createShared();
      
      auto headerIt = info->headers.begin();
      while (headerIt != info->headers.end()) {
        auto param = *headerIt++;
        auto parameter = PathItemParameter::createShared();
        parameter->in = "header";
        parameter->name = param.name;
        parameter->schema = generateSchemaForType(param.type, true, usedSchemas);
        pathItem->parameters->pushBack(parameter);
      }
      
      auto pathIt = info->pathParams.begin();
      while (pathIt != info->pathParams.end()) {
        auto param = *pathIt++;
        auto parameter = PathItemParameter::createShared();
        parameter->in = "path";
        parameter->name = param.name;
        parameter->required = true;
        parameter->schema = generateSchemaForType(param.type, true, usedSchemas);
        pathItem->parameters->pushBack(parameter);
      }
      
    }
    
  }
  
}
  
Generator::Paths::ObjectWrapper Generator::generatePaths(const std::shared_ptr<Endpoints>& endpoints, UsedSchemas& usedSchemas) {
  
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
      generatePathItemData(endpoint, pathItem, usedSchemas);
    }
    
    curr = curr->getNext();
  }
  
  return result;
  
}
  
Components::ObjectWrapper Generator::generateComponents(const UsedSchemas& usedSchemas) {
  
  auto result = Components::createShared();
  result->schemas = result->schemas->createShared();
  
  auto it = usedSchemas.begin();
  while (it != usedSchemas.end()) {
    UsedSchemas schemas; ///< dummy
    result->schemas->put(it->first, generateSchemaForType(it->second, false, schemas));
    it ++;
  }
  
  return result;
  
}
  
Document::ObjectWrapper Generator::generateDocument(const Info::ObjectWrapper& info, const std::shared_ptr<Endpoints>& endpoints) {
  
  auto document = oas3::Document::createShared();
  document->info = info;
  
  UsedSchemas usedSchemas;
  document->paths = generatePaths(endpoints, usedSchemas);
  
  document->components = generateComponents(usedSchemas);
  
  return document;
  
}
  
}}}
