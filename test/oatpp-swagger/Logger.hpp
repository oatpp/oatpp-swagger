//
// Created by Leonid on 2019-03-01.
//

#ifndef OATPP_SWAGGER_LOGGER_HPP
#define OATPP_SWAGGER_LOGGER_HPP

#include "oatpp/core/concurrency/SpinLock.hpp"
#include "oatpp/core/base/Environment.hpp"
#include <iostream>

class Logger : public oatpp::base::Logger {
private:
  oatpp::concurrency::SpinLock m_lock;
public:

  void log(v_int32 priority, const std::string& tag, const std::string& message) override {
    std::lock_guard<oatpp::concurrency::SpinLock> lock(m_lock);
    std::cout << tag << ":" << message << "\n";
  }

};


#endif //OATPP_SWAGGER_LOGGER_HPP
