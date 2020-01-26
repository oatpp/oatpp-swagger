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

/**[info]
 * This file contains Open Api 3.0.0 Specification Model.
 */

#ifndef oatpp_swagger_oas3_Model_hpp
#define oatpp_swagger_oas3_Model_hpp

#include "oatpp-swagger/Model.hpp"

#include "oatpp/core/data/mapping/type/Object.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace oatpp { namespace swagger { namespace oas3 {
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Contact.
 */
class Contact : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(Contact, Object)

  /**
   * Name.
   */
  DTO_FIELD(String, name);

  /**
   * Url.
   */
  DTO_FIELD(String, url);

  /**
   * Email.
   */
  DTO_FIELD(String, email);

  /**
   * Create Contact from &id:oatpp::swagger::Contact;.
   * @param model - &id:oatpp::swagger::Contact;
   * @return - Contact.
   */
  static ObjectWrapper createFromBaseModel(const std::shared_ptr<oatpp::swagger::Contact>& model) {
    if(model) {
      auto result = createShared();
      result->name = model->name;
      result->url = model->url;
      result->email = model->email;
      return result;
    }
    return nullptr;
  }
  
};

/**
 * License.
 */
class License : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(License, Object)

  /**
   * Name.
   */
  DTO_FIELD(String, name);

  /**
   * Url.
   */
  DTO_FIELD(String, url);

  /**
   * Create Liscense from &id:oatpp::swagger::License;.
   * @param model - &id:oatpp::swagger::License;.
   * @return - License.
   */
  static ObjectWrapper createFromBaseModel(const std::shared_ptr<oatpp::swagger::License>& model) {
    if(model) {
      auto result = createShared();
      result->name = model->name;
      result->url = model->url;
      return result;
    }
    return nullptr;
  }
  
};

/**
 * Info.
 */
class Info : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(Info, Object)

  /**
   * Title.
   */
  DTO_FIELD(String, title);

  /**
   * Description.
   */
  DTO_FIELD(String, description);

  /**
   * Terms of service.
   */
  DTO_FIELD(String, termsOfService);

  /**
   * &id:oatpp::swagger::oas3::Contact;.
   */
  DTO_FIELD(Contact::ObjectWrapper, contact);

  /**
   * &id:oatpp::swagger::oas3::License;.
   */
  DTO_FIELD(License::ObjectWrapper, license);

  /**
   * Version.
   */
  DTO_FIELD(String, version);

  /**
   * Create Info from &id:oatpp::swagger::DocumentHeader;.
   * @param model - &id:oatpp::swagger::DocumentHeader;.
   * @return - Info.
   */
  static ObjectWrapper createFromBaseModel(const std::shared_ptr<oatpp::swagger::DocumentHeader>& model) {
    if(model) {
      auto result = createShared();
      result->title = model->title;
      result->description = model->description;
      result->termsOfService = model->termsOfService;
      result->contact = Contact::createFromBaseModel(model->contact);
      result->license = License::createFromBaseModel(model->license);
      result->version = model->version;
      return result;
    }
    return nullptr;
  }
  
};

/**
 * Server Variable.
 */
class ServerVariable : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(ServerVariable, Object)

  /**
   * Description.
   */
  DTO_FIELD(String, description);

  /**
   * Enum values.
   */
  DTO_FIELD(List<String>::ObjectWrapper, enumValues, "enum");

  /**
   * Default value.
   */
  DTO_FIELD(String, defaultValue, "default");

  /**
   * Create ServerVariable from &id:oatpp::swagger::ServerVariable;.
   * @param model - &id:oatpp::swagger::ServerVariable;.
   * @return - ServerVariable.
   */
  static ObjectWrapper createFromBaseModel(const std::shared_ptr<oatpp::swagger::ServerVariable>& model) {
    
    if(model) {
      
      auto result = createShared();
      result->description = model->description;
      result->defaultValue = model->defaultValue;
      
      if(model->enumValues) {
        
        result->enumValues = List<String>::createShared();
        for(const auto &it : *model->enumValues) {
          result->enumValues->pushBack(it);
        }
        
      }
      
      return result;
      
    }
    
    return nullptr;
    
  }
  
};

/**
 * Server.
 */
class Server : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(Server, Object)

  /**
   * Url.
   */
  DTO_FIELD(String, url);

  /**
   * Description.
   */
  DTO_FIELD(String, description);

  /**
   * Variables.
   */
  DTO_FIELD(Fields<ServerVariable::ObjectWrapper>::ObjectWrapper, variables);

  /**
   * Create Server from &id:oatpp::swagger::Server;.
   * @param model - &id:oatpp::swagger::Server;.
   * @return - Server.
   */
  static ObjectWrapper createFromBaseModel(const std::shared_ptr<oatpp::swagger::Server>& model) {
    if(model) {
      auto result = createShared();
      result->url = model->url;
      result->description = model->description;
      
      if(model->variables) {
        
        result->variables = Fields<ServerVariable::ObjectWrapper>::createShared();

        for(const auto &it : *model->variables){
          result->variables->put(it.first, ServerVariable::createFromBaseModel(it.second));
        }

      }
      
      return result;
    }
    return nullptr;
  }
  
};

/**
 * Schema.
 */
class Schema : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(Schema, Object)

  /**
   * Type. Ex.: integer, string, etc.
   */
  DTO_FIELD(String, type);

  /**
   * Format. Ex.: int32, int64, etc.
   */
  DTO_FIELD(String, format);

  /**
   * Minimum value.
   */
  DTO_FIELD(Int64, minimum);

  /**
   * Maximum value.
   */
  DTO_FIELD(Int64, maximum);

  /**
   * Map of &id:oatpp::String; to &l:Schema;.
   */
  DTO_FIELD(Fields<Schema::ObjectWrapper>::ObjectWrapper, properties);

  /**
   * Items.
   */
  DTO_FIELD(Schema::ObjectWrapper, items);

  /**
   * Ref.
   */
  DTO_FIELD(String, ref, "$ref");
  
};


/**
 * Media type object.
 */
class MediaTypeObject : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(MediaTypeObject, Object)

  /**
   * &l:Schema;.
   */
  DTO_FIELD(Schema::ObjectWrapper, schema);
  
};

/**
 * OAuth flow Object https://swagger.io/specification/#oauthFlowObject
 */
class OAuthFlow : public oatpp::data::mapping::type::Object {

 DTO_INIT(OAuthFlow, Object)

  /**
   * Authorization url.
   */
  DTO_FIELD(String, authorizationUrl);

  /**
   * Token url.
   */
  DTO_FIELD(String, tokenUrl);

  /**
   * Refresh url.
   */
  DTO_FIELD(String, refreshUrl);

  /**
   * Scopes.
   */
  DTO_FIELD(Fields<String>::ObjectWrapper, scopes);
};

/**
 * OAuth Flows Object https://swagger.io/specification/#oauthFlowObject
 */
class OAuthFlows : public oatpp::data::mapping::type::Object {

 DTO_INIT(OAuthFlows, Object)

  /**
   * Implicit.
   */
  DTO_FIELD(OAuthFlow::ObjectWrapper, implicit);

  /**
   * Password.
   */
  DTO_FIELD(OAuthFlow::ObjectWrapper, password);

  /**
   * Client credentials.
   */
  DTO_FIELD(OAuthFlow::ObjectWrapper, clientCredentials);

  /**
   * Authorization code.
   */
  DTO_FIELD(OAuthFlow::ObjectWrapper, authorizationCode);
};

/**
 * Security Scheme object.
 */
class SecurityScheme : public oatpp::data::mapping::type::Object {

 DTO_INIT(SecurityScheme, Object)

  /**
   * Type.
   */
  DTO_FIELD(String, type);

  /**
   * Description.
   */
  DTO_FIELD(String, description);

  /**
   * Name.
   */
  DTO_FIELD(String, name);

  /**
   * 'In'.
   */
  DTO_FIELD(String, in);

  /**
   * Scheme.
   */
  DTO_FIELD(String, scheme);

  /**
   * Bearer format.
   */
  DTO_FIELD(String, bearerFormat);

  /**
   * Flows.
   */
  DTO_FIELD(OAuthFlows::ObjectWrapper, flows);

  /**
   * Open id connect url.
   */
  DTO_FIELD(String, openIdConnectUrl);

};

/**
 * Operation Response.
 */
class OperationResponse : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(OperationResponse, Object)

  /**
   * Description.
   */
  DTO_FIELD(String, description);

  /**
   * &l:MediaTypeObject;.
   */
  DTO_FIELD(Fields<MediaTypeObject::ObjectWrapper>::ObjectWrapper, content);
  
};

/**
 * Request body.
 */
class RequestBody : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(RequestBody, Object)

  /**
   * Description.
   */
  DTO_FIELD(String, description);

  /**
   * &l:MediaTypeObject;.
   */
  DTO_FIELD(Fields<MediaTypeObject::ObjectWrapper>::ObjectWrapper, content);
  
};

/**
 * Path item parameter.
 */
class PathItemParameter : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(PathItemParameter, Object)

  /**
   * Parameter name.
   */
  DTO_FIELD(String, name);

  /**
   * Parameter description.
   */
  DTO_FIELD(String, description);

  /**
   * Parameter place - "query", "header", "path"
   */
  DTO_FIELD(String, in);

  /**
   * Is Parameter required.
   */
  DTO_FIELD(Boolean, required);

  /**
   * Is Parameter deprecated.
   */
  DTO_FIELD(Boolean, deprecated);

  /**
   * Parameter schema. &l:Schema;.
   */
  DTO_FIELD(Schema::ObjectWrapper, schema);
  
};

/**
 * Path item operation.
 */
class PathItemOperation : public oatpp::data::mapping::type::Object {

  DTO_INIT(PathItemOperation, Object)

  /**
   * Description.
   */
  DTO_FIELD(String, description);

  /**
   * Summary.
   */
  DTO_FIELD(String, summary);

  /**
   * Operation ID.
   */
  DTO_FIELD(String, operationId);

  /**
   * Request Body.
   */
  DTO_FIELD(RequestBody::ObjectWrapper, requestBody);

  /**
   * List of tags.
   */
  DTO_FIELD(List<String>::ObjectWrapper, tags);

  /**
   * Map of &id:oatpp::String; to &l:OperationResponse;.
   */
  DTO_FIELD(Fields<OperationResponse::ObjectWrapper>::ObjectWrapper, responses);

  /**
   * List of &l:PathItemParameter;.
   */
  DTO_FIELD(List<PathItemParameter::ObjectWrapper>::ObjectWrapper, parameters);

  /**
   * Security requirements.
   */
  DTO_FIELD(List<Fields<List<String>::ObjectWrapper>::ObjectWrapper>::ObjectWrapper, security);

};

/**
 * Path item.
 */
class PathItem : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(PathItem, Object)

  /**
   * Operation GET.
   */
  DTO_FIELD(PathItemOperation::ObjectWrapper, operationGet, "get");

  /**
   * Operation PUT.
   */
  DTO_FIELD(PathItemOperation::ObjectWrapper, operationPut, "put");

  /**
   * Operation POST.
   */
  DTO_FIELD(PathItemOperation::ObjectWrapper, operationPost, "post");

  /**
   * Operation DELETE.
   */
  DTO_FIELD(PathItemOperation::ObjectWrapper, operationDelete, "delete");

  /**
   * Operation OPTIONS.
   */
  DTO_FIELD(PathItemOperation::ObjectWrapper, operationOptions, "options");

  /**
   * Operation HEAD.
   */
  DTO_FIELD(PathItemOperation::ObjectWrapper, operationHead, "head");

  /**
   * Operation PATCH.
   */
  DTO_FIELD(PathItemOperation::ObjectWrapper, operationPatch, "patch");

  /**
   * Operation TRACE.
   */
  DTO_FIELD(PathItemOperation::ObjectWrapper, operationTrace, "trace");
  //DTO_FIELD(List<PathItemParameter::ObjectWrapper>::ObjectWrapper, parameters);
  
};

/**
 * Component.
 */
class Components : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(Components, Object)

  /**
   * Map of &id:oatpp::String; to &l:Schema;.
   */
  DTO_FIELD(Fields<Schema::ObjectWrapper>::ObjectWrapper, schemas);

  /**
   * Map of &id:oatpp::String; to &l:SecurityScheme;.
   */
  DTO_FIELD(Fields<SecurityScheme::ObjectWrapper>::ObjectWrapper, securitySchemes);

};

/**
 * Document.
 */
class Document : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(Document, Object)

  /**
   * Open Api Version.
   */
  DTO_FIELD(String, openapi, "openapi") = "3.0.0";

  /**
   * &l:Info;.
   */
  DTO_FIELD(Info::ObjectWrapper, info);

  /**
   * List of &l:Server;.
   */
  DTO_FIELD(List<Server::ObjectWrapper>::ObjectWrapper, servers);

  /**
   * Map of &id:oatpp::String; to &l:PathItem;.
   */
  DTO_FIELD(Fields<PathItem::ObjectWrapper>::ObjectWrapper, paths);

  /**
   * &l:Components;.
   */
  DTO_FIELD(Components::ObjectWrapper, components);
  
};
  
#include OATPP_CODEGEN_END(DTO)
}}}

#endif /* oatpp_swagger_oas3_Model_hpp */
