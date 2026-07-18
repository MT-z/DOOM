#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim
  make -f /Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim
  make -f /Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim
  make -f /Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim
  make -f /Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/CMakeScripts/ReRunCMake.make
fi

