#ifndef MODELARCHIVEMANAGER_H
#define MODELARCHIVEMANAGER_H

#include <string>
#include <vector>
#include "CurveDataManager.h" // 引用 CurveData 结构体

class ModelArchiveManager
{
public:
    /**
     * @brief 1. 根据气体名和模型类型，将曲线数据归档。
     * @details 存储路径为：[程序目录]/ModelData/[气体名]/[模型类型]曲线数据/...
     */
    static bool archiveCurves(
        const std::string& gasName,
        const std::string& modelType,
        const std::vector<CurveData>& rawCurves,
        const std::vector<CurveData>& processedCurves);

    /**
     * @brief 2. 根据气体名和模型类型，从归档中加载曲线数据。
     */
    static bool loadArchivedCurves(
        const std::string& gasName,
        const std::string& modelType,
        std::vector<CurveData>& outRawCurves,
        std::vector<CurveData>& outProcessedCurves);

private:
    ModelArchiveManager() = default;

    /**
     * @brief 获取数据存储的根目录，即 [程序目录]/ModelData
     */
    static std::string getBaseDataPath();

    /**
     * @brief (辅助函数) 确保一个多层目录路径存在，如果不存在则创建它。
     * @param path 要创建的完整目录路径。
     * @return 如果目录存在或创建成功，返回 true。
     */
    static bool ensureDirectoryExists(const std::string& path);
};

#endif // MODELARCHIVEMANAGER_H