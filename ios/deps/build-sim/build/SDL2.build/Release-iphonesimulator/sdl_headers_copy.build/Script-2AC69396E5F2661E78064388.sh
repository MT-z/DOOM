#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim
  /opt/homebrew/bin/cmake -E copy_if_different /Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_misc.h /Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/include/SDL2/SDL_misc.h
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim
  /opt/homebrew/bin/cmake -E copy_if_different /Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_misc.h /Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/include/SDL2/SDL_misc.h
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim
  /opt/homebrew/bin/cmake -E copy_if_different /Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_misc.h /Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/include/SDL2/SDL_misc.h
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim
  /opt/homebrew/bin/cmake -E copy_if_different /Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_misc.h /Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/include/SDL2/SDL_misc.h
fi

