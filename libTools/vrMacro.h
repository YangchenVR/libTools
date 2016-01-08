#ifndef _vrMacro_h_
#define _vrMacro_h_

#ifdef _WIN32
#define DoublePrecision (1)
#define Invalid_Id (-1)
#define vrZero (0)
#define vrNull (0)
#define X_AXIS (0)
#define Y_AXIS (1)
#define Z_AXIS (2)
#define vrError(X)do{printf("%s\n",#X);exit(66);}while(false)
#define vrPause system("pause")
#define vrExit exit(66)
#define vrASSERT(X) do{if (!(X)){/*printf("%s\n",#X);*/system("pause");}}while(false)
#define vrDim (3)
#define vrBufferSize (256)
#define MyNotice
#define LogInfo printf
#define VNAME(name) (#name)
#define INFOVAR(name) std::cout << VNAME(name) << " = " << name << std::endl;
#define INFOMAT(name) std::cout << VNAME(name) << " = [" << (name).rows() << "," << (name).cols() << "]" << std::endl;
#define INFOSTDCOLLECTION(name) std::cout << VNAME(name) << " = " << name.size() << std::endl;
#define vrMatRowMajor (1)
#define vrMatColMajor (0)
#define vrMatStoreType vrMatRowMajor
#define vrLoggingPP (1)
#if vrLoggingPP
#define infoLog LOG(INFO)
#define debugLog LOG(DEBUG)
#define warningLog LOG(WARNING)
//#define globalLog LOG(GLOBAL)
#define traceLog LOG(TRACE)
#define fatalLog LOG(FATAL)
#define errorLog LOG(ERROR)
#define verboseLog VLOG(1)
#endif
#else
	#error("Unsupport OS Type!") 
#endif

#endif//_vrMacro_h_