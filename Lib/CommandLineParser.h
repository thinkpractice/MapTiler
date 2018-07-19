#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <QCommandLineParser>
#include <string>
#include "Settings.h"

class CommandLineParser
{
public:
    enum CommandLineParseResult
    {
        CommandLineOk,
        CommandLineDisplayUrl,
        CommandLineError,
        CommandLineVersionRequested,
        CommandLineHelpRequested
    };

public:
    CommandLineParser();

    Settings GetSettings();

    CommandLineParseResult Parse();
    std::string ErrorMessage() const;

    std::string GetStringValue(std::string settingsName, std::string defaultValue = "");
    int GetIntValue(std::string settingsName, int defaultValue = 0);

private:
    void DisplayUrl(std::string displayUrl);

private:
    QCommandLineParser _parser;
    std::string _errorMessage;
    Settings _settings;
};

#endif // COMMANDLINEPARSER_H
