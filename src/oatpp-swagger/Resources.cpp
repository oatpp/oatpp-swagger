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

#include "Resources.hpp"

#include "oatpp/data/resource/File.hpp"
#include "oatpp/data/resource/InMemoryData.hpp"
#include "oatpp/base/Log.hpp"

#include <fstream>

namespace oatpp { namespace swagger {
  
Resources::Resources(const oatpp::String& resDir, bool streaming)
  : m_resDir(resDir)
  , m_streaming(streaming)
{

  if(!resDir || resDir->empty()) {
    throw std::runtime_error("[oatpp::swagger::Resources::Resources()]: Invalid resDir path. Please specify full path to oatpp-swagger/res folder");
  }

  addResource("favicon-16x16.png");
  addResource("favicon-32x32.png");
  addResource("index.css");
  addResource("index.html");
  addResource("oauth2-redirect.html");
  addResource("swagger-initializer.js");
  addResource("swagger-ui-bundle.js");
  addResource("swagger-ui-bundle.js.map");
  addResource("swagger-ui-es-bundle-core.js");
  addResource("swagger-ui-es-bundle-core.js.map");
  addResource("swagger-ui-es-bundle.js");
  addResource("swagger-ui-es-bundle.js.map");
  addResource("swagger-ui-standalone-preset.js");
  addResource("swagger-ui-standalone-preset.js.map");
  addResource("swagger-ui.css");
  addResource("swagger-ui.css.map");
  addResource("swagger-ui.js");
  addResource("swagger-ui.js.map");

}
  
void Resources::addResource(const oatpp::String& fileName) {

  if(m_streaming) {
    m_resources[fileName] = std::make_shared<data::resource::File>(m_resDir, fileName);
  } else {
    auto path = data::resource::File::concatDirAndName(m_resDir, fileName);
    auto data = oatpp::String::loadFromFile(path->c_str());
    if(!data) {
      OATPP_LOGe("oatpp::swagger::Resources::addResource()", "Can't load file '{}'", path);
      throw std::runtime_error("[oatpp::swagger::Resources::addResource()]: Can't load file. Please make sure you specified full path to oatpp-swagger/res folder");
    }
    m_resources[fileName] = std::make_shared<data::resource::InMemoryData>(data);
  }
}

void Resources::overrideResource(const oatpp::String& filename, const std::shared_ptr<data::resource::Resource>& resource) {
  m_resources[filename] = resource;
}

std::shared_ptr<data::resource::Resource> Resources::getResource(const oatpp::String& filename) const {

  auto it = m_resources.find(filename);
  if(it != m_resources.end()) {
    return it->second;
  }
  throw std::runtime_error("[oatpp::swagger::Resources::getResource()]: Resource file not found. "
                           "Please make sure: "
                           "1. You are using correct version of oatpp-swagger. "
                           "2. oatpp-swagger/res is not empty. "
                           "3. You specified correct full path to oatpp-swagger/res folder");
}

oatpp::String Resources::getResourceData(const oatpp::String& filename) const {
  auto resource = getResource(filename);
  if(resource->getInMemoryData() && resource->getKnownSize() > 0) {
    return resource->getInMemoryData();
  }
  v_char8 buffer[1024];
  oatpp::data::stream::BufferOutputStream ss(1024);
  oatpp::data::stream::transfer(resource->openInputStream(), &ss, 0, buffer, 1024);
  return ss.toString();
}

bool Resources::hasEnding(std::string fullString, std::string const &ending) const {
    std::transform(fullString.begin(), fullString.end(), fullString.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

std::string Resources::getMimeType(const std::string &filename) const {
    if (hasEnding(filename, ".html")) return "text/html";
    if (hasEnding(filename, ".jpg")) return "image/jpeg";
    if (hasEnding(filename, ".jpeg")) return "image/jpeg";
    if (hasEnding(filename, ".png")) return "image/png";
    if (hasEnding(filename, ".gif")) return "image/gif";
    if (hasEnding(filename, ".css")) return "text/css";
    if (hasEnding(filename, ".js")) return "text/javascript";
    if (hasEnding(filename, ".xml")) return "text/xml";
    return "text/plain";
}

bool Resources::isStreaming() const {
  return m_streaming;
}

}}
