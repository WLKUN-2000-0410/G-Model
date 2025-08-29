#ifndef PATHUTILS_H
#define PATHUTILS_H
#include <windows.h> 
#include <string>

namespace PathUtils {

    /**
     * @brief 获取可执行文件(.exe)所在的目录路径。
     * @return 返回exe所在目录的完整路径，末尾不带'\'。
     */
    std::string getExecutableDir() {
        char buffer[MAX_PATH];
        // 获取可执行文件的完整路径（包括文件名）
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        std::string exePath = buffer;
        // 查找最后一个'\'，从而分离目录和文件名
        size_t lastSlash = exePath.find_last_of("\\/");
        if (std::string::npos != lastSlash) {
            return exePath.substr(0, lastSlash);
        }
        return ""; // 如果找不到，返回空
    }

    /**
     * @brief 组合两个路径片段。
     * @param part1 路径的第一部分 (e.g., "C:\\Users")
     * @param part2 路径的第二部分 (e.g., "MyFolder")
     * @return 组合后的路径 (e.g., "C:\\Users\\MyFolder")
     */
    std::string combine(const std::string& part1, const std::string& part2) {
        if (part1.empty()) return part2;
        if (part2.empty()) return part1;

        // 确保第一个路径的末尾有且只有一个'\'
        char lastChar = part1.back();
        if (lastChar == '\\' || lastChar == '/') {
            return part1 + part2;
        }
        return part1 + "\\" + part2;
    }
}

#endif // PATHUTILS_H