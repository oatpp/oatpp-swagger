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

namespace oatpp { namespace swagger { namespace oas3 {
 
Schema::ObjectWrapper Generator::generateSchemaForTypeObject(const oatpp::data::mapping::type::Type* type) {
  
  auto result = Schema::createShared();
  result->type = "object";
  result->properties = result->properties->createShared();
  
  auto properties = type->properties;
  if(properties->getList().size() == 0) {
    type->creator(); // init type by creating first instance of that type
  }
  
  auto it = properties->getList().begin();
  while (it != properties->getList().end()) {
    auto p = *it ++;
    result->properties->put(p->name, generateSchemaForType(p->type));
  }
  
  return result;
  
}
  
Schema::ObjectWrapper Generator::generateSchemaForTypeList(const oatpp::data::mapping::type::Type* type) {
  auto result = Schema::createShared();
  result->type = "array";
  result->items = generateSchemaForType(*type->params.begin());
  return result;
}
  
Schema::ObjectWrapper Generator::generateSchemaForType(const oatpp::data::mapping::type::Type* type) {
  
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
    return generateSchemaForTypeObject(type);
  } else if(typeName == oatpp::data::mapping::type::__class::AbstractList::CLASS_NAME){
    return generateSchemaForTypeList(type);
  } else if(typeName == oatpp::data::mapping::type::__class::AbstractListMap::CLASS_NAME){
    // TODO
  }
  
  return Schema::createShared();
  
}
  
void Generator::generatePathItemData(const std::shared_ptr<Endpoint>& endpoint, const PathItem::ObjectWrapper& pathItem) {
  
  auto info = endpoint->info;
  
  if(info) {
    
    auto operation = PathItemOperation::createShared();
    operation->operationId = info->name;
    
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
    
    if(!operation->responses) {
      
      operation->responses = operation->responses->createShared();
      
      auto mediaType = MediaTypeObject::createShared();
      mediaType->schema = generateSchemaForType(oatpp::String::Class::getType());
      
      auto response = OperationResponse::createShared();
      response->description = "success";
      response->content = response->content->createShared();
      response->content->put("test/plain", mediaType);
      
      operation->responses->put("200", response);
      
    }
    
    if(info->body.name != nullptr) {
      
      operation->requestBody = RequestBody::createShared();
      operation->requestBody->description = "request body";
      
      auto mediaType = MediaTypeObject::createShared();
      mediaType->schema = generateSchemaForType(info->body.type);
      
      operation->requestBody->content = operation->requestBody->content->createShared();
      operation->requestBody->content->put("application/json", mediaType);
      
    }
    
    if(!pathItem->parameters) {
      
      pathItem->parameters = pathItem->parameters->createShared();
      
      auto headerIt = info->headers.begin();
      while (headerIt != info->headers.end()) {
        auto param = *headerIt++;
        auto parameter = PathItemParameter::createShared();
        parameter->in = "header";
        parameter->name = param.name;
        parameter->schema = generateSchemaForType(param.type);
        pathItem->parameters->pushBack(parameter);
      }
      
      auto pathIt = info->pathParams.begin();
      while (pathIt != info->pathParams.end()) {
        auto param = *pathIt++;
        auto parameter = PathItemParameter::createShared();
        parameter->in = "path";
        parameter->name = param.name;
        parameter->schema = generateSchemaForType(param.type);
        pathItem->parameters->pushBack(parameter);
      }
      
    }
    
  }
  
}
  
Generator::Paths::ObjectWrapper Generator::generatePaths(const std::shared_ptr<Endpoints>& endpoints) {
  
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
      generatePathItemData(endpoint, pathItem);
    }
    
    curr = curr->getNext();
  }
  
  return result;
  
}
  
}}}
