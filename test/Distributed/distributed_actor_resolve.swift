// RUN: %target-typecheck-verify-swift -enable-experimental-distributed
// REQUIRES: concurrency
// REQUIRES: distributed

import _Distributed

/// Use the existential wrapper as the default actor transport.
@available(SwiftStdlib 5.6, *)
typealias DefaultDistributedActorSystem = AnyDistributedActorSystem

@available(SwiftStdlib 5.6, *)
distributed actor Capybara { }

@available(SwiftStdlib 5.6, *)
func test(id: ID, transport: AnyDistributedActorSystem) async throws {
  let _: Capybara = try Capybara.resolve(identity, using: transport)
}
