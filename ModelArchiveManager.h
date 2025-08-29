#ifndef MODELARCHIVEMANAGER_H
#define MODELARCHIVEMANAGER_H

#include <string>
#include <vector>
#include "CurveDataManager.h" // ���� CurveData �ṹ��

class ModelArchiveManager
{
public:
    /**
     * @brief 1. ������������ģ�����ͣ����������ݹ鵵��
     * @details �洢·��Ϊ��[����Ŀ¼]/ModelData/[������]/[ģ������]��������/...
     */
    static bool archiveCurves(
        const std::string& gasName,
        const std::string& modelType,
        const std::vector<CurveData>& rawCurves,
        const std::vector<CurveData>& processedCurves);

    /**
     * @brief 2. ������������ģ�����ͣ��ӹ鵵�м����������ݡ�
     */
    static bool loadArchivedCurves(
        const std::string& gasName,
        const std::string& modelType,
        std::vector<CurveData>& outRawCurves,
        std::vector<CurveData>& outProcessedCurves);

private:
    ModelArchiveManager() = default;

    /**
     * @brief ��ȡ���ݴ洢�ĸ�Ŀ¼���� [����Ŀ¼]/ModelData
     */
    static std::string getBaseDataPath();

    /**
     * @brief (��������) ȷ��һ�����Ŀ¼·�����ڣ�����������򴴽�����
     * @param path Ҫ����������Ŀ¼·����
     * @return ���Ŀ¼���ڻ򴴽��ɹ������� true��
     */
    static bool ensureDirectoryExists(const std::string& path);
};

#endif // MODELARCHIVEMANAGER_H