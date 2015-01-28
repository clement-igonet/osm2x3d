#ifndef LIBDEFBOXTOOLS_LOG_H__
#define LIBDEFBOXTOOLS_LOG_H__

#include <sstream>
#include <string>
#include <stdio.h>
#include <sys/time.h>


inline std::string NowTime();

enum TLogLevel {
    logERROR, logWARNING, logINFO, logDEBUG, logDEBUG1, logDEBUG2, logDEBUG3, logDEBUG4
};

/**
 * Usage:
 * Log::setLevel(logINFO);
 * FILE_LOG(logDEBUG) << "Message to log";
 */

class Log {
public:
    Log();
    virtual ~Log();


    std::ostringstream& Get(TLogLevel level = logINFO);
public:
    // TODO: add a way to define log level
    static void init();
    static void setLevel(TLogLevel logLevel);
    static TLogLevel& ReportingLevel();
    static std::string ToString(TLogLevel level);
    static TLogLevel FromString(const std::string& level);
    static void setOutFile(std::string& outFile);
    static void setErrFile(std::string& errFile);

protected:
    std::ostringstream os_out;
    std::ostringstream os_err;
private:
    Log(const Log&);
    Log& operator =(const Log&);
    static TLogLevel reportingLevel;
//    static std::string outFile_;
//    static std::string errFile_;
};



typedef Log FILELog;

#define FILE_LOG(level) \
    if (level > FILELog::ReportingLevel()) ; \
    else Log().Get(level)

std::string NowTime() {
    char timeBuffer[11];
    char dateBuffer[14];
    time_t t;
    time(&t);
//    tm r = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    tm r;
    //    %X	Time representation *	14:55:02
    //    %c	Date and time representation *	Thu Aug 23 14:55:02 2001
    strftime(timeBuffer, sizeof (timeBuffer), "%X", localtime_r(&t, &r));
    //    %F	Short YYYY-MM-DD date, equivalent to %Y-%m-%d	2001-08-23
    strftime(dateBuffer, sizeof (dateBuffer), "%F", localtime_r(&t, &r));
    struct timeval tv;
    gettimeofday(&tv, 0);
    char result[100] = {0};
    sprintf(result, "%s_%s.%03ld", dateBuffer, timeBuffer, (long) tv.tv_usec / 1000);
    return result;
}


#endif // LIBDEFBOXTOOLS_LOG_H__
