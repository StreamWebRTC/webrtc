/*
 *  Copyright (c) 2023 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef API_TEST_VIDEO_TEST_VIDEO_TRACK_SOURCE_H_
#define API_TEST_VIDEO_TEST_VIDEO_TRACK_SOURCE_H_

#include "absl/types/optional.h"
#include "api/media_stream_interface.h"
#include "api/notifier.h"
#include "api/sequence_checker.h"
#include "api/video/recordable_encoded_frame.h"
#include "api/video/video_frame.h"
#include "api/video/video_sink_interface.h"
#include "api/video/video_source_interface.h"
#include "rtc_base/system/no_unique_address.h"
#include "rtc_base/thread_annotations.h"

namespace webrtc {
namespace test {

// Video source that can be used as input for tests.
class TestVideoTrackSource : public Notifier<VideoTrackSourceInterface> {
 public:
  explicit TestVideoTrackSource(bool remote);
  ~TestVideoTrackSource() override = default;

  void SetState(SourceState new_state);

  SourceState state() const override;
  bool remote() const override { return remote_; }

  bool is_screencast() const override { return false; }
  absl::optional<bool> needs_denoising() const override {
    return absl::nullopt;
  }

  bool GetStats(Stats* stats) override { return false; }

  void AddOrUpdateSink(rtc::VideoSinkInterface<VideoFrame>* sink,
                       const rtc::VideoSinkWants& wants) override;
  void RemoveSink(rtc::VideoSinkInterface<VideoFrame>* sink) override;

  bool SupportsEncodedOutput() const override { return false; }
  void GenerateKeyFrame() override {}
  void AddEncodedSink(
      rtc::VideoSinkInterface<RecordableEncodedFrame>* sink) override {}
  void RemoveEncodedSink(
      rtc::VideoSinkInterface<RecordableEncodedFrame>* sink) override {}

  // Starts producing video.
  virtual void Start() = 0;

  // Stops producing video.
  virtual void Stop() = 0;

  virtual void SetScreencast(bool is_screencast) = 0;

 protected:
  virtual rtc::VideoSourceInterface<VideoFrame>* source() = 0;

 private:
  RTC_NO_UNIQUE_ADDRESS SequenceChecker worker_thread_checker_;
  RTC_NO_UNIQUE_ADDRESS SequenceChecker signaling_thread_checker_;
  SourceState state_ RTC_GUARDED_BY(&signaling_thread_checker_);
  const bool remote_;
};

}  // namespace test
}  // namespace webrtc

#endif  // API_TEST_VIDEO_TEST_VIDEO_TRACK_SOURCE_H_
