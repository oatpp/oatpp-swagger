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

#include "oatpp/core/collection/ListMap.hpp"
#include "oatpp/core/collection/LinkedList.hpp"
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
  std::shared_ptr<oatpp::collection::LinkedList<String>> enumValues;

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
  std::shared_ptr<oatpp::collection::ListMap<String, std::shared_ptr<ServerVariable>>> variables;
  
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
  std::shared_ptr<oatpp::collection::LinkedList<std::shared_ptr<Server>>> servers;

  /**
   * Document Info Builder.
   */
  class Builder {
  private:
    
    std::shared_ptr<DocumentHeader> m_header;
    std::shared_ptr<oatpp::collection::LinkedList<std::shared_ptr<Server>>> m_servers;
    
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
        m_servers = oatpp::collection::LinkedList<std::shared_ptr<Server>>::createShared();
      }
      m_servers->pushBack(server);
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
     * Build Document Info.
     * @return - &l:DocumentInfo;.
     */
    std::shared_ptr<DocumentInfo> build() {
      auto document = DocumentInfo::createShared();
      document->header = m_header;
      document->servers = m_servers;
      return document;
    }
    
  };
  
};
  
}}

#endif /* oatpp_swagger_Config_hpp */
