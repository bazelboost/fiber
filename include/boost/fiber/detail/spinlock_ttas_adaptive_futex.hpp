
//          Copyright Oliver Kowalke 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_SPINLOCK_TTAS_ADAPTIVE_FUTEX_H
#define BOOST_FIBERS_SPINLOCK_TTAS_ADAPTIVE_FUTEX_H

#include <atomic>
#include <thread>

#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/detail/futex.hpp>

namespace boost {
namespace fibers {
namespace detail {

class spinlock_ttas_adaptive_futex {
private:
    template< typename FBSplk >
    friend class spinlock_rtm;

    std::atomic< std::int32_t >                 value_{ 0 };
    std::atomic< std::int32_t >                 retries_{ 0 };

public:
    spinlock_ttas_adaptive_futex() = default;

    spinlock_ttas_adaptive_futex( spinlock_ttas_adaptive_futex const&) = delete;
    spinlock_ttas_adaptive_futex & operator=( spinlock_ttas_adaptive_futex const&) = delete;

    void lock() noexcept;

    bool try_lock() noexcept {
        std::int32_t expected = 0;
        return value_.compare_exchange_strong( expected, 1, std::memory_order_acquire);
    }

    void unlock() noexcept {
        if ( 1 != value_.fetch_sub( 1, std::memory_order_acquire) ) {
            value_.store( 0, std::memory_order_release);
            futex_wake( & value_);
        }
    }
};

}}}

#endif // BOOST_FIBERS_SPINLOCK_TTAS_ADAPTIVE_FUTEX_H
