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
 *  This is the general model for swagger info. It is used as a base model
 *  to generate API info for a particular API specification. Ex. for OAS3
 */

#ifndef oatpp_swagger_Config_hpp
#define oatpp_swagger_Config_hpp

#include "oatpp/core/Types.hpp"

namespace oatpp { namespace swagger {

/**
 * Contact.
 */
struct Contact {

  /**
   * Create shared Contact.
   * @return - 'std::shared_ptr' to Contact.
   */
  static std::shared_ptr<Contact> createShared() {
    return std::make_shared<Contact>();
  }

  /**
   * name.
   */
  String name;

  /**
   * url.
   */
  String url;

  /**
   * email.
   */
  String email;
  
};

/**
 * License.
 */
struct License {

  /**
   * Create shared License.
   * @return - 'std::shared_ptr' to License.
   */
  static std::shared_ptr<License> createShared() {
    return std::make_shared<License>();
  }

  /**
   * name.
   */
  String name;

  /**
   * Url.
   */
  String url;
  
};

/**
 * Document Header.
 */
struct DocumentHeader {

  /**
   * Create shared DocumentHeader.
   * @return - 'std::shared_ptr' to DocumentHeader.
   */
  static std::shared_ptr<DocumentHeader> createShared() {
    return std::make_shared<DocumentHeader>();
  }

  /**
   * Document title.
   */
  String title;

  /**
   * Document description.
   */
  String description;

  /**
   * Terms of service.
   */
  String termsOfService;

  /**
   * &l:Contact;.
   */
  std::shared_ptr<Contact> contact;

  /**
   * &l:License;.
   */
  std::shared_ptr<License> license;

  /**
   * Version.
   */
  String version;
  
};

/**
 * Server variable.
 */
struct ServerVariable {

  /**
   * Create shared ServerVariable.
   * @return - 'std::shared_ptr' to ServerVariable.
   */
  static std::shared_ptr<ServerVariable> createShared() {
    return std::make_shared<ServerVariable>();
  }

  /**
   * Description.
   */
  String description;

  /**
   * Enum values.
   */
  std::shared_ptr<std::list<String>> enumValues;

  /**
   * Default value.
   */
  String defaultValue;
  
};

/**
 * Server
 */
struct Server {

  /**
   * Create shared Server.
   * @return - 'std::shared_ptr' to Server.
   */
  static std::shared_ptr<Server> createShared() {
    return std::make_shared<Server>();
  }

  /**
   * Url.
   */
  String url;

  /**
   * Description.
   */
  String description;

  /**
   * Variables.
   */
  std::shared_ptr<std::unordered_map<String, std::shared_ptr<ServerVariable>>> variables;
  
};

/**
 * OAuth flow Object https://swagger.io/specification/#oauthFlowObject
 */
struct OAuthFlowObject {

  static std::shared_ptr<OAuthFlowObject> createShared() {
    return std::make_shared<OAuthFlowObject>();
  }

  /**
   * Authorization Url
   */
  oatpp::String authorizationUrl;

  /**
   * Token Url
   */
  oatpp::String tokenUrl;

  /**
   * Refresh Url
   */
  oatpp::String refreshUrl;

  /**
   * Scopes
   */
  std::shared_ptr<std::unordered_map<String, String>> scopes;
};

/**
 * OAuth Flows Object https://swagger.io/specification/#oauthFlowObject
 */
struct OAuthFlowsObject {

  static std::shared_ptr<OAuthFlowsObject> createShared() {
    return std::make_shared<OAuthFlowsObject>();
  }

  /**
   * Implicit
   */
  std::shared_ptr<OAuthFlowObject> implicit;

  /**
   * Password
   */
  std::shared_ptr<OAuthFlowObject> password;

  /**
   * Client Credentials
   */
  std::shared_ptr<OAuthFlowObject> clientCredentials;

  /**
   * Authorization Code
   */
  std::shared_ptr<OAuthFlowObject> authorizationCode;
};

/**
 * Security Scheme object - https://swagger.io/specification/#securitySchemeObject .
 */
struct SecuritySchemeObject {

  static std::shared_ptr<SecuritySchemeObject> createShared() {
    return std::make_shared<SecuritySchemeObject>();
  }

  /**
   * Type
   */
  oatpp::String type;

  /**
   * Description
   */
  oatpp::String description;

  /**
   * Name
   */
  oatpp::String name;

  /**
   * In
   */
  oatpp::String in;

  /**
   * Scheme
   */
  oatpp::String scheme;

  /**
   * Bearer Format.
   */
  oatpp::String bearerFormat;

  /**
   * Flows.
   */
  std::shared_ptr<OAuthFlowsObject> flows;

  /**
   * Open Id Connect Url.
   */
  oatpp::String openIdConnectUrl;

};

/**
 * Document Info.
 */
class DocumentInfo {
public:

  /**
   * Create shared DocumentInfo.
   * @return - 'std::shared_ptr' to DocumentInfo.
   */
  static std::shared_ptr<DocumentInfo> createShared() {
    return std::make_shared<DocumentInfo>();
  }

  /**
   * &l:DocumentHeader;.
   */
  std::shared_ptr<DocumentHeader> header;

  /**
   * List of &l:Server;.
   */
  std::shared_ptr<std::list<std::shared_ptr<Server>>> servers;

  /**
   * Map of &id:oatpp::String; to &l:SecuritySchemeObject;.
   */
   std::shared_ptr<std::unordered_map<oatpp::String, std::shared_ptr<SecuritySchemeObject>>> ssos;

  /**
   * SecuritySchemeObject Builder.
   */
  class SSOBuilder {

   private:
    oatpp::String m_type;
    oatpp::String m_description;
    oatpp::String m_name;
    oatpp::String m_in;
    oatpp::String m_scheme;
    oatpp::String m_bearerFormat;
    std::shared_ptr<OAuthFlowsObject> m_flows;
    oatpp::String m_openIdConnectUrl;

    std::shared_ptr<OAuthFlowsObject> getFlows() {
      if(!m_flows) {
        m_flows = OAuthFlowsObject::createShared();
      }
      return m_flows;
    }

   public:

    /**
     * Convenience default Basic-Authorization SSO.
     * @return - &l:SecuritySchemeObject;.
     */
    static std::shared_ptr<SecuritySchemeObject> DefaultBasicAuthorizationSSO() {
      auto sso = SecuritySchemeObject::createShared();
      sso->description = "Default HTTP Basic Authorization";
      sso->scheme = "basic";
      sso->type = "http";
      sso->name = nullptr;
      sso->bearerFormat = nullptr;
      sso->flows = nullptr;
      sso->in = nullptr;
      sso->openIdConnectUrl = nullptr;
      return sso;
    }

    /**
     * Convenience default Bearer-Authorization SSO.
     * @param bearerFormat - for documentation purposes.
     * @return - &l:SecuritySchemeObject;.
     */
    static std::shared_ptr<SecuritySchemeObject> DefaultBearerAuthorizationSSO(const oatpp::String& bearerFormat = nullptr) {
      auto sso = SecuritySchemeObject::createShared();
      sso->description = "Default HTTP Basic Authorization";
      sso->scheme = "bearer";
      sso->type = "http";
      sso->name = nullptr;
      sso->bearerFormat = bearerFormat;
      sso->flows = nullptr;
      sso->in = nullptr;
      sso->openIdConnectUrl = nullptr;
      return sso;
    }

    /**
     * Set type.
     * @param type
     * @return
     */
    SSOBuilder& setType(const String& type) {
      m_type = type;
      return *this;
    }

    /**
     * Set description.
     * @param description
     * @return
     */
    SSOBuilder& setDescription(const String& description) {
      m_description = description;
      return *this;
    }

    /**
     * Set name.
     * @param name
     * @return
     */
    SSOBuilder& setName(const String& name) {
      m_name = name;
      return *this;
    }

    /**
     * Set 'in'.
     * @param in
     * @return
     */
    SSOBuilder& setIn(const String& in) {
      m_in = in;
      return *this;
    }

    /**
     * Set scheme.
     * @param scheme
     * @return
     */
    SSOBuilder& setScheme(const String& scheme) {
      m_scheme = scheme;
      return *this;
    }

    /**
     * Set bearer format.
     * @param bearerFormat
     * @return
     */
    SSOBuilder& setBearerFormat(const String& bearerFormat) {
      m_bearerFormat = bearerFormat;
      return *this;
    }

    /**
     * Set implicit flow.
     * @param flow - &l:OAuthFlowObject;.
     * @return
     */
    SSOBuilder& setImplicitFlow(const std::shared_ptr<OAuthFlowObject>& flow){
      getFlows()->implicit = flow;
      return *this;
    }

    /**
     * Set password flow.
     * @param flow - &l:OAuthFlowObject;.
     * @return
     */
    SSOBuilder& setPasswordFlow(const std::shared_ptr<OAuthFlowObject>& flow){
      getFlows()->password = flow;
      return *this;
    }

    /**
     * Set client credentials flow.
     * @param flow - &l:OAuthFlowObject;.
     * @return
     */
    SSOBuilder& setClientCredentialsFlow(const std::shared_ptr<OAuthFlowObject>& flow){
      getFlows()->clientCredentials = flow;
      return *this;
    }

    /**
     * Set authorization code flow.
     * @param flow - &l:OAuthFlowObject;.
     * @return
     */
    SSOBuilder& setAuthorizationCodeFlow(const std::shared_ptr<OAuthFlowObject>& flow){
      getFlows()->authorizationCode = flow;
      return *this;
    }

    /**
     * Set open id connect url.
     * @param openIdConnectUrl
     * @return
     */
    SSOBuilder& setOpenIdConnectUrl(const String& openIdConnectUrl) {
      m_openIdConnectUrl = openIdConnectUrl;
      return *this;
    }

    /**
     * Build SecuritySchemeObject.
     * @return - `std::shared_ptr` to &l:SecuritySchemeObject;.
     */
    std::shared_ptr<SecuritySchemeObject> build() {
      auto sso = SecuritySchemeObject::createShared();
      sso->description = m_description;
      sso->scheme = m_scheme;
      sso->type = m_type;
      sso->name = m_name;
      sso->bearerFormat = m_bearerFormat;
      sso->flows = m_flows;
      sso->in = m_in;
      sso->openIdConnectUrl = m_openIdConnectUrl;
      return sso;
    }
  };

  /**
   * OAuthFlowObject Builder.
   */
  class OAFOBuilder {
   private:
    oatpp::String m_authorizationUrl;
    oatpp::String m_tokenUrl;
    oatpp::String m_refreshUrl;
    std::shared_ptr<std::unordered_map<String, String>> m_scopes;

    std::shared_ptr<std::unordered_map<String, String>> getScopes() {
      if(!m_scopes) {
        m_scopes = std::make_shared<std::unordered_map<String, String>>();
      }
      return m_scopes;
    }

   public:

    /**
     * Set authorization url.
     * @param authorizationUrl
     * @return
     */
    OAFOBuilder& setAuthorizationUrl(const String& authorizationUrl) {
      m_authorizationUrl = authorizationUrl;
      return *this;
    }

    /**
     * Set token url.
     * @param tokenUrl
     * @return
     */
    OAFOBuilder& setTokenUrl(const String& tokenUrl) {
      m_tokenUrl = tokenUrl;
      return *this;
    }

    /**
     * Set refresh url.
     * @param refreshUrl
     * @return
     */
    OAFOBuilder& setRefreshUrl(const String& refreshUrl) {
      m_refreshUrl = refreshUrl;
      return *this;
    }

    /**
     * add scope.
     * @param name
     * @param scope
     * @return
     */
    OAFOBuilder& addScope(const String& name, const String& scope){
      getScopes()->at(name) = scope;
      return *this;
    }

    /**
     * Build OAuthFlowObject.
     * @return - &l:OAuthFlowObject;.
     */
    std::shared_ptr<OAuthFlowObject> build() {
      auto oafo = OAuthFlowObject::createShared();
      oafo->authorizationUrl = m_authorizationUrl;
      oafo->refreshUrl = m_refreshUrl;
      oafo->tokenUrl = m_tokenUrl;
      oafo->scopes = m_scopes;
      return oafo;
    }
  };

  /**
   * Document Info Builder.
   */
  class Builder {
  private:
    
    std::shared_ptr<DocumentHeader> m_header;
    std::shared_ptr<std::list<std::shared_ptr<Server>>> m_servers;
    std::shared_ptr<std::unordered_map<oatpp::String, std::shared_ptr<SecuritySchemeObject>>> m_ssos;

  private:

    std::shared_ptr<DocumentHeader> getHeader() {
      if(!m_header) {
        m_header = DocumentHeader::createShared();
      }
      return m_header;
    }
    
    std::shared_ptr<Contact> getContact() {
      auto header = getHeader();
      if(!header->contact) {
        header->contact = Contact::createShared();
      }
      return header->contact;
    }
    
    std::shared_ptr<License> getLicense() {
      auto header = getHeader();
      if(!header->license) {
        header->license = License::createShared();
      }
      return header->license;
    }

    std::shared_ptr<std::unordered_map<oatpp::String, std::shared_ptr<SecuritySchemeObject>>> getSSOS() {
      if(!m_ssos) {
        m_ssos = std::make_shared<std::unordered_map<oatpp::String, std::shared_ptr<SecuritySchemeObject>>>();
      }
      return m_ssos;
    }
    
  public:

    /**
     * Set document title.
     * @param title - document title. &id:oatpp::String;.
     * @return - &l:DocumentInfo::Builder;.
     */
    Builder& setTitle(const oatpp::String& title) {
      getHeader()->title = title;
      return *this;
    }

    /**
     * Set document description.
     * @param description
     * @return - &l:DocumentInfo::Builder;.
     */
    Builder& setDescription(const oatpp::String& description) {
      getHeader()->description = description;
      return *this;
    }

    /**
     * Set terms of service.
     * @param termsOfService
     * @return - &l:DocumentInfo::Builder;.
     */
    Builder& setTermsOfService(const oatpp::String& termsOfService) {
      getHeader()->termsOfService = termsOfService;
      return *this;
    }

    /**
     * Set version.
     * @param version
     * @return - &l:DocumentInfo::Builder;.
     */
    Builder& setVersion(const oatpp::String& version) {
      getHeader()->version = version;
      return *this;
    }

    /**
     * Set contact name.
     * @param name
     * @return - &l:DocumentInfo::Builder;.
     */
    Builder& setContactName(const oatpp::String& name) {
      getContact()->name = name;
      return *this;
    }

    /**
     * Set contact url.
     * @param url
     * @return - &l:DocumentInfo::Builder;.
     */
    Builder& setContactUrl(const oatpp::String& url) {
      getContact()->url = url;
      return *this;
    }

    /**
     * Set contact email.
     * @param email
     * @return - &l:DocumentInfo::Builder;.
     */
    Builder& setContactEmail(const oatpp::String& email) {
      getContact()->email = email;
      return *this;
    }

    /**
     * Set license name.
     * @param name
     * @return - &l:DocumentInfo::Builder;.
     */
    Builder& setLicenseName(const oatpp::String& name) {
      getLicense()->name = name;
      return *this;
    }

    /**
     * Set license url.
     * @param url
     * @return - &l:DocumentInfo::Builder;.
     */
    Builder& setLicenseUrl(const oatpp::String& url) {
      getLicense()->url = url;
      return *this;
    }

    /**
     * Add &l:Server;.
     * @param server - &l:Server;.
     * @return - &l:DocumentInfo::Builder;.
     */
    Builder& addServer(const std::shared_ptr<Server>& server) {
      if(!m_servers) {
        m_servers = std::make_shared<std::list<std::shared_ptr<Server>>>();
      }
      m_servers->push_back(server);
      return *this;
    }

    /**
     * Add &l:Server;.
     * @param url
     * @param description
     * @return - &l:DocumentInfo::Builder;.
     */
    Builder& addServer(const oatpp::String& url, const oatpp::String& description) {
      auto server = Server::createShared();
      server->url = url;
      server->description = description;
      return addServer(server);
    }

    /**
     * Add &l:SecuritySchemeObject;.
     * When you are using the `AUTHENTICATION()` Endpoint-Macro you must add an [SecuritySchemeObject](https://swagger.io/specification/#securitySchemeObject).
     * For basic-authentication you can use the default &id:oatpp::swagger::DocumentInfo::SSOBuilder::DefaultBasicAuthorizationSSO;.
     * For more complex authentication schemes you can use the &id:oatpp::swagger::DocumentInfo::SSOBuilder; builder.
     * Don't forget to add `info->addSecurityRequirement(name)` to your `ENDPOINT_INFO()` macro!
     * @param name
     * @param sso
     * @return - &l:DocumentInfo::Builder;.
     */
    Builder& addSSO(const oatpp::String& name, const std::shared_ptr<SecuritySchemeObject> &sso) {
      getSSOS()->operator[](name) = sso; //ugly but works
      return *this;
    }

    /**
     * Build Document Info.
     * @return - &l:DocumentInfo;.
     */
    std::shared_ptr<DocumentInfo> build() {
      auto document = DocumentInfo::createShared();
      document->header = m_header;
      document->servers = m_servers;
      document->ssos = m_ssos;
      return document;
    }
    
  };
  
};
  
}}

#endif /* oatpp_swagger_Config_hpp */
