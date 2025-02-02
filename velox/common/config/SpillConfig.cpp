/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "velox/common/config/SpillConfig.h"
#include "velox/common/base/Exceptions.h"

namespace facebook::velox::common {
int32_t SpillConfig::joinSpillLevel(uint8_t startBitOffset) const {
  const auto numPartitionBits = joinPartitionBits;
  VELOX_CHECK_LE(
      startBitOffset + numPartitionBits,
      64,
      "startBitOffset:{} numPartitionsBits:{}",
      startBitOffset,
      numPartitionBits);
  const int32_t deltaBits = startBitOffset - startPartitionBit;
  VELOX_CHECK_GE(deltaBits, 0, "deltaBits:{}", deltaBits);
  VELOX_CHECK_EQ(
      deltaBits % numPartitionBits,
      0,
      "deltaBits:{} numPartitionsBits{}",
      deltaBits,
      numPartitionBits);
  return deltaBits / numPartitionBits;
}

bool SpillConfig::exceedJoinSpillLevelLimit(uint8_t startBitOffset) const {
  if (startBitOffset + joinPartitionBits > 64) {
    return true;
  }
  if (maxSpillLevel == -1) {
    return false;
  }
  return joinSpillLevel(startBitOffset) > maxSpillLevel;
}
} // namespace facebook::velox::common
