/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_MOCK_STORAGE_HPP
#define IROHA_MOCK_STORAGE_HPP

#include "ametsuchi/storage.hpp"

#include <gmock/gmock.h>
#include "ametsuchi/block_storage_factory.hpp"
#include "ametsuchi/mutable_storage.hpp"
#include "ametsuchi/temporary_wsv.hpp"

namespace iroha {
  namespace ametsuchi {

    class MockStorage : public Storage {
     public:
      MOCK_CONST_METHOD0(getWsvQuery, std::shared_ptr<WsvQuery>());
      MOCK_CONST_METHOD0(getBlockQuery, std::shared_ptr<BlockQuery>());
      MOCK_METHOD0(
          createCommandExecutor,
          expected::Result<std::unique_ptr<CommandExecutor>, std::string>());
      MOCK_METHOD1(
          createTemporaryWsv,
          std::unique_ptr<TemporaryWsv>(std::shared_ptr<CommandExecutor>));
      MOCK_METHOD1(
          createMutableStorage,
          std::unique_ptr<MutableStorage>(std::shared_ptr<CommandExecutor>));
      MOCK_CONST_METHOD0(createPeerQuery,
                         boost::optional<std::shared_ptr<PeerQuery>>());
      MOCK_CONST_METHOD0(createBlockQuery,
                         boost::optional<std::shared_ptr<BlockQuery>>());
      MOCK_CONST_METHOD2(
          createQueryExecutor,
          boost::optional<std::shared_ptr<QueryExecutor>>(
              std::shared_ptr<PendingTransactionStorage>,
              std::shared_ptr<shared_model::interface::QueryResponseFactory>));
      MOCK_METHOD1(doCommit, CommitResult(MutableStorage *storage));
      MOCK_CONST_METHOD0(preparedCommitEnabled, bool());
      MOCK_METHOD1(
          commitPrepared,
          CommitResult(std::shared_ptr<const shared_model::interface::Block>));
      MOCK_METHOD1(insertBlock,
                   bool(std::shared_ptr<const shared_model::interface::Block>));
      MOCK_METHOD2(
          createMutableStorage,
          std::unique_ptr<MutableStorage>(std::shared_ptr<CommandExecutor>,
                                          BlockStorageFactory &));

      MOCK_METHOD1(insertPeer,
                   expected::Result<void, std::string>(
                       const shared_model::interface::Peer &));
      MOCK_METHOD0(reset, void());
      MOCK_METHOD0(resetWsv, expected::Result<void, std::string>());
      MOCK_METHOD0(resetPeers, void());
      MOCK_METHOD0(dropStorage, void());
      MOCK_METHOD0(freeConnections, void());
      MOCK_METHOD1(prepareBlock_, void(std::unique_ptr<TemporaryWsv> &));

      void prepareBlock(std::unique_ptr<TemporaryWsv> wsv) override {
        // gmock workaround for non-copyable parameters
        prepareBlock_(wsv);
      }

      rxcpp::observable<std::shared_ptr<const shared_model::interface::Block>>
      on_commit() override {
        return notifier.get_observable();
      }
      CommitResult commit(std::unique_ptr<MutableStorage> storage) override {
        return doCommit(storage.get());
      }
      rxcpp::subjects::subject<
          std::shared_ptr<const shared_model::interface::Block>>
          notifier;
    };

  }  // namespace ametsuchi
}  // namespace iroha

#endif  // IROHA_MOCK_STORAGE_HPP
