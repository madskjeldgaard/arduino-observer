#pragma once

#include "Observer.hpp"
#include <memory>
#include <vector>
#include <Arduino.h>

namespace obs {

template <typename T> class Observable {
public:
  void notify(T &source, const String &field_name) {
    for (auto observer : mObservers) {
      observer->field_changed(source, field_name);
    }
  }

  void subscribe(Observer<T> &observer) { mObservers.push_back(&observer); }

  void unsubscribe(Observer<T> &observer) {
    mObservers.erase(
        std::remove(mObservers.begin(), mObservers.end(), &observer),
        mObservers.end());
  }

private:
  // FIXME: shared_ptr
  std::vector<Observer<T> *> mObservers{};
};
} // namespace obs
