#ifndef SIGNALS_H
#define SIGNALS_H

#include <signal.h>
#include <csignal>

namespace Signals {
    volatile inline std::sig_atomic_t signalStatus = 0;
        
    // Register a handler by sigaction. Default flag is SA_RESTART.
    inline bool registerHandler(int sig, void (*handler)(int), int flags = SA_RESTART) noexcept {
        struct sigaction sa {};
        sa.sa_handler = handler;
        sa.sa_flags = flags;
        sigemptyset(&sa.sa_mask);
        return sigaction(sig, &sa, nullptr) == 0;
    }
    
    inline void signalHandler(int signal) {
        signalStatus = signal;
    }

}

#endif // SIGNALS_H
