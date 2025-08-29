#include "ModelArchiveManager.h"
#include "PathUtils.h"       // �������ǵ�С����
#include <windows.h>         // ���� Windows ����ͷ�ļ�
#include <iostream>
#include <direct.h>          // Ϊ�� _mkdir

// --- ˽�и������� ---

std::string ModelArchiveManager::getBaseDataPath() {
    return PathUtils::combine(PathUtils::getExecutableDir(), "ModelData");
}

bool ModelArchiveManager::ensureDirectoryExists(const std::string& path) {
    std::string tempPath = path;
    for (char& c : tempPath) {
        if (c == '/') c = '\\'; // ͳһΪWindows����'\'
    }

    // ��ͷ��ʼ�𼶴���Ŀ¼
    for (size_t i = 1; i < tempPath.length(); ++i) {
        if (tempPath[i] == '\\') {
            std::string subDir = tempPath.substr(0, i);
            // ���Ŀ¼�Ƿ���ڣ��������򴴽�
            if (GetFileAttributesA(subDir.c_str()) == INVALID_FILE_ATTRIBUTES) {
                if (_mkdir(subDir.c_str()) != 0) {
                    return false; // ����ʧ��
                }
            }
        }
    }
    // �������һ��Ŀ¼
    if (GetFileAttributesA(tempPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        if (_mkdir(tempPath.c_str()) != 0) {
            return false;
        }
    }
    return true;
}

// --- �����ӿ�ʵ�� ---

bool ModelArchiveManager::archiveCurves(
    const std::string& gasName,
    const std::string& modelType,
    const std::vector<CurveData>& rawCurves,
    const std::vector<CurveData>& processedCurves)
{
    try {
        // 1. ����Ŀ¼·��
        std::string basePath = getBaseDataPath();
        std::string rawDataPath = PathUtils::combine(PathUtils::combine(PathUtils::combine(basePath, gasName), modelType + "��������"), "�ۻ���֮ǰ����������");
        std::string processedDataPath = PathUtils::combine(PathUtils::combine(PathUtils::combine(basePath, gasName), modelType + "��������"), "�ۻ��ߺ����������");

        // 2. ����Ŀ¼�ṹ
        if (!ensureDirectoryExists(rawDataPath) || !ensureDirectoryExists(processedDataPath)) {
            std::cerr << "Error: Failed to create archive directories." << std::endl;
            return false;
        }

        // 3. ��������ԭʼ����
        for (const auto& curve : rawCurves) {
            std::string filePath = PathUtils::combine(rawDataPath, curve.name + ".txt");
            if (!CurveDataManager::saveSpectrum(filePath, curve)) {
                std::cerr << "Error: Failed to save raw curve to " << filePath << std::endl;
                return false;
            }
        }

        // 4. �������д���������
        for (const auto& curve : processedCurves) {
            std::string filePath = PathUtils::combine(processedDataPath, curve.name + ".txt");
            if (!CurveDataManager::saveSpectrum(filePath, curve)) {
                std::cerr << "Error: Failed to save processed curve to " << filePath << std::endl;
                return false;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in archiveCurves: " << e.what() << std::endl;
        return false;
    }
    return true;
}


bool ModelArchiveManager::loadArchivedCurves(
    const std::string& gasName,
    const std::string& modelType,
    std::vector<CurveData>& outRawCurves,
    std::vector<CurveData>& outProcessedCurves)
{
    outRawCurves.clear();
    outProcessedCurves.clear();

    std::string basePath = getBaseDataPath();
    std::string rawDataPath = PathUtils::combine(PathUtils::combine(PathUtils::combine(basePath, gasName), modelType + "��������"), "�ۻ���֮ǰ����������");
    std::string processedDataPath = PathUtils::combine(PathUtils::combine(PathUtils::combine(basePath, gasName), modelType + "��������"), "�ۻ��ߺ����������");

    WIN32_FIND_DATAA findFileData;
    HANDLE hFind;

    // --- ����ԭʼ���� ---
    std::string rawSearchPath = PathUtils::combine(rawDataPath, "*.txt");
    hFind = FindFirstFileA(rawSearchPath.c_str(), &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                std::string fullPath = PathUtils::combine(rawDataPath, findFileData.cFileName);
                CurveData loadedCurve;
                if (CurveDataManager::loadSpectrum(fullPath, loadedCurve)) {
                    std::string fileName = findFileData.cFileName;
                    loadedCurve.name = fileName.substr(0, fileName.find_last_of('.')); // ȥ��.txt��׺
                    outRawCurves.push_back(loadedCurve);
                }
            }
        } while (FindNextFileA(hFind, &findFileData) != 0);
        FindClose(hFind);
    }

    // --- ���ش��������� ---
    std::string processedSearchPath = PathUtils::combine(processedDataPath, "*.txt");
    hFind = FindFirstFileA(processedSearchPath.c_str(), &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                std::string fullPath = PathUtils::combine(processedDataPath, findFileData.cFileName);
                CurveData loadedCurve;
                if (CurveDataManager::loadSpectrum(fullPath, loadedCurve)) {
                    std::string fileName = findFileData.cFileName;
                    loadedCurve.name = fileName.substr(0, fileName.find_last_of('.')); // ȥ��.txt��׺
                    outProcessedCurves.push_back(loadedCurve);
                }
            }
        } while (FindNextFileA(hFind, &findFileData) != 0);
        FindClose(hFind);
    }

    return !(outRawCurves.empty() && outProcessedCurves.empty());
}