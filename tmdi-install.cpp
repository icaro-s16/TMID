#include <iostream>
#include <filesystem>
#include <string>
#include <unistd.h>

bool install_tmid(const std::filesystem::path& binary_path) {
    const std::string link_path = "/usr/local/bin/tmid";

    if (getuid() != 0) {
        std::cerr << "Error: This installer requires root privileges.\n";
        std::cerr << "Please run: sudo ./tmdi-install\n";
        return false;
    }

    if (!std::filesystem::exists(binary_path)) {
        std::cerr << "Error: Could not find binary at " << binary_path << "\n";
        return false;
    }

    std::cout << "Attempting to install 'tmid' symlink to " << link_path << "...\n";

    // -s: symbolic link, -f: force
    std::string command = "ln -sf \"" + binary_path.string() + "\" " + link_path;

    int result = std::system(command.c_str());

    return (result == 0);
}

int main(int argc, char* argv[]) {
    
    // Assuming the binary is in TMID/build/tmid relative to where the installer is run
    std::filesystem::path project_root = std::filesystem::current_path();
    std::filesystem::path tmid_binary = project_root / "build" / "tmid";

    if (install_tmid(tmid_binary)) {
        std::cout << "\nSuccess!\n";
    } else {
        std::cerr << "\nInstallation failed.\n";
        return 1;
    }

    return 0;
}