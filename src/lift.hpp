#include <functional>

#define LIFT(func) \
   [](auto&&... args) \
   {\
      return func(std::forward<decltype(args)>(args)...);\
   }