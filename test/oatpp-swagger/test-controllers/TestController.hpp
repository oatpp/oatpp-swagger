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

ENUM(HelloEnum, v_int16,
     VALUE(V1, 10, "value-1", "description-1"),
     VALUE(V2, 20, "value-2", "description-2"),
     VALUE(V3, 30, "value-3", "description-3"),
     VALUE(V4, 40, "value-4", "description-4"),
     VALUE(V5, 50, "value-5", "description-5")
)

class TaskDto : public oatpp::DTO {
  DTO_INIT(TaskDto, DTO)

  DTO_FIELD(String, description);
  DTO_FIELD(Boolean, done);
};

class UserDto : public oatpp::DTO {

  DTO_INIT(UserDto, DTO)

  DTO_FIELD(Int32, id);
  DTO_FIELD(String, firstName, "first-name");
  DTO_FIELD(String, lastName, "last-name");
  DTO_FIELD_INFO(referral) {
    info->pattern = "^[^\\s]+$";
  };
  DTO_FIELD(String, referral, "referral") = "direct";
  DTO_FIELD(Int32, intVal) = 32;
  DTO_FIELD(List<String>, friends) = { "me", "he", "she" };
  DTO_FIELD_INFO(todoList) {
    info->description = "Task title -> task description";
  };
  DTO_FIELD(Fields<String>, todoList);
  DTO_FIELD(Fields<Fields<String>>, todoTree);
  DTO_FIELD(Fields<Int64>, numberList);
  DTO_FIELD(Fields<Object<TaskDto>>, tasksByTitle);
  DTO_FIELD(Fields<Object<UserDto>>, friendsByNickname);
  DTO_FIELD(Fields<List<String>>, taskForFriends);

  DTO_FIELD(Enum<HelloEnum>, helloEnum) = HelloEnum::V1;

};

#include OATPP_CODEGEN_END(DTO)

#include OATPP_CODEGEN_BEGIN(DTO)

struct VPoint {
  v_int32 x;
  v_int32 y;
  v_int32 z;
};

namespace __class {
  class PointClass;
}

typedef oatpp::data::mapping::type::Primitive<VPoint, __class::PointClass> Point;

namespace __class {

class PointClass {
private:

  class PointDto : public oatpp::DTO {

    DTO_INIT(PointDto, DTO)

    DTO_FIELD(Int32, x);

    DTO_FIELD(Int32, y);

    DTO_FIELD(Int32, z);

  };

  class Inter : public oatpp::Type::Interpretation<Point, oatpp::Object<PointDto>> {
  public:

    oatpp::Object<PointDto> interpret(const Point &value) const override {
      auto dto = PointDto::createShared();
      dto->x = value->x;
      dto->y = value->y;
      dto->z = value->z;
      return dto;
    }

    Point reproduce(const oatpp::Object<PointDto> &value) const override {
      return Point({value->x, value->y, value->z});
    }

  };

public:

  static const oatpp::ClassId CLASS_ID;

  static oatpp::Type *getType() {
    static oatpp::Type type(
      CLASS_ID, nullptr, nullptr,
      {
        {"test", new Inter()}
      }
    );
    return &type;
  }

};

const oatpp::ClassId PointClass::CLASS_ID("test::Point");

}

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
    info->addResponse<Object<UserDto>>(Status::CODE_200, "text/html");
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
    info->addConsumes<Object<UserDto>>("application/json");
    info->addResponse<Object<UserDto>>(Status::CODE_200, "application/json");
  }
  ENDPOINT("POST", "demo/api/users", createUser,
           BODY_DTO(Object<UserDto>, userDto)) {
    return createDtoResponse(Status::CODE_200, UserDto::createShared());
  }


  ENDPOINT_INFO(putUser) {
    info->summary = "Update User by userId";
    info->addConsumes<Object<UserDto>>("application/json");
    info->addResponse<Object<UserDto>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
  }
  ENDPOINT("PUT", "demo/api/users/{userId}", putUser,
           PATH(Int32, userId),
           BODY_DTO(Object<UserDto>, userDto)) {
    userDto->id = userId;
    return createDtoResponse(Status::CODE_200, UserDto::createShared());
  }


  ENDPOINT_INFO(getUserById) {
    info->summary = "Get one User by userId";
    info->addResponse<Object<UserDto>>(Status::CODE_200, "application/json", "User found for id");
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
    info->addResponse<List<Object<UserDto>>>(Status::CODE_200, "application/json");
  }
  ENDPOINT("GET", "demo/api/users", getUsers) {
    return createDtoResponse(Status::CODE_200, List<Object<UserDto>>::createShared());
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


  ENDPOINT_INFO(getPoint) {
    info->summary = "get custom type Point";
    info->addResponse<Point>(Status::CODE_200, "application/json");
  }
  ENDPOINT("GET", "demo/api/point", getPoint) {
    Point p = std::make_shared<VPoint>();
    p->x = 1;
    p->y = 2;
    p->z = 3;
    return createDtoResponse(Status::CODE_200, p);
  }

  /**
   *  Finish ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_END(ApiController)

};


#endif //OATPP_SWAGGER_TESTCONTROLLER_HPP
