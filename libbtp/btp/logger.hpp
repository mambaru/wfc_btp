#pragma once 

#include <wlog/logging.hpp>

#define BTPLOG_ERROR(X)    WLOG_LOG_ERROR   ( "BTP", X )
#define BTPLOG_WARNING(X)  WLOG_LOG_WARNING ( "BTP", X )
#define BTPLOG_MESSAGE(X)  WLOG_LOG_MESSAGE ( "BTP", X )
#define BTPLOG_FATAL(X)    WLOG_LOG_FATAL   ( "BTP", X )
#define BTPLOG_BEGIN(X)    WLOG_LOG_BEGIN   ( "BTP", X )
#define BTPLOG_END(X)      WLOG_LOG_END     ( "BTP", X )
#define BTPLOG_DEBUG(X)    WLOG_LOG_DEBUG   ( "BTP", X )
#define BTPLOG_TRACE(X)    WLOG_LOG_TRACE   ( "BTP", X )
#define BTPLOG_PROGRESS(X) WLOG_LOG_PROGRESS( "BTP", X )


