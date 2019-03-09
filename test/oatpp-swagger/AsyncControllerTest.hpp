//
// Created by Leonid on 2019-03-09.
//

#ifndef OATPP_SWAGGER_ASYNCCONTROLLERTEST_HPP
#define OATPP_SWAGGER_ASYNCCONTROLLERTEST_HPP

#include "oatpp-test/UnitTest.hpp"

namespace oatpp { namespace test { namespace swagger {

class AsyncControllerTest : public UnitTest {
public:

  AsyncControllerTest():UnitTest("TEST[swagger::AsyncControllerTest]"){}
  void onRun() override;

};

}}}


#endif //OATPP_SWAGGER_ASYNCCONTROLLERTEST_HPP
