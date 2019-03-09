//
// Created by Leonid on 2019-03-09.
//

#ifndef OATPP_SWAGGER_CONTROLLERTEST_HPP
#define OATPP_SWAGGER_CONTROLLERTEST_HPP

#include "oatpp-test/UnitTest.hpp"

namespace oatpp { namespace test { namespace swagger {

class ControllerTest : public UnitTest {
public:

  ControllerTest():UnitTest("TEST[swagger::ControllerTest]"){}
  void onRun() override;

};

}}}

#endif //OATPP_SWAGGER_CONTROLLERTEST_HPP
