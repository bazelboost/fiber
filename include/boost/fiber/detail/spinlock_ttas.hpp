
//          Copyright Oliver Kowalke 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_SPINLOCK_TTAS_H
#define BOOST_FIBERS_SPINLOCK_TTAS_H

#include <atomic>
#include <thread>

#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/detail/spinlock_status.hpp>

namespace boost {
namespace fibers {
namespace detail {

class spinlock_ttas {
private:
    template< typename FBSplk >
    friend class spinlock_rtm;

    std::atomic< spinlock_status >              state_{ spinlock_status::unlocked };

public:
    spinlock_ttas() = default;

    spinlock_ttas( spinlock_ttas const&) = delete;
    spinlock_ttas & operator=( spinlock_ttas const&) = delete;

    void lock() noexcept;

    bool try_lock() noexcept {
        return spinlock_status::unlocked == state_.exchange( spinlock_status::locked, std::memory_order_acquire);
    }

    void unlock() noexcept {
        state_.store( spinlock_status::unlocked, std::memory_order_release);
    }
};

}}}

#endif // BOOST_FIBERS_SPINLOCK_TTAS_H
