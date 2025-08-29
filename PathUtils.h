#ifndef PATHUTILS_H
#define PATHUTILS_H
#include <windows.h> 
#include <string>

namespace PathUtils {

    /**
     * @brief ��ȡ��ִ���ļ�(.exe)���ڵ�Ŀ¼·����
     * @return ����exe����Ŀ¼������·����ĩβ����'\'��
     */
    std::string getExecutableDir() {
        char buffer[MAX_PATH];
        // ��ȡ��ִ���ļ�������·���������ļ�����
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        std::string exePath = buffer;
        // �������һ��'\'���Ӷ�����Ŀ¼���ļ���
        size_t lastSlash = exePath.find_last_of("\\/");
        if (std::string::npos != lastSlash) {
            return exePath.substr(0, lastSlash);
        }
        return ""; // ����Ҳ��������ؿ�
    }

    /**
     * @brief �������·��Ƭ�Ρ�
     * @param part1 ·���ĵ�һ���� (e.g., "C:\\Users")
     * @param part2 ·���ĵڶ����� (e.g., "MyFolder")
     * @return ��Ϻ��·�� (e.g., "C:\\Users\\MyFolder")
     */
    std::string combine(const std::string& part1, const std::string& part2) {
        if (part1.empty()) return part2;
        if (part2.empty()) return part1;

        // ȷ����һ��·����ĩβ����ֻ��һ��'\'
        char lastChar = part1.back();
        if (lastChar == '\\' || lastChar == '/') {
            return part1 + part2;
        }
        return part1 + "\\" + part2;
    }
}

#endif // PATHUTILS_H