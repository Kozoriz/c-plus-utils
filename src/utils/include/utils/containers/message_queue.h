#pragma once

#include "utils/containers/queue.h"

#include "utils/threads/synchronization/conditional_variable.h"
#include "utils/threads/synchronization/atomic.h"
#include "utils/threads/synchronization/auto_lock.h"

namespace utils {

namespace {
// TODO use profiler
const UInt new_messages_wait_default_timeout = 10u;
}

template <typename MessageType>
class MessageQueue {
 public:
  void PushMessage(const MessageType& message);
  MessageType GetMessage();
  void WaitNewMessages();
  bool IsFinalyzing() const;
  void Finalyze();
  bool IsEmpty() const;

  MessageQueue();
  ~MessageQueue();

  size_t Size() const;

 private:
  Queue<MessageType> queue_;
  synchronization::Lock queue_lock_;
  synchronization::Lock wait_lock_;
  synchronization::ConditionalVariable new_messages_cond_var_;
  synchronization::AtomicBool finalyzing_;
};

template <typename MessageType>
void MessageQueue<MessageType>::PushMessage(const MessageType& message) {
  if (IsFinalyzing()) {
    return;
  }
  {
    synchronization::AutoLock auto_lock(queue_lock_);
    queue_.push(message);
  }
  new_messages_cond_var_.Broadcast();
}

template <typename MessageType>
MessageType MessageQueue<MessageType>::GetMessage() {
  if (IsEmpty()) {
    return MessageType();
  }
  synchronization::AutoLock auto_lock(queue_lock_);
  MessageType message = queue_.front();
  queue_.pop();
  return message;
}

template <typename MessageType>
void MessageQueue<MessageType>::WaitNewMessages() {
  while (!IsFinalyzing() && queue_.empty()) {
    new_messages_cond_var_.WaitFor(queue_lock_,
                                   new_messages_wait_default_timeout);
  }
}

template <typename MessageType>
bool MessageQueue<MessageType>::IsFinalyzing() const {
  return finalyzing_;
}

template <typename MessageType>
void MessageQueue<MessageType>::Finalyze() {
  finalyzing_ = true;
}

template <typename MessageType>
bool MessageQueue<MessageType>::IsEmpty() const {
  return queue_.empty();
}

template <typename MessageType>
MessageQueue<MessageType>::MessageQueue()
    : finalyzing_(false) {}

template <typename MessageType>
MessageQueue<MessageType>::~MessageQueue() {
  Finalyze();
}

template <typename MessageType>
size_t MessageQueue<MessageType>::Size() const {
  return queue_.size();
}
}
