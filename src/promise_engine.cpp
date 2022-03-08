#include "running-man/promise_engine.h"

namespace RunningMan {
void PromiseEngine::enqueue(Task t) {
  done = false;
  tasks.push_back(t);
}

void PromiseEngine::run() {
  for (int jobs = tasks.size(); jobs > 0; jobs--) {
    auto task = tasks.front();
    tasks.pop_front();
    task();
  }
  done = tasks.size() == 0;
}

std::deque<Task> PromiseEngine::tasks = std::deque<Task>();

bool PromiseEngine::done = true;

bool PromiseEngine::isDone() { return done; }
} // end namespace RunningMan
