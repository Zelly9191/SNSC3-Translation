#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <regex>
#include <list>


const std::string rawDumpDir = "./raw_dump/";
const std::string untranslatedDir = "./script_untranslated/";
const std::string JEScriptDir = "./JE_Scripts/";
const std::string TestJEScriptDir = "./Test_JEScripts/";

const std::string ENRegex = "^[a-zA-Z0-9!@#$&;:_|()\\-`.+,<>=/\"\\s]+$";
