#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <unordered_map>
#include <sstream>
#include <array>
#include <unistd.h>

// Colores ANSI
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_WHITE   "\x1b[37m"
#define ANSI_COLOR_BLACK   "\x1b[30m"
#define ANSI_RESET         "\x1b[0m"

// Iconos
#define ICON_KERNEL   ""
#define ICON_UPTIME   ""
#define ICON_DISTRO   ""
#define ICON_USER     ""
#define ICON_HOST     ""
#define ICON_COLORS   ""

// Usa '-' en lugar de '─' para evitar advertencias
std::string make_line(int n) {
    return std::string(n, '-');
}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "N/A";
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe) != nullptr) {
        result += buffer.data();
    }
    pclose(pipe);
    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }
    return result;
}

std::string getDistroName() {
    std::string pretty_name = exec("grep '^PRETTY_NAME=' /etc/os-release | cut -d'=' -f2 | tr -d '\"'");
    if (!pretty_name.empty()) return pretty_name;
    std::string id = exec("grep '^ID=' /etc/os-release | cut -d'=' -f2 | tr -d '\"'");
    if (id.empty()) return "Linux";
    if (id == "arch") return "Arch Linux";
    return id + " Linux";
}

std::string getDistroID() {
    std::string id = exec("grep '^ID=' /etc/os-release | cut -d'=' -f2 | tr -d '\"'");
    if (id.empty()) return "generic";
    std::transform(id.begin(), id.end(), id.begin(), ::tolower);
    id.erase(std::remove(id.begin(), id.end(), '-'), id.end());
    return id;
}

std::vector<std::string> splitLines(const std::string& text) {
    std::vector<std::string> lines;
    std::istringstream iss(text);
    std::string line;
    while (std::getline(iss, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        lines.push_back(line);
    }
    return lines;
}

std::vector<std::string> getLogoLines(const std::string& distro_id) {
    static const std::unordered_map<std::string, std::string> logos = {
        {"arch", R"(    ___              __  
   /   |  __________/ /_ 
  / /| | / ___/ ___/ __ \
 / ___ |/ /  / /__/ / / /
/_/  |_/_/   \___/_/ /_/ )"},

        {"debian", R"(    ____       __    _           
   / __ \___  / /_  (_)___ _____ 
  / / / / _ \/ __ \/ / __ `/ __ \
 / /_/ /  __/ /_/ / / /_/ / / / /
/_____/\___/_.___/_/\__,_/_/ /_/ )"},

        {"fedora", R"(    ______         __                
   / ____/__  ____/ /___  _________ _
  / /_  / _ \/ __  / __ \/ ___/ __ `/
 / __/ /  __/ /_/ / /_/ / /  / /_/ / 
/_/    \___/\__,_/\____/_/   \__,_/ )"},

        {"ubuntu", R"(UBUNTU)"},

        {"suse", R"(   _____               
  / ___/__  __________ 
  \__ \/ / / / ___/ _ \
 ___/ / /_/ (__  )  __/
/____/\__,_/____/\___/ )"},

        {"nixos", R"(    _   ___      ____  _____
   / | / (_)  __/ __ \/ ___/
  /  |/ / / |/_/ / / /\__ \ 
 / /|  / />  </ /_/ /___/ / 
/_/ |_/_/_/|_|\____//____/  )"},

        {"mint", R"(    __  ____       __ 
   /  |/  (_)___  / /_
  / /|_/ / / __ \/ __/
 / /  / / / / / / /_  
/_/  /_/_/_/ /_/\__/  )"},

        {"manjaro", R"(    __  ___              _                
   /  |/  /___ _____    (_)___ __________ 
  / /|_/ / __ `/ __ \  / / __ `/ ___/ __ \
 / /  / / /_/ / / / / / / /_/ / /  / /_/ /
/_/  /_/\__,_/_/ /_/_/ /\__,_/_/   \____/ 
                  /___/                   )"},

        {"generic", R"(    __    _                 
   / /   (_)___  __  ___  __
  / /   / / __ \/ / / / |/_/
 / /___/ / / / / /_/ />  <  
/_____/_/_/ /_/\__,_/_/|_|  )"}
    };

    auto it = logos.find(distro_id);
    if (it != logos.end()) {
        return splitLines(it->second);
    }
    return splitLines(logos.at("generic"));
}

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

std::vector<std::string> getAllDistroIDs() {
    return {"arch", "debian", "fedora", "ubuntu", "suse", "nixos", "mint", "manjaro", "generic"};
}

void showHelp() {
    std::cout << "Usage: linver [OPTION]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --help         Show this help message\n";
    std::cout << "  --list-logos   Show all available logos\n";
    std::cout << "\n";
    std::cout << "linver displays system info with a distribution-specific ASCII logo.\n";
    std::cout << "Without arguments, it runs normally.\n";
}

void showAvailableLogos() {
    std::cout << "Available logos:\n\n";
    for (const auto& id : getAllDistroIDs()) {
        std::cout << "● " << id << "\n";
        auto lines = getLogoLines(id);
        for (const auto& line : lines) {
            std::cout << "    " << line << "\n";
        }
        std::cout << "\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        std::string arg = argv[1];
        if (arg == "--help") {
            showHelp();
            return 0;
        } else if (arg == "--list-logos") {
            showAvailableLogos();
            return 0;
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            std::cerr << "Try 'linver --help' for more information.\n";
            return 1;
        }
    } else if (argc > 2) {
        std::cerr << "Too many arguments.\n";
        std::cerr << "Try 'linver --help' for more information.\n";
        return 1;
    }

    std::string kernel = exec("uname -r");
    std::string uptime = exec("uptime -p");
    std::string arch = exec("uname -m");
    std::string user = exec("whoami");
    std::string host = exec("hostname");
    std::string distro_name = getDistroName();
    std::string distro_id = getDistroID();

    auto logo_lines = getLogoLines(distro_id);

    while (!logo_lines.empty() && trim(logo_lines.front()).empty()) {
        logo_lines.erase(logo_lines.begin());
    }
    while (!logo_lines.empty() && trim(logo_lines.back()).empty()) {
        logo_lines.pop_back();
    }

    const int total_width = 50;

    if (!logo_lines.empty()) {
        int max_logo_width = 0;
        for (const auto& line : logo_lines) {
            if (static_cast<int>(line.length()) > max_logo_width)
                max_logo_width = static_cast<int>(line.length());
        }
        int pad = (total_width - max_logo_width) / 2;
        if (pad < 0) pad = 0;
        for (const auto& line : logo_lines) {
            std::cout << std::string(pad, ' ') << line << "\n";
        }
    }
    std::cout << "\n";

    std::cout << "+" << make_line(total_width - 2) << "+\n";
    std::cout << "  " << ANSI_COLOR_CYAN << ICON_KERNEL << " kernel > " << ANSI_RESET << kernel << "\n";
    std::cout << "  " << ANSI_COLOR_GREEN << ICON_UPTIME << " uptime > " << ANSI_RESET << uptime << "\n";
    std::cout << "  " << ANSI_COLOR_YELLOW << ICON_DISTRO << " distro > " << ANSI_RESET << distro_name << " " << arch << "\n";
    std::cout << "  " << ANSI_COLOR_BLUE << ICON_USER << " user   > " << ANSI_RESET << user << "\n";
    std::cout << "  " << ANSI_COLOR_MAGENTA << ICON_HOST << " host   > " << ANSI_RESET << host << "\n";
    std::cout << "+" << make_line(total_width - 2) << "+\n\n";

    std::cout << "+" << make_line(total_width - 2) << "+\n";
    std::cout << " " << ANSI_COLOR_GREEN << ICON_COLORS << " colors > " << ANSI_RESET
              << ANSI_COLOR_RED << "● " << ANSI_RESET
              << ANSI_COLOR_GREEN << "● " << ANSI_RESET
              << ANSI_COLOR_YELLOW << "● " << ANSI_RESET
              << ANSI_COLOR_BLUE << "● " << ANSI_RESET
              << ANSI_COLOR_MAGENTA << "● " << ANSI_RESET
              << ANSI_COLOR_CYAN << "● " << ANSI_RESET
              << ANSI_COLOR_WHITE << "●" << ANSI_RESET
              << "\n";
    std::cout << "+" << make_line(total_width - 2) << "+\n\n";

    std::cout << "+" << make_line(total_width - 2) << "+\n";
    std::cout << "  This product is licensed under the GNU GPL.\n";
    std::cout << "  Terms to: " << user << "\n";
    std::cout << "+" << make_line(total_width - 2) << "+\n";

    return 0;
}
