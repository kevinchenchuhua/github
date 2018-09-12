This repo is created to track the changes in webrtc.

To build chome, please follow the steps below:

1. follow the steps in https://chromium.googlesource.com/chromium/src/+/master/docs/linux_build_instructions.md#Install-additional-build-dependencies to get chromium source code

2. create a branch based on the tag 67.0.3396.99
   git checkout -b newbranch 67.0.3396.99

3. replace ./src/third_party/webrtc/ inside newbranch with this repo

4. compile chrome
   ninja -v -C out/Default/ chrome


