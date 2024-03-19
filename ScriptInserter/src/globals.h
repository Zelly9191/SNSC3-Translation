#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <regex>
#include <list>

const std::string rawDumpDir = "./raw dump/";
const std::string untranslatedDir = "./script untranslated/";
const std::string JEScriptDir = "./JEScripts/";
const std::string TestJEScriptDir = "./Test_JEScripts/";

const std::string ENRegex = "^[a-zA-Z0-9!@#$&;:_|()\\-`.+,<>=/\"\\s]+$";
