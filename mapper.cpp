#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int main(int argc, char** argv) {
    std::string device = exec("xinput list | grep -i '↳' | dmenu -i -l 15 -p 'Device: '");
    if(device.empty())
    	return 0;
    device = device.substr(device.find("id=")+3);
    device = device.substr(0, device.find('\t'));
    std::string output = exec("xrandr | grep -i -E 'dvi|hdmi|vga' | grep -i ' connected' | dmenu -i -l 15 -p 'Map to output: '");
    if(output.empty())
    	return 0;
    output = output.substr(0, output.find(' '));
    std::string wacomExec = "xsetwacom set "+device+" MapToOutput "+output;
    std::cout << exec(wacomExec.c_str()) << std::endl;
	return 0;
}
