#pragma once

#include <functional>
#include <deque>

namespace RunningMan
{

typedef std::function<void()> Task;

class PromiseEngine 
{
public:
  static void enqueue(Task);
  static void run();
  static bool isDone();
private:
  static bool done;
  static std::deque<Task> tasks;
};

} // end namespace RunningMan
