/*
 *  Copyright 2004 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "rtc_base/ssladapter.h"

#include "rtc_base/openssladapter.h"

///////////////////////////////////////////////////////////////////////////////

namespace rtc {

SSLConfig::SSLConfig() = default;
SSLConfig::SSLConfig(const SSLConfig&) = default;
SSLConfig::~SSLConfig() = default;

///////////////////////////////////////////////////////////////////////////////

SSLAdapterFactory* SSLAdapterFactory::Create() {
  return new OpenSSLAdapterFactory();
}

SSLAdapter* SSLAdapter::Create(AsyncSocket* socket) {
  return new OpenSSLAdapter(socket);
}

///////////////////////////////////////////////////////////////////////////////

bool InitializeSSL() {
  return OpenSSLAdapter::InitializeSSL();
}

bool CleanupSSL() {
  return OpenSSLAdapter::CleanupSSL();
}

///////////////////////////////////////////////////////////////////////////////

}  // namespace rtc