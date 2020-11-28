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

int mapToOutput() {
    std::string device = exec("xinput list | grep -i '↳' | sed 's|[↳,\t,⎜]||g' | dmenu -i -l 15 -p 'Device: '");
    if(device.empty())
    	return 0;
    device = device.substr(device.find("id=")+3);
    device = device.substr(0, device.find('['));
    std::string output = exec("xrandr --listactivemonitors | grep -i -E '[0-9]:' | dmenu -i -l 15 -p 'Map to output: '");
    if(output.empty())
    	return 0;
    std::size_t found = output.find_last_of(' ');
   	output = output.substr(found+1);
    std::string wacomExec = "xsetwacom set \""+device+"\" MapToOutput "+output;
    std::cout << exec(wacomExec.c_str()) << std::endl;
    return 0;
}

int main(int argc, char** argv) {
	mapToOutput();
	return 0;
}
