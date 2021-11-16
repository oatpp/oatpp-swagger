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

#include "oatpp/core/Types.hpp"
#include "oatpp/core/data/stream/BufferStream.hpp"
#include "oatpp/core/data/stream/FileStream.hpp"

#include <unordered_map>


namespace oatpp { namespace swagger {

/**
 * This class is responsible for loading Swagger-UI resources.
 */
class Resources {
private:
  oatpp::String m_resDir;
  std::unordered_map<oatpp::String, oatpp::String> m_resources;
  bool m_streaming;
private:
  oatpp::String loadFromFile(const char* fileName);
  void cacheResource(const char* fileName);

  class ReadCallback : public oatpp::data::stream::ReadCallback {
   private:
    oatpp::String m_file;
    oatpp::data::stream::FileInputStream m_stream;

   public:

    ReadCallback(const oatpp::String& file);
    v_io_size read(void *buffer, v_buff_size count, async::Action& action) override;

  };

public:
  /**
   * Constructor.
   * @param resDir - directory containing swagger-ui resources.
   */
  Resources(const oatpp::String& resDir, bool streaming = false);
public:

  /**
   * Load and cache Swagger-UI resources.
   * @param resDir - directory containing swagger-ui resources.
   * @return - `std::shared_ptr` to Resources.
   */
  static std::shared_ptr<Resources> loadResources(const oatpp::String& resDir) {
    auto res = std::make_shared<Resources>(resDir);

    res->cacheResource("favicon-16x16.png");
    res->cacheResource("favicon-32x32.png");
    res->cacheResource("index.html");
    res->cacheResource("oauth2-redirect.html");
    res->cacheResource("swagger-ui-bundle.js");
    res->cacheResource("swagger-ui-bundle.js.map");
    res->cacheResource("swagger-ui-es-bundle-core.js");
    res->cacheResource("swagger-ui-es-bundle-core.js.map");
    res->cacheResource("swagger-ui-es-bundle.js");
    res->cacheResource("swagger-ui-es-bundle.js.map");
    res->cacheResource("swagger-ui-standalone-preset.js");
    res->cacheResource("swagger-ui-standalone-preset.js.map");
    res->cacheResource("swagger-ui.css");
    res->cacheResource("swagger-ui.css.map");
    res->cacheResource("swagger-ui.js");
    res->cacheResource("swagger-ui.js.map");
    
    return res;
  }

  /**
   * Stream Swagger-UI resources directly from disk.
   * @param resDir - directory containing swagger-ui resources.
   * @return - `std::shared_ptr` to Resources.
   */
  static std::shared_ptr<Resources> streamResources(const oatpp::String& resDir) {
    auto res = std::make_shared<Resources>(resDir, true);

    return res;
  }

  /**
   * Get cached resource by filename.
   * @param filename - name of the resource file.
   * @return - &id:oatpp::String; containing resource binary data.
   */
  oatpp::String getResource(const oatpp::String& filename);

  /**
   * Get streamed resource by filename.
   * @param filename - name of the resource file.
   * @return - `std::shared_ptr` to &id:oatpp::data::stream::ReadCallback; containing resource binary data stream."
   */
  std::shared_ptr<ReadCallback> getResourceStream(const oatpp::String& filename);

  /**
   * Returns true if this is a streaming ressource instance.
   * @return
   */
  bool isStreaming() {
    return m_streaming;
  }
};
  
}}

#endif /* oatpp_swagger_Resources_hpp */
