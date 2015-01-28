#include "Log.h"
#include <iostream>
#include <fstream>
using namespace std;
TLogLevel Log::reportingLevel = logDEBUG4;

Log::Log() {
}

std::ostringstream& Log::Get(TLogLevel level) {
    if (level < logINFO) {
        os_err << "- " << NowTime();
        os_err << " " << ToString(level) << ": ";
        os_err << std::string(level > logDEBUG ? level - logDEBUG : 0, '\t');
        return os_err;
    } else {
        os_out << "- " << NowTime();
        os_out << " " << ToString(level) << ": ";
        os_out << std::string(level > logDEBUG ? level - logDEBUG : 0, '\t');
        return os_out;
    }
}

Log::~Log() {
    if (os_out.tellp() != 0) {
        os_out << std::endl;
        std::string logOutFile("osm2x3d.log");
        ofstream myOutFile;
        myOutFile.open(logOutFile, ios::app);
        myOutFile << os_out.str();
        myOutFile.close();
        //        fprintf(stdout, "%s", os_out.str().c_str());
        //        fflush(stdout);
    }
    if (os_err.tellp() != 0) {
        os_err << std::endl;
        string logErrFile("osm2x3d_error.log");
        ofstream myErrFile;
        myErrFile.open(logErrFile, ios::app);
        myErrFile << os_err.str();
        myErrFile.close();
        //        fprintf(stderr, "%s", os_err.str().c_str());
        //        fflush(stderr);
    }
}

TLogLevel& Log::ReportingLevel() {
    //    static TLogLevel reportingLevel = logDEBUG4;
    return Log::reportingLevel;
}

void Log::setLevel(TLogLevel logLevel) {
    Log::reportingLevel = logLevel;
}

void Log::init() {
        ofstream myfile;

        std::string logOutFile("osm2x3d.log");
        myfile.open(logOutFile);
        myfile << "";
        myfile.close();
        
        string logErrFile("osm2x3d_error.log");
        myfile.open(logErrFile);
        myfile << "";
        myfile.close();
}

std::string Log::ToString(TLogLevel level) {
    static const char* const buffer[] = {"ERROR", "WARNING", "INFO", "DEBUG", "DEBUG1", "DEBUG2", "DEBUG3", "DEBUG4"};
    return buffer[level];
}

TLogLevel Log::FromString(const std::string& level) {
    if (level == "DEBUG4")
        return logDEBUG4;
    if (level == "DEBUG3")
        return logDEBUG3;
    if (level == "DEBUG2")
        return logDEBUG2;
    if (level == "DEBUG1")
        return logDEBUG1;
    if (level == "DEBUG")
        return logDEBUG;
    if (level == "INFO")
        return logINFO;
    if (level == "WARNING")
        return logWARNING;
    if (level == "ERROR")
        return logERROR;
    Log().Get(logWARNING) << "Unknown logging level '" << level << "'. Using INFO level as default.";
    return logINFO;
}




