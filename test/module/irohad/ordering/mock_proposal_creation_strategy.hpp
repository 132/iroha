/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_MOCK_PROPOSAL_CREATION_STRATEGY_HPP
#define IROHA_MOCK_PROPOSAL_CREATION_STRATEGY_HPP

#include "ordering/ordering_service_proposal_creation_strategy.hpp"

#include <gmock/gmock.h>

namespace iroha {
  namespace ordering {
    class MockProposalCreationStrategy : public ProposalCreationStrategy {
     public:
      MOCK_METHOD1(onCollaborationOutcome, void(const PeerList &));
      MOCK_METHOD2(shouldCreateRound,
                   void(RoundType, const std::function<void()> &));
      MOCK_METHOD2(onProposalRequest,
                   boost::optional<RoundType>(const PeerType &, RoundType));
    };
  }  // namespace ordering
}  // namespace iroha

#endif  // IROHA_MOCK_PROPOSAL_CREATION_STRATEGY_HPP