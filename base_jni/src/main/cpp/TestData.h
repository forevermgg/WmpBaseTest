//
// Created by centforever on 2023/3/19.
//

#ifndef TEST_TESTDATA_H
#define TEST_TESTDATA_H

#include <map>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <string>
#include "network/network_sharing_platform.h"
#include "network/android_network_sharing_platform.h"
#include "network/context_impl.h"
#include "network/network_monitor.h"
#include <memory>

class TestData : public std::enable_shared_from_this<TestData> {
 public:
  TestData();
  ~TestData();
  void TestExec();

private:
  std::unique_ptr<FOREVER::Context> context_;
};

#endif  // TEST_TESTDATA_H
