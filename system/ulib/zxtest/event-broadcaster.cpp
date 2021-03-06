// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <utility>

#include <zircon/assert.h>
#include <zxtest/base/event-broadcaster.h>

namespace zxtest {
namespace internal {

namespace {

template <auto Fn, typename ObserverList, typename... Args>
void Broadcast(ObserverList* observers, Args&&... args) {
    for (auto& observer : *observers) {
        (observer->*Fn)(std::forward<Args>(args)...);
    }
}

} // namespace

EventBroadcaster::EventBroadcaster() = default;
EventBroadcaster::EventBroadcaster(EventBroadcaster&&) = default;
EventBroadcaster::~EventBroadcaster() = default;

// Reports before every TestCase is set up.
void EventBroadcaster::OnTestCaseStart(const TestCase& test_case) {
    Broadcast<&LifecycleObserver::OnTestCaseStart>(&lifecycle_observers_, test_case);
}

// Reports before every test starts.
void EventBroadcaster::OnTestStart(const TestCase& test_case, const TestInfo& test) {
    Broadcast<&LifecycleObserver::OnTestStart>(&lifecycle_observers_, test_case, test);
}

// Reports before every test starts.
void EventBroadcaster::OnTestSkip(const TestCase& test_case, const TestInfo& test) {
    Broadcast<&LifecycleObserver::OnTestSkip>(&lifecycle_observers_, test_case, test);
}

// Reports before every TestCase is set up.
void EventBroadcaster::OnTestFailure(const TestCase& test_case, const TestInfo& test) {
    Broadcast<&LifecycleObserver::OnTestFailure>(&lifecycle_observers_, test_case, test);
}

// Reports before every TestCase is set up.
void EventBroadcaster::OnTestSuccess(const TestCase& test_case, const TestInfo& test) {
    Broadcast<&LifecycleObserver::OnTestSuccess>(&lifecycle_observers_, test_case, test);
}
// Reports before every TestCase is torn down.
void EventBroadcaster::OnTestCaseEnd(const TestCase& test_case) {
    Broadcast<&LifecycleObserver::OnTestCaseEnd>(&lifecycle_observers_, test_case);
}

// Adds a lifecycle observer to the registered list of observers.
void EventBroadcaster::Subscribe(LifecycleObserver* observer) {
    ZX_DEBUG_ASSERT_MSG(observer != this, "EventBroadcaster cannot observe itself.");
    ZX_DEBUG_ASSERT_MSG(observer != nullptr, "Canno register nullptr as a LifecycleObserver");
    lifecycle_observers_.push_back(observer);
}

} // namespace internal
} // namespace zxtest
