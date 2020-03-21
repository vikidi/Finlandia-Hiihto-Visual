#include "ilmatiedeapi.h"
#include "logger.h"
#include <QString>

InternetExplorers::IlmatiedeAPI::IlmatiedeAPI()
{
    auto msg(QString("Constructor ready"));
    auto msgSender(QString("IlmatiedeAPI"));
    InternetExplorers::Logger::getInstance().log(msg, Constants::Logger::Severity::INFO, msgSender);
}
