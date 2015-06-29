#pragma once

#include <functional>

// via http://stackoverflow.com/a/1295338
#define CONCAT_IMPL( x, y ) x##y
#define MACRO_CONCAT( x, y ) CONCAT_IMPL( x, y )
#define UNIQUE_NAME MACRO_CONCAT( uniqueStateGuard_, __COUNTER__ )

#define STATE_CHECK( A ) \
    ScopeGuard UNIQUE_NAME( [this] { \
        if( A ) { \
            LOG_ERROR( "Bad State!" ); \
        } \
        assert( !A ); \
    },[this] { \
        A = true; \
    } );


class ScopeGuard {
    public:
        ScopeGuard( const std::function<void()>& onInit, const std::function<void()>& onExit ) : m_OnInit( onInit ), m_OnExit( onExit ) {
            m_OnInit();
        }
        ~ScopeGuard() {
            m_OnExit();
        }
    private:
        const std::function<void()> m_OnInit;
        const std::function<void()> m_OnExit;
};
