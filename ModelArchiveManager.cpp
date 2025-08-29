#include "ModelArchiveManager.h"
#include "PathUtils.h"       // 引入我们的小工具
#include <windows.h>         // 引入 Windows 核心头文件
#include <iostream>
#include <direct.h>          // 为了 _mkdir

// --- 私有辅助函数 ---

std::string ModelArchiveManager::getBaseDataPath() {
    return PathUtils::combine(PathUtils::getExecutableDir(), "ModelData");
}

bool ModelArchiveManager::ensureDirectoryExists(const std::string& path) {
    std::string tempPath = path;
    for (char& c : tempPath) {
        if (c == '/') c = '\\'; // 统一为Windows风格的'\'
    }

    // 从头开始逐级创建目录
    for (size_t i = 1; i < tempPath.length(); ++i) {
        if (tempPath[i] == '\\') {
            std::string subDir = tempPath.substr(0, i);
            // 检查目录是否存在，不存在则创建
            if (GetFileAttributesA(subDir.c_str()) == INVALID_FILE_ATTRIBUTES) {
                if (_mkdir(subDir.c_str()) != 0) {
                    return false; // 创建失败
                }
            }
        }
    }
    // 创建最后一级目录
    if (GetFileAttributesA(tempPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        if (_mkdir(tempPath.c_str()) != 0) {
            return false;
        }
    }
    return true;
}

// --- 公共接口实现 ---

bool ModelArchiveManager::archiveCurves(
    const std::string& gasName,
    const std::string& modelType,
    const std::vector<CurveData>& rawCurves,
    const std::vector<CurveData>& processedCurves)
{
    try {
        // 1. 构建目录路径
        std::string basePath = getBaseDataPath();
        std::string rawDataPath = PathUtils::combine(PathUtils::combine(PathUtils::combine(basePath, gasName), modelType + "曲线数据"), "扣基线之前的曲线数据");
        std::string processedDataPath = PathUtils::combine(PathUtils::combine(PathUtils::combine(basePath, gasName), modelType + "曲线数据"), "扣基线后的数据数据");

        // 2. 创建目录结构
        if (!ensureDirectoryExists(rawDataPath) || !ensureDirectoryExists(processedDataPath)) {
            std::cerr << "Error: Failed to create archive directories." << std::endl;
            return false;
        }

        // 3. 保存所有原始曲线
        for (const auto& curve : rawCurves) {
            std::string filePath = PathUtils::combine(rawDataPath, curve.name + ".txt");
            if (!CurveDataManager::saveSpectrum(filePath, curve)) {
                std::cerr << "Error: Failed to save raw curve to " << filePath << std::endl;
                return false;
            }
        }

        // 4. 保存所有处理后的曲线
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
    std::string rawDataPath = PathUtils::combine(PathUtils::combine(PathUtils::combine(basePath, gasName), modelType + "曲线数据"), "扣基线之前的曲线数据");
    std::string processedDataPath = PathUtils::combine(PathUtils::combine(PathUtils::combine(basePath, gasName), modelType + "曲线数据"), "扣基线后的数据数据");

    WIN32_FIND_DATAA findFileData;
    HANDLE hFind;

    // --- 加载原始曲线 ---
    std::string rawSearchPath = PathUtils::combine(rawDataPath, "*.txt");
    hFind = FindFirstFileA(rawSearchPath.c_str(), &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                std::string fullPath = PathUtils::combine(rawDataPath, findFileData.cFileName);
                CurveData loadedCurve;
                if (CurveDataManager::loadSpectrum(fullPath, loadedCurve)) {
                    std::string fileName = findFileData.cFileName;
                    loadedCurve.name = fileName.substr(0, fileName.find_last_of('.')); // 去掉.txt后缀
                    outRawCurves.push_back(loadedCurve);
                }
            }
        } while (FindNextFileA(hFind, &findFileData) != 0);
        FindClose(hFind);
    }

    // --- 加载处理后的曲线 ---
    std::string processedSearchPath = PathUtils::combine(processedDataPath, "*.txt");
    hFind = FindFirstFileA(processedSearchPath.c_str(), &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                std::string fullPath = PathUtils::combine(processedDataPath, findFileData.cFileName);
                CurveData loadedCurve;
                if (CurveDataManager::loadSpectrum(fullPath, loadedCurve)) {
                    std::string fileName = findFileData.cFileName;
                    loadedCurve.name = fileName.substr(0, fileName.find_last_of('.')); // 去掉.txt后缀
                    outProcessedCurves.push_back(loadedCurve);
                }
            }
        } while (FindNextFileA(hFind, &findFileData) != 0);
        FindClose(hFind);
    }

    return !(outRawCurves.empty() && outProcessedCurves.empty());
}