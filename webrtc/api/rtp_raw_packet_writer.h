/*
 *  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef MODULES_LOG_RTP_RAW_PACKET_DUMP_WRITER_H_
#define MODULES_LOG_RTP_RAW_PACKET_DUMP_WRITER_H_

#include "test/rtp_file_writer.h"

namespace webrtc {
namespace test {

static const uint16_t kPacketHeaderSize = 8;
static const char kFirstLine[] = "#!rtpplay1.0 0.0.0.0/0\n";

// Write RTP packets to file in rtpdump format, as documented at:
// http://www.cs.columbia.edu/irt/software/rtptools/
class RtpPacketDumpWriter : public RtpFileWriter {
 public:
  explicit RtpPacketDumpWriter(FILE* file) : file_(file) {
    RTC_CHECK(file_ != NULL);
    Init();
  }
  virtual ~RtpPacketDumpWriter() {
    if (file_ != NULL) {
      fclose(file_);
      file_ = NULL;
    }
  }

  bool WritePacket(const RtpPacket* packet) override {
    uint16_t len = static_cast<uint16_t>(packet->length + kPacketHeaderSize);
    uint16_t plen = static_cast<uint16_t>(packet->original_length);
    uint32_t offset = packet->time_ms;
    RTC_CHECK(WriteUint16(len));
    RTC_CHECK(WriteUint16(plen));
    RTC_CHECK(WriteUint32(offset));
    return fwrite(packet->data, sizeof(uint8_t), packet->length, file_) ==
           packet->length;
  }

  static RtpFileWriter* Create(FileFormat format,
                                     const std::string& filename) {
      FILE* file = fopen(filename.c_str(), "wb");
      if (file == NULL) {
        printf("ERROR: Can't open file: %s\n", filename.c_str());
        return NULL;
      }
      switch (format) {
        case kRtpDump:
          return new RtpPacketDumpWriter(file);
      }
      fclose(file);
      return NULL;
  }

 private:
  bool Init() {
    fprintf(file_, "%s", kFirstLine);

    RTC_CHECK(WriteUint32(0));
    RTC_CHECK(WriteUint32(0));
    RTC_CHECK(WriteUint32(0));
    RTC_CHECK(WriteUint16(0));
    RTC_CHECK(WriteUint16(0));

    return true;
  }

  bool WriteUint32(uint32_t in) {
    // Loop through shifts = {24, 16, 8, 0}.
    for (int shifts = 24; shifts >= 0; shifts -= 8) {
      uint8_t tmp = static_cast<uint8_t>((in >> shifts) & 0xFF);
      if (fwrite(&tmp, sizeof(uint8_t), 1, file_) != 1)
        return false;
    }
    return true;
  }

  bool WriteUint16(uint16_t in) {
    // Write 8 MSBs.
    uint8_t tmp = static_cast<uint8_t>((in >> 8) & 0xFF);
    if (fwrite(&tmp, sizeof(uint8_t), 1, file_) != 1)
      return false;
    // Write 8 LSBs.
    tmp = static_cast<uint8_t>(in & 0xFF);
    if (fwrite(&tmp, sizeof(uint8_t), 1, file_) != 1)
      return false;
    return true;
  }

  FILE* file_;

  RTC_DISALLOW_COPY_AND_ASSIGN(RtpPacketDumpWriter);
};


}  // namespace test
}  // namespace webrtc

#endif