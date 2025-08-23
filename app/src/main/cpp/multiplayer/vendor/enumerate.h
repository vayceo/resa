#pragma once

#include <algorithm>
#include <iostream>

namespace notsa {
    namespace detail {
        template <typename ItType>
        class enumerate_iterator {
        public:
            using value_type = typename std::iterator_traits<ItType>::value_type;
            using reference = typename std::iterator_traits<ItType>::reference;
            using pointer = typename std::iterator_traits<ItType>::pointer;
            using difference_type = typename std::iterator_traits<ItType>::difference_type;
            using iterator_category = std::input_iterator_tag;

            using pair_type = std::pair<difference_type, reference>;

        private:
            ItType m_iter{};
            difference_type m_counter{};

        public:
            enumerate_iterator() = default;

            enumerate_iterator(ItType it, difference_type start) : m_iter(std::move(it)), m_counter(start) {}

            auto operator++() -> enumerate_iterator& { // pre-increment
                ++m_iter;
                ++m_counter;
                return *this;
            }

            auto operator++(int) -> enumerate_iterator& { // post-increment
                auto ret = *this;
                ++(*this);
                return ret;
            }

            auto operator--() -> enumerate_iterator& { // pre-decrement
                --m_iter;
                --m_counter;
                return *this;
            }

            auto operator--(int) -> enumerate_iterator& { // post-decrement
                auto ret = *this;
                --(*this);
                return ret;
            }

            template <typename OtherItType>
            auto operator==(const enumerate_iterator<OtherItType>& other) const {
                return m_iter == other.m_iter;
            }

            template <typename OtherItType>
            auto operator!=(const enumerate_iterator<OtherItType>& other) const {
                return !(*this == other);
            }

            pair_type operator*() {
                return {m_counter, *m_iter};
            }

            pair_type operator*() const {
                return {m_counter, *m_iter};
            }
        };

        template <typename S, typename E>
        class enumerate_object {
        private:
            using difference_type = typename std::iterator_traits<S>::difference_type;

            difference_type m_start{};
            S m_begin{};
            E m_end{};

        public:
            using iterator = enumerate_iterator<S>;
            using reverse_iterator = std::reverse_iterator<iterator>;
            using const_iterator = iterator;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;

            using value_type = typename iterator::value_type;

            enumerate_object(S begin, E end, difference_type start = 0) : m_start(start), m_begin(begin), m_end(end) {}

            auto begin() -> iterator {
                return {m_begin, m_start};
            }

            auto end() -> iterator {
                return {m_end, {}};
            }

            auto begin() const -> const_iterator {
                return {m_begin, m_start};
            }

            auto end() const -> const_iterator {
                return {m_end, {}};
            }

            auto rbegin() -> reverse_iterator {
                return {begin()};
            }

            auto rend() -> reverse_iterator {
                return {end()};
            }

            auto rbegin() const -> const_reverse_iterator {
                return {begin()};
            }

            auto rend() const -> const_reverse_iterator {
                return {end()};
            }
        };
    } // namespace detail

    template <typename S, typename E>
    auto enumerate(S begin, E end, typename std::iterator_traits<S>::difference_type start = 0) {
        return detail::enumerate_object<S, E>{begin, end, start};
    }

    template <typename R>
    auto enumerate(R&& r, typename std::iterator_traits<decltype(std::begin(r))>::difference_type start = 0) {
        using std::begin;
        using std::end;
        return detail::enumerate_object<decltype(begin(r)), decltype(end(r))>{begin(r), end(r), start};
    }
} // namespace notsa

