#!/bin/sh
if [ "$PLATFORM_NAME" = "macosx" ]; then
INNER_TARGET="blitcurveMetal-macOS"
else
INNER_TARGET="blitcurveMetal"
fi
if [ "$PLATFORM_NAME" = "iphonesimulator" ]; then
SDK_ARGS="-sdk iphonesimulator"
else
SDK_ARKS = ""
fi
# we want to use BUILT_PRODUCTS_DIR directly.  This avoids a lot of tricky problems
# about how to predict where the output will be on a per-target per-architecture basis
xcodebuild build -project ${BLITCURVE_DIR}/blitcurveMetal.xcodeproj -configuration ${CONFIGURATION} -target "$INNER_TARGET" ${SDK_ARGS} BUILT_PRODUCTS_DIR=${BUILT_PRODUCTS_DIR}
