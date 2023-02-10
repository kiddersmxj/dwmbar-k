#include <iostream>
#include <fstream>
#include <array>
#include <filesystem>
#include "../std-k.hpp"
namespace fs = std::filesystem;

// NULL function to attach breakpoint to in gdb
void BreakPoint(void) {
    std::cout << "";
}

std::string ExecCmd(const std::string cmd, const int Output, int ExitStatus) {
	auto pPipe = ::popen(cmd.c_str(), "r");
	if(pPipe == nullptr)
	{
	    throw std::runtime_error("Cannot open pipe");	
	}

    std::array<char, 256> buffer;

    std::string result;

	while(not feof(pPipe)) {
	    auto bytes = fread(buffer.data(), 1, buffer.size(), pPipe);
	    result.append(buffer.data(), bytes);
	}

	auto rc = ::pclose(pPipe);

    if(Output == 1)
        std::cout << result << std::endl;

    if(WIFEXITED(rc)) {
	    ExitStatus = WEXITSTATUS(rc);
	}

	return result;
}

void VPrint(std::vector<std::string> Input) {
	for(std::string Output: Input)
	    std::cout << Output << std::endl;
}

void WriteFileLines(std::vector<std::string> Lines, std::string File) {
    std::fstream FileToWrite(File, std::ios::out);
	if (!FileToWrite) {
        std::cout << "File not created!" << std::endl;
	}
	else {
        for(std::string Line: Lines)
            FileToWrite << Line << std::endl;
		FileToWrite.close();
	}
}

std::vector<std::string> ReadFileLines(std::string File) {
    std::string Line;
    std::vector<std::string> Output;
    std::ifstream FileToRead(File);

    while(getline(FileToRead, Line)) {
        Output.push_back(Line);
    }

    FileToRead.close();
    return Output;
}

// Pass delim most likley usng string literal R"(...)"
std::vector<std::string> SplitStr(std::string String, const char* Delim) {
    char* Array = new char[String.length() + 1];
    strcpy(Array, String.c_str());
    char* substr = strtok(Array, Delim);
    
    std::vector<std::string> Output;
    while(substr != NULL) {
        Output.push_back(substr);
    }
    delete[] Array;

    return Output;
}

int PollClock(const std::string Dir) {
    std::string File;
    int FileInt;
    for (const auto & entry : fs::directory_iterator(Dir))
        File = fs::path(entry.path()).filename();
    if(File == "")
        FileInt = -1;
    else
        FileInt = stoi(File);
#ifdef COUT
    std::cout << FileInt << std::endl;
#endif
    return FileInt;
}

void Touch(std::string File) {
    std::fstream fs;
    fs.open(File, std::ios::out);
    fs.close();
}

void Data(std::string Text, std::string File) {
    std::vector<std::string> Lines;
    Lines.push_back(Text);
    WriteFileLines(Lines, File);
}
