#pragma region Copyright (c) 2014-2016 OpenRCT2 Developers
/*****************************************************************************
 * OpenRCT2, an open source clone of Roller Coaster Tycoon 2.
 *
 * OpenRCT2 is the work of many authors, a full list can be found in contributors.md
 * For more information, visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * A full copy of the GNU General Public License can be found in licence.txt
 *****************************************************************************/
#pragma endregion

extern "C"
{
    #include "../platform/platform.h"
}

#include "../core/Console.hpp"
#include "../core/Math.hpp"
#include "../core/String.hpp"
#include "CommandLine.hpp"

#pragma region CommandLineArgEnumerator

CommandLineArgEnumerator::CommandLineArgEnumerator(const char * const * arguments, int count)
{
    _arguments = arguments;
    _count = count;
    _index = 0;
}

void CommandLineArgEnumerator::Reset()
{
    _index = 0;
}

bool CommandLineArgEnumerator::Backtrack()
{
    if (_index > 0)
    {
        _index--;
        return true;
    }
    else
    {
        return false;
    }
}

bool CommandLineArgEnumerator::TryPop()
{
    if (_index < _count)
    {
        _index++;
        return true;
    }
    else
    {
        return false;
    }
}

bool CommandLineArgEnumerator::TryPopInteger(sint32 * result)
{
    char const * arg;
    if (TryPopString(&arg))
    {
        *result = (sint32)atol(arg);
        return true;
    }
    
    return false;
}

bool CommandLineArgEnumerator::TryPopReal(float * result)
{
    char const * arg;
    if (TryPopString(&arg))
    {
        *result = (float)atof(arg);
        return true;
    }
    
    return false;
}

bool CommandLineArgEnumerator::TryPopString(const char * * result)
{
    if (_index < _count)
    {
        *result = _arguments[_index];
        _index++;
        return true;
    }
    else
    {
        return false;
    }
}

#pragma endregion

namespace CommandLine
{
    constexpr const char * HelpText = "openrct2 -ha shows help for all commands. "
                                      "openrct2 <command> -h will show help and details for a given command.";

    static void   PrintHelpFor(const CommandLineCommand * commands);
    static void   PrintOptions(const CommandLineOptionDefinition *options);
    static void   PrintExamples(const CommandLineExample *examples);
    static utf8 * GetOptionCaption(utf8 * buffer, size_t bufferSize, const CommandLineOptionDefinition *option);

    static const CommandLineOptionDefinition * FindOption(const CommandLineOptionDefinition * options, char shortName);
    static const CommandLineOptionDefinition * FindOption(const CommandLineOptionDefinition * options, const char * longName);

    static bool ParseShortOption(const CommandLineOptionDefinition * options, CommandLineArgEnumerator *argEnumerator, const char *argument);
    static bool ParseLongOption(const CommandLineOptionDefinition * options, CommandLineArgEnumerator * argEnumerator, const char * argument);
    static bool ParseOptionValue(const CommandLineOptionDefinition * option, const char * valueString);

    static bool HandleSpecialArgument(const char * argument);

    void PrintHelp(bool allCommands)
    {
        PrintHelpFor(RootCommands);
        PrintExamples(RootExamples);

        if (allCommands)
        {
            for (const CommandLineCommand *command = RootCommands; command->Name != nullptr; command++)
            {
                if (command->SubCommands != nullptr)
                {
                    size_t commandNameLength = String::LengthOf(command->Name);
                    for (size_t i = 0; i < commandNameLength; i++)
                    {
                        Console::Write("-");
                    }
                    Console::WriteLine();
                    Console::WriteLine(command->Name);
                    for (size_t i = 0; i < commandNameLength; i++)
                    {
                        Console::Write("-");
                    }
                    Console::WriteLine();
                    PrintHelpFor(command->SubCommands);
                }
            }
        }
        else
        {
            Console::WriteLine(HelpText);
        }
    }

    static void PrintHelpFor(const CommandLineCommand * commands)
    {
        // Print usage
        const char * usageString = "usage: openrct2 ";
        const size_t usageStringLength = String::LengthOf(usageString);
        Console::Write(usageString);

        // Get the largest command name length and parameter length
        size_t maxNameLength = 0;
        size_t maxParamsLength = 0;
        const CommandLineCommand * command;
        for (command = commands; command->Name != nullptr; command++)
        {
            maxNameLength = Math::Max(maxNameLength, String::LengthOf(command->Name));
            maxParamsLength = Math::Max(maxParamsLength, String::LengthOf(command->Parameters));
        }

        for (command = commands; command->Name != nullptr; command++)
        {
            if (command != commands)
            {
                Console::WriteSpace(usageStringLength);
            }

            Console::Write(command->Name);
            Console::WriteSpace(maxNameLength - String::LengthOf(command->Name) + 1);

            if (command->SubCommands == nullptr)
            {
                Console::Write(command->Parameters);
                Console::WriteSpace(maxParamsLength - String::LengthOf(command->Parameters));

                if (command->Options != nullptr)
                {
                    Console::Write(" [options]");
                }
            }
            else
            {
                Console::Write("...");
            }
            Console::WriteLine();
        }
        Console::WriteLine();

        if (commands->Options != nullptr)
        {
            PrintOptions(commands->Options);
        }
    }

    static void PrintOptions(const CommandLineOptionDefinition *options)
    {
        // Print options for main commands
        size_t maxOptionLength = 0;
        const CommandLineOptionDefinition * option = options;
        for (; option->Type != 255; option++)
        {
            char buffer[128];
            GetOptionCaption(buffer, sizeof(buffer), option);
            size_t optionCaptionLength = String::LengthOf(buffer);
            maxOptionLength = Math::Max(maxOptionLength, optionCaptionLength);
        }

        option = options;
        for (; option->Type != 255; option++)
        {
            Console::WriteSpace(4);

            char buffer[128];
            GetOptionCaption(buffer, sizeof(buffer), option);
            size_t optionCaptionLength = String::LengthOf(buffer);
            Console::Write(buffer);

            Console::WriteSpace(maxOptionLength - optionCaptionLength + 4);
            Console::Write(option->Description);
            Console::WriteLine();
        }
        Console::WriteLine();
    }

    static void PrintExamples(const CommandLineExample *examples)
    {
        size_t maxArgumentsLength = 0;

        const CommandLineExample * example;
        for (example = examples; example->Arguments != nullptr; example++)
        {
            size_t argumentsLength = String::LengthOf(example->Arguments);
            maxArgumentsLength = Math::Max(maxArgumentsLength, argumentsLength);
        }

        Console::WriteLine("examples:");
        for (example = examples; example->Arguments != nullptr; example++)
        {
            Console::Write("  openrct2 ");
            Console::Write(example->Arguments);

            size_t argumentsLength = String::LengthOf(example->Arguments);
            Console::WriteSpace(maxArgumentsLength - argumentsLength + 4);
            Console::Write(example->Description);
            Console::WriteLine();
        }

        Console::WriteLine();
    }

    static utf8 * GetOptionCaption(utf8 * buffer, size_t bufferSize, const CommandLineOptionDefinition *option)
    {
        buffer[0] = 0;

        if (option->ShortName != '\0')
        {
            String::AppendFormat(buffer, bufferSize, "-%c, ", option->ShortName);
        }

        String::Append(buffer, bufferSize, "--");
        String::Append(buffer, bufferSize, option->LongName);

        switch (option->Type) {
        case CMDLINE_TYPE_INTEGER:
            String::Append(buffer, bufferSize, "=<int>");
            break;
        case CMDLINE_TYPE_REAL:
            String::Append(buffer, bufferSize, "=<real>");
            break;
        case CMDLINE_TYPE_STRING:
            String::Append(buffer, bufferSize, "=<str>");
            break;
        }

        return buffer;
    }

    const CommandLineCommand * FindCommandFor(const CommandLineCommand * commands, CommandLineArgEnumerator *argEnumerator)
    {
        // Check if end of arguments or options have started
        const char * firstArgument;
        if (!argEnumerator->TryPopString(&firstArgument))
        {
            return commands;
        }
        if (firstArgument[0] == '-')
        {
            argEnumerator->Backtrack();
            return commands;
        }

        // Search through defined commands for one that matches
        const CommandLineCommand * fallback = nullptr;
        const CommandLineCommand * command = commands;
        for (; command->Name != nullptr; command++)
        {
            if (command->Name[0] == '\0')
            {
                // If we don't find a command, this should be used
                fallback = command;
            }
            else if (String::Equals(command->Name, firstArgument))
            {
                if (command->SubCommands == nullptr)
                {
                    // Found matching command
                    return command;
                }
                else
                {
                    // Recurse for the sub command table
                    return FindCommandFor(command->SubCommands, argEnumerator);
                }
            }
        }

        argEnumerator->Backtrack();
        return fallback;
    }

    static bool ParseOptions(const CommandLineOptionDefinition * options, CommandLineArgEnumerator *argEnumerator)
    {
        bool firstOption = true;

        const char * argument;
        while (argEnumerator->TryPopString(&argument))
        {
            if (HandleSpecialArgument(argument))
            {
                continue;
            }

            if (argument[0] == '-')
            {
                if (argument[1] == '-')
                {
                    if (!ParseLongOption(options, argEnumerator, &argument[2]))
                    {
                        return false;
                    }
                }
                else
                {
                    if (!ParseShortOption(options, argEnumerator, argument))
                    {
                        return false;
                    }
                }
                firstOption = false;
            }
            else if (!firstOption)
            {
                Console::Error::WriteLine("All options must be passed at the end of the command line.");
                return false;
            }
            else
            {
                continue;
            }
        }

        return true;
    }

    static bool ParseLongOption(const CommandLineOptionDefinition * options,
                                CommandLineArgEnumerator *argEnumerator,
                                const char *argument)
    {
        // Get just the option name
        char optionName[64];
        const char * equalsCh = strchr(argument, '=');
        if (equalsCh != nullptr)
        {
            String::Set(optionName, sizeof(optionName), argument, equalsCh - argument);
        }
        else
        {
            String::Set(optionName, sizeof(optionName), argument);
        }

        // Find a matching option definition
        const CommandLineOptionDefinition * option = FindOption(options, optionName);
        if (option == nullptr)
        {
            Console::Error::WriteLine("Unknown option: --%s", optionName);
            return false;
        }

        if (equalsCh == nullptr)
        {
            if (option->Type == CMDLINE_TYPE_SWITCH)
            {
                ParseOptionValue(option, nullptr);
            }
            else
            {
                const char * valueString = nullptr;
                if (!argEnumerator->TryPopString(&valueString))
                {
                    Console::Error::WriteLine("Expected value for option: %s", optionName);
                    return false;
                }
                
                if (!ParseOptionValue(option, valueString))
                {
                    return false;
                }
            }
        }
        else
        {
            if (option->Type == CMDLINE_TYPE_SWITCH)
            {
                Console::Error::WriteLine("Option is a switch: %s", optionName);
                return false;
            }
            else
            {
                if (!ParseOptionValue(option, equalsCh + 1))
                {
                    return false;
                }
            }
        }

        return true;
    }

    static bool ParseShortOption(const CommandLineOptionDefinition * options,
                                 CommandLineArgEnumerator *argEnumerator,
                                 const char *argument)
    {
        const CommandLineOptionDefinition * option = nullptr;

        const char * shortOption = &argument[1];
        for (; *shortOption != '\0'; shortOption++)
        {
            option = FindOption(options, shortOption[0]);
            if (option == nullptr)
            {
                Console::Error::WriteLine("Unknown option: -%c", shortOption[0]);
                return false;
            }
            if (option->Type == CMDLINE_TYPE_SWITCH)
            {
                if (!ParseOptionValue(option, nullptr))
                {
                    return false;
                }
            }
            else if (shortOption[1] != '\0')
            {
                if (!ParseOptionValue(option, &shortOption[1]))
                {
                    return false;
                }
                return true;
            }
        }

        if (option != nullptr && option->Type != CMDLINE_TYPE_SWITCH)
        {
            const char * valueString = nullptr;
            if (!argEnumerator->TryPopString(&valueString))
            {
                Console::Error::WriteLine("Expected value for option: %c", option->ShortName);
                return false;
            }
                
            if (!ParseOptionValue(option, valueString))
            {
                return false;
            }
        }

        return true;
    }

    static bool ParseOptionValue(const CommandLineOptionDefinition * option, const char * valueString)
    {
        if (option->OutAddress == nullptr) return true;

        switch (option->Type) {
        case CMDLINE_TYPE_SWITCH:
            *((bool *)option->OutAddress) = true;
            return true;
        case CMDLINE_TYPE_INTEGER:
            *((sint32 *)option->OutAddress) = (sint32)atol(valueString);
            return true;
        case CMDLINE_TYPE_REAL:
            *((float *)option->OutAddress) = (float)atof(valueString);
            return true;
        case CMDLINE_TYPE_STRING:
            *((utf8 * *)option->OutAddress) = String::Duplicate(valueString);
            return true;
        default:
            Console::Error::WriteLine("Unknown CMDLINE_TYPE for: %s", option->LongName);
            return false;
        }
    }

    static bool HandleSpecialArgument(const char * argument)
    {
#ifdef __MACOSX__
        if (String::Equals(argument, "-NSDocumentRevisionsDebugMode"))
        {
            return true;
        }
        if (String::StartsWith(argument, "-psn_"))
        {
            return true;
        }
#endif
        return false;
    }


    const CommandLineOptionDefinition * FindOption(const CommandLineOptionDefinition * options, char shortName)
    {
        for (const CommandLineOptionDefinition * option = options; option->Type != 255; option++)
        {
            if (option->ShortName == shortName)
            {
                return option;
            }
        }
        return nullptr;
    }

    const CommandLineOptionDefinition * FindOption(const CommandLineOptionDefinition * options, const char * longName)
    {
        for (const CommandLineOptionDefinition * option = options; option->Type != 255; option++)
        {
            if (String::Equals(option->LongName, longName))
            {
                return option;
            }
        }
        return nullptr;
    }
}

extern "C"
{
    int cmdline_run(const char * * argv, int argc)
    {
        auto argEnumerator = CommandLineArgEnumerator(argv, argc);

        // Pop process path
        argEnumerator.TryPop();

        const CommandLineCommand * command = CommandLine::FindCommandFor(CommandLine::RootCommands, &argEnumerator);
        if (command->Options != nullptr)
        {
            auto argEnumeratorForOptions = CommandLineArgEnumerator(argEnumerator);
            if (!CommandLine::ParseOptions(command->Options, &argEnumeratorForOptions))
            {
                return EXITCODE_FAIL;
            }
        }
        if (command == CommandLine::RootCommands && command->Func == nullptr)
        {
            return CommandLine::HandleCommandDefault();
        }
        else
        {
            return command->Func(&argEnumerator);
        }
    }
}
