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

/**
 *  This is the general model for swagger info. It is used as a base model
 *  to generate API info for a particular API specification. Ex. for OAS3
 */

#ifndef oatpp_swagger_Config_hpp
#define oatpp_swagger_Config_hpp

#include "oatpp/core/collection/ListMap.hpp"
#include "oatpp/core/collection/LinkedList.hpp"
#include "oatpp/core/Types.hpp"

namespace oatpp { namespace swagger {
  
struct Contact {
  
  static std::shared_ptr<Contact> createShared() {
    return std::make_shared<Contact>();
  }
  
  String name;
  String url;
  String email;
  
};

struct License {
  
  static std::shared_ptr<License> createShared() {
    return std::make_shared<License>();
  }
  
  String name;
  String url;
  
};

struct DocumentHeader {
  
  static std::shared_ptr<DocumentHeader> createShared() {
    return std::make_shared<DocumentHeader>();
  }
  
  String title;
  String description;
  String termsOfService;
  std::shared_ptr<Contact> contact;
  std::shared_ptr<License> license;
  String version;
  
};

struct ServerVariable {
  
  static std::shared_ptr<ServerVariable> createShared() {
    return std::make_shared<ServerVariable>();
  }
  
  String description;
  std::shared_ptr<oatpp::collection::LinkedList<String>> enumValues;
  String defaultValue;
  
};

struct Server {
  
  static std::shared_ptr<Server> createShared() {
    return std::make_shared<Server>();
  }
  
  String url;
  String description;
  std::shared_ptr<oatpp::collection::ListMap<String, std::shared_ptr<ServerVariable>>> variables;
  
};
  
class DocumentInfo {
public:
  
  static std::shared_ptr<DocumentInfo> createShared() {
    return std::make_shared<DocumentInfo>();
  }
  
  std::shared_ptr<DocumentHeader> header;
  std::shared_ptr<oatpp::collection::LinkedList<std::shared_ptr<Server>>> servers;
  
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
    
    Builder& setTitle(const oatpp::String& title) {
      getHeader()->title = title;
      return *this;
    }
    
    Builder& setDescription(const oatpp::String& description) {
      getHeader()->description = description;
      return *this;
    }
    
    Builder& setTermsOfService(const oatpp::String& termsOfService) {
      getHeader()->termsOfService = termsOfService;
      return *this;
    }
    
    Builder& setVersion(const oatpp::String& version) {
      getHeader()->version = version;
      return *this;
    }
    
    Builder& setContactName(const oatpp::String& name) {
      getContact()->name = name;
      return *this;
    }
    
    Builder& setContactUrl(const oatpp::String& url) {
      getContact()->url = url;
      return *this;
    }
    
    Builder& setContactEmail(const oatpp::String& email) {
      getContact()->email = email;
      return *this;
    }
    
    Builder& setLicenseName(const oatpp::String& name) {
      getLicense()->name = name;
      return *this;
    }
    
    Builder& setLicenseUrl(const oatpp::String& url) {
      getLicense()->url = url;
      return *this;
    }
    
    Builder& addServer(const std::shared_ptr<Server>& server) {
      if(!m_servers) {
        m_servers = oatpp::collection::LinkedList<std::shared_ptr<Server>>::createShared();
      }
      m_servers->pushBack(server);
      return *this;
    }
    
    Builder& addServer(const oatpp::String& url, const oatpp::String& description) {
      auto server = Server::createShared();
      server->url = url;
      server->description = description;
      return addServer(server);
    }
    
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
