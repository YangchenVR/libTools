#ifndef _vrHead_h_
#define _vrHead_h_

#include "vrMacro.h"
#include "vrTypes.h"
#if vrLoggingPP
#define ELPP_NO_DEFAULT_LOG_FILE
#define ELPP_THREAD_SAFE
#define ELPP_FORCE_USE_STD_THREAD
#include "easyloggingpp/easylogging++.h"
INITIALIZE_EASYLOGGINGPP
#endif//vrLoggingPP
#endif//_vrHead_h_