#pragma once

//! Fixed point number (With implicit conversion to float)
template<typename T, int CompressValue>
class FixedFloat {
    T value{};

public:
    FixedFloat() = default;
    FixedFloat(float v) : value(static_cast<T>(v * CompressValue)) {}
    template<typename Y>
    FixedFloat(Y x) : value(x) {}

    operator float() const { return static_cast<float>(value) / CompressValue; }
};
