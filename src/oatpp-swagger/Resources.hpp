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

#ifndef oatpp_swagger_Resources_hpp
#define oatpp_swagger_Resources_hpp

#include "oatpp/Types.hpp"
#include "oatpp/data/stream/BufferStream.hpp"
#include "oatpp/data/stream/FileStream.hpp"
#include "oatpp/data/resource/Resource.hpp"

#include <unordered_map>

namespace oatpp { namespace swagger {

/**
 * This class is responsible for loading Swagger-UI resources.
 */
class Resources {
private:
  oatpp::String m_resDir;
  bool m_streaming;
  std::unordered_map<oatpp::String, std::shared_ptr<data::resource::Resource>> m_resources;
private:
  void addResource(const oatpp::String& fileName);
  bool hasEnding(std::string fullString, std::string const &ending) const;
public:
  /**
   * Constructor.
   * @param resDir - directory containing swagger-ui resources.
   * @param streaming - whether to stream resources from file or to cache in-memory
   */
  Resources(const oatpp::String& resDir, bool streaming = false);

  /**
   * Legacy function.
   * Use std::make_shared<Resources>(resDir) directly
   * @param resDir
   * @param streaming
   * @return
   */
  static std::shared_ptr<Resources> loadResources(const oatpp::String& resDir, bool streaming = false) {
    return std::make_shared<Resources>(resDir, false);
  }

  /**
   * Override swagger resource.
   * @param filename
   * @param resource
   */
  void overrideResource(const oatpp::String& filename, const std::shared_ptr<data::resource::Resource>& resource);

  /**
   * Get resource by filename.
   * @param filename - name of the resource file.
   * @return - &id:oatpp::data::resource::Resource;
   */
  std::shared_ptr<data::resource::Resource> getResource(const oatpp::String& filename) const;

  /**
   * Get resource data.
   * @param filename
   * @return
   */
  oatpp::String getResourceData(const oatpp::String& filename) const;

  /**
   * Returns the MIME type for a given filename
   * @param filename to return the MIME type
   * @return a MIME type
   */
  std::string getMimeType(const std::string &filename) const;

  /**
   * Returns true if this is a streaming ressource instance.
   * @return
   */
  bool isStreaming() const;

};
  
}}

#endif /* oatpp_swagger_Resources_hpp */
