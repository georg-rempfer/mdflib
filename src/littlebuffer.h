/*
 * Copyright 2022 Ingemar Hedvall
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>
namespace mdf {

template <typename T>
class LittleBuffer {
 public:
  LittleBuffer() = default;
  explicit LittleBuffer(const T& value);
  LittleBuffer(const std::vector<uint8_t>& buffer, size_t offset);

  [[nodiscard]] const uint8_t* data() const;
  [[nodiscard]] uint8_t* data();
  [[nodiscard]] size_t size() const;
  T value() const;

 private:
  std::array<uint8_t, sizeof(T)> buffer_;
};

template <typename T>
LittleBuffer<T>::LittleBuffer(const T& value) {
  constexpr int num = 1;
  if (*(char*) &num == 0) { // Big endian
    std::array<uint8_t, sizeof(T)> temp = {0};
    memcpy(temp.data(), &value, sizeof(T));
    for (size_t index = sizeof(T); index > 0; --index) {
      buffer_[sizeof(T) - index] = temp[index - 1];
    }
  } else {
    memcpy(buffer_.data(), &value, sizeof(T));
  }
}

template <typename T>
LittleBuffer<T>::LittleBuffer(const std::vector<uint8_t>& buffer,
                              size_t offset) {
  constexpr int num = 1;
  if (*(char*) &num == 0) {
    std::array<uint8_t, sizeof(T)> temp = {0};
    memcpy(temp.data(), buffer.data() + offset, sizeof(T));
    for (size_t index = sizeof(T); index > 0; --index) {
      buffer_[sizeof(T) - index] = temp[index - 1];
    }
  } else {
    memcpy(buffer_.data(), buffer.data() + offset, sizeof(T));
  }
}

template <typename T>
const uint8_t* LittleBuffer<T>::data() const {
  return buffer_.data();
}

template <typename T>
uint8_t* LittleBuffer<T>::data() {
  return buffer_.data();
}
template <typename T>
size_t LittleBuffer<T>::size() const {
  return buffer_.size();
}

template <typename T>
T LittleBuffer<T>::value() const {
  constexpr int num = 1;
  std::array<uint8_t, sizeof(T)> temp = {0};
  if (*(char*) &num == 0) {
    for (size_t index = sizeof(T); index > 0; --index) {
      temp[sizeof(T) - index] = buffer_[index - 1];
    }
  } else {
    memcpy(temp.data(), buffer_.data(), sizeof(T));
  }
  const T* val = reinterpret_cast<const T*>(temp.data());
  return *val;
}

}  // namespace mdf
