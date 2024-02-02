//
// Created by centforever on 2023/3/19.
//

#include "TestData.h"


TestData::TestData() {
  auto sharing_platform = FOREVER::AndroidNetWorkSharingPlatform();
  context_ = std::make_unique<FOREVER::ContextImpl>((sharing_platform));
}

TestData::~TestData() {
  context_->GetConnectivityManager()->UnregisterConnectionListener("mgg");
  context_.reset();
}

void TestData::TestExec() {
  context_->GetConnectivityManager()->RegisterConnectionListener(
            "mgg", [](FOREVER::ConnectionType type,
                   bool is_lan_connected) {
  });
}