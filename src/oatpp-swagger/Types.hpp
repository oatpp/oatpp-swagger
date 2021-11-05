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

#ifndef oatpp_swagger_Types_hpp
#define oatpp_swagger_Types_hpp

#include "oatpp/core/data/mapping/type/Type.hpp"
#include "oatpp/core/Types.hpp"

namespace oatpp { namespace swagger {
  
namespace __class {

  /**
   * Binary type class info.
   */
  class Binary {
  private:

    static oatpp::Type* createType() {
      oatpp::Type::Info info;
      info.nameQualifier = "binary";
      return new oatpp::Type(CLASS_ID, info);
    }

  public:

    /**
     * CLASS_NAME = `"string"`.
     */
    static const oatpp::data::mapping::type::ClassId CLASS_ID;

    /**
     * Get type information.
     * @return - &id:oatpp::data::mapping::type::Type;.
     */
    static oatpp::Type* getType(){
      static oatpp::Type* type = createType();
      return type;
    }
    
  };
  
}

/**
 * Typedef for Binary. It is used to indicate file upload in Swagger-UI. <br>
 * Usage example: `info->addConsumes<oatpp::swagger::Binary>("application/octet-stream");`.<br>
 * For more info see: [Endpoint Annotation And API Documentation](/docs/components/api-controller/#endpoint-annotation-and-api-documentation).
 */
typedef oatpp::data::mapping::type::ObjectWrapper<std::string, __class::Binary> Binary;
  
}}

#endif /* oatpp_swagger_Types_hpp */
