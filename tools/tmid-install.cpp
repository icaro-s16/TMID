#include <iostream>
#include <filesystem>
#include <string>
#include <cstdlib>

#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>

bool install_tmid(const std::filesystem::path& binary_path) {
    if (!std::filesystem::exists(binary_path)) {
        std::cerr << "Error: Could not find binary at " << binary_path << "\n";
        return false;
    }

    std::string folder_path = binary_path.parent_path().string();
    std::string command = "setx PATH \"%PATH%;" + folder_path + "\"";

    return (std::system(command.c_str()) == 0);
}

#elif defined(__linux__) || defined(__APPLE__)

#include <unistd.h>

bool install_tmid(const std::filesystem::path& binary_path) {
    if (getuid() != 0) {
        std::cerr << "Error: This installer requires root privileges.\n";
        std::cerr << "Please run: sudo ./tmid-install\n";
        return false;
    }

    if (!std::filesystem::exists(binary_path)) {
        std::cerr << "Error: Could not find binary at " << binary_path << "\n";
        return false;
    }

    const std::string link_path = "/usr/local/bin/tmid";
    std::string command = "ln -sf \"" + binary_path.string() + "\" " + link_path;

    return (std::system(command.c_str()) == 0);
}

#endif

int main() {
    std::filesystem::path project_root = std::filesystem::current_path().parent_path();
    std::filesystem::path tmid_binary = project_root / "build" / "tmid";

#if defined(_WIN32) || defined(_WIN64)
    tmid_binary.replace_extension(".exe");
#endif

    if (install_tmid(tmid_binary)) {
        std::cout << "\nSuccess!\n";
    } else {
        std::cerr << "\nInstallation failed.\n";
        return 1;
    }

    return 0;
}