//
// Created by Leonid on 2019-03-01.
//

#ifndef OATPP_SWAGGER_TESTCONTROLLER_HPP
#define OATPP_SWAGGER_TESTCONTROLLER_HPP

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/core/data/mapping/type/Object.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

// DTO //-----------------------------------------------------------------------------

#include OATPP_CODEGEN_BEGIN(DTO)

class UserDto : public oatpp::Object {

DTO_INIT(UserDto, Object)

  DTO_FIELD(Int32, id);
  DTO_FIELD(String, firstName, "first-name");
  DTO_FIELD(String, lastName, "last-name");
  DTO_FIELD(List<String>, friends) = List<String>::createShared();

};

#include OATPP_CODEGEN_END(DTO)

// Handler //-------------------------------------------------------------------------

class MyAuthorizationObject : public oatpp::web::server::handler::AuthorizationObject {
 public:

  MyAuthorizationObject(v_int64 pId, const oatpp::String& user)
      : id(pId)
      , userName(user)
  {}

  v_int64 id;
  oatpp::String userName;

};

class MyBasicAuthorizationHandler : public oatpp::web::server::handler::BasicAuthorizationHandler {
 public:

  std::shared_ptr<AuthorizationObject> authorize(const oatpp::String& userId, const oatpp::String& password) override {
    if(userId == "foo" && password == "bar") {
      return std::make_shared<MyAuthorizationObject>(1337, userId);
    }
    return nullptr;
  }

};

// Controller //----------------------------------------------------------------------

class TestController : public oatpp::web::server::api::ApiController {
public:
  TestController(const std::shared_ptr<ObjectMapper>& objectMapper)
    : oatpp::web::server::api::ApiController(objectMapper)
  {
    m_defaultAuthorizationHandler = std::make_shared<MyBasicAuthorizationHandler>();
  }
public:

  static std::shared_ptr<TestController> createShared(const std::shared_ptr<ObjectMapper>& objectMapper){
    return std::make_shared<TestController>(objectMapper);
  }

  /**
   *  Begin ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_BEGIN(ApiController)

  ENDPOINT_INFO(root) {
    info->summary = "Index.html page";
    info->addResponse<UserDto>(Status::CODE_200, "text/html");
  }
  ENDPOINT("GET", "/", root) {
    const char* html =
      "<html lang='en'>"
      "<head>"
      "<meta charset=utf-8/>"
      "</head>"
      "<body>"
      "<p>Hello CRUD example project!</p>"
      "<a href='swagger/ui'>Checkout Swagger-UI page</a>"
      "</body>"
      "</html>";
    auto response = createResponse(Status::CODE_200, html);
    response->putHeader(Header::CONTENT_TYPE, "text/html");
    return response;
  }

  ENDPOINT_INFO(createUser) {
    info->summary = "Create new User";
    info->addConsumes<UserDto>("application/json");
    info->addResponse<UserDto>(Status::CODE_200, "application/json");
  }
  ENDPOINT("POST", "demo/api/users", createUser,
           BODY_DTO(UserDto, userDto)) {
    return createDtoResponse(Status::CODE_200, UserDto::createShared());
  }


  ENDPOINT_INFO(putUser) {
    info->summary = "Update User by userId";
    info->addConsumes<UserDto>("application/json");
    info->addResponse<UserDto>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
  }
  ENDPOINT("PUT", "demo/api/users/{userId}", putUser,
           PATH(Int32, userId),
           BODY_DTO(UserDto, userDto)) {
    userDto->id = userId;
    return createDtoResponse(Status::CODE_200, UserDto::createShared());
  }


  ENDPOINT_INFO(getUserById) {
    info->summary = "Get one User by userId";
    info->addResponse<UserDto>(Status::CODE_200, "application/json", "User found for id");
    info->addResponse<String>(Status::CODE_404, "text/plain", "User not found");
    info->pathParams.add("someId", String::Class::getType()).description = "Some ID:)";
    info->pathParams["userId"].description = "User Id";
  }
  ENDPOINT("GET", "demo/api/users/{userId}/{a}", getUserById,
           PATH(Int32, userId), PATH(Int32, a)) {
    return createDtoResponse(Status::CODE_200, UserDto::createShared());
  }


  ENDPOINT_INFO(getUsers) {
    info->summary = "get all stored users";
    info->addResponse<List<UserDto>>(Status::CODE_200, "application/json");
  }
  ENDPOINT("GET", "demo/api/users", getUsers) {
    return createDtoResponse(Status::CODE_200, List<UserDto>::createShared());
  }


  ENDPOINT_INFO(deleteUser) {
    info->summary = "Delete User by userId";
    info->addResponse<String>(Status::CODE_200, "text/plain");
    info->addResponse<String>(Status::CODE_404, "text/plain");
  }
  ENDPOINT("DELETE", "demo/api/users/{userId}", deleteUser,
           PATH(Int32, userId)) {
    return createResponse(Status::CODE_200, "User successfully deleted");
  }

  ENDPOINT_INFO(deleteAllUsers) {
    info->summary = "Deletes all users";
    info->addResponse<String>(Status::CODE_200, "text/plain");
    info->addResponse<String>(Status::CODE_401, "text/plain");
    info->addSecurityRequirement("basic_auth");
  }
  ENDPOINT("DELETE", "demo/api/users", deleteAllUsers,
           AUTHORIZATION(std::shared_ptr<MyAuthorizationObject>, authorization)) {
    if(authorization->userName == "john" && authorization->id == 1337) {
      return createResponse(Status::CODE_200, "User successfully deleted");
    } else {
      return createResponse(Status::CODE_201, "Username or password invalid");
    }
  }

  /**
   *  Finish ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_END(ApiController)

};


#endif //OATPP_SWAGGER_TESTCONTROLLER_HPP
