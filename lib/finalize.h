#pragma once

#include <utility>

template <typename T>
class Finalizator {
public:
    Finalizator(T&& block)
        : block(std::move(block))
    {
    }

    ~Finalizator() { block(); }

private:
    T block;
};

class FinalizeMarker { };

template <typename T>
Finalizator<T> operator<<(const FinalizeMarker&, T&& block)
{
    return Finalizator<T>(std::move(block));
}

#define CONCAT_IMPL__(s1, s2) s1##s2
#define CONCAT__(s1, s2) CONCAT_IMPL__(s1, s2)

#define finalize auto CONCAT__(finalize_block_, __LINE__) = FinalizeMarker() << [&]()
