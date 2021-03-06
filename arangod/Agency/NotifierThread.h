////////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2014-2016 ArangoDB GmbH, Cologne, Germany
/// Copyright 2004-2014 triAGENS GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Andreas Streichardt
////////////////////////////////////////////////////////////////////////////////

#ifndef ARANGOD_CONSENSUS_NOTIFIER_COMMON_H
#define ARANGOD_CONSENSUS_NOTIFIER_COMMON_H 1

#include <memory>

#include "Basics/Common.h"
#include "Basics/ConditionVariable.h"
#include "Basics/Thread.h"

#include <velocypack/Builder.h>
#include <velocypack/velocypack-aliases.h>

namespace arangodb {
namespace consensus {

struct NotificationResult {
  bool success;
  std::string endpoint;
};

class NotifierThread : public Thread {
 public:
  NotifierThread(const std::string& path, std::shared_ptr<VPackBuilder> body,
                 const std::vector<std::string>& endpoints);
  virtual ~NotifierThread();

  void run() override;
  bool start();

  /// @brief Orderly shutdown of thread
  void beginShutdown() override;

 private:
  void scheduleNotification(const std::string&);

  arangodb::basics::ConditionVariable _cv;

  std::string _path;
  std::shared_ptr<VPackBuilder> _body;
  std::vector<std::string> _endpoints;

  std::vector<NotificationResult> _openResults;
};
}
}

#endif  // ARANGODB_CONSENSUS_NOTIFIER_COMMON_H
