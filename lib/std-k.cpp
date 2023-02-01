#include <iostream>
#include <array>

std::string ExecCommand(const std::string cmd, const int Output, int ExitStatus) {
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

