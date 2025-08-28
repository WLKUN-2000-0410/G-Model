// CurveDataManager.h

#ifndef CURVEDATAMANAGER_H
#define CURVEDATAMANAGER_H

#include <string>
#include <vector>
// #include <map> // 不再需要 map

/**
 * @struct CurveData
 * @brief 用于存储单条曲线数据的结构体。
 */
struct CurveData
{
    std::string name;                 // 曲线的唯一名称
    std::vector<double> x_values;     // X轴数据点
    std::vector<double> y_values;     // Y轴数据点
};

/**
 * @class CurveDataManager
 * @brief (单例) 负责管理当前会话中所有临时曲线的生命周期。
 *
 * (方案C) 内部使用 std::vector 存储，保证了曲线的添加顺序。
 */
class CurveDataManager
{
public:
    /**
     * @brief 获取 CurveDataManager 的全局唯一实例。
     */
    static CurveDataManager& GetInstance();

    // --- 核心管理功能 ---

    /**
     * @brief 添加一条新曲线到管理器中。
     * 内部会检查重名，如果传入的名称已存在，则添加失败。
     * @param newCurve 要添加的曲线数据。
     * @return 如果曲线名称不重复且添加成功，返回 true；否则返回 false。
     */
    bool addCurve(const CurveData& newCurve);

    /**
     * @brief 根据名称删除一条曲线。
     * @param curveName 要删除的曲线的名称。
     * @return 如果找到并成功删除，返回 true；否则返回 false。
     */
    bool deleteCurve(const std::string& curveName);
    
    /**
     * @brief 检查曲线名是否已经存在。
     * @param curveName 要检查的曲线名。
	 * @return 如果已经存在，返回 true；否则返回 false。
     */
    bool isCurveNameExists(const std::string& curveName) const;

    /**
     * @brief 重命名一条已存在的曲线。
     * @param oldName 曲线的当前名称。
     * @param newName 曲线的新名称。
     * @return 如果重命名成功，返回 true；如果旧名称不存在或新名称已存在，返回 false。
     */
    bool renameCurve(const std::string& oldName, const std::string& newName);

    // --- 数据查询功能 ---

    /**
     * @brief 根据名称获取一条曲线的只读数据指针。
     * @param curveName 要查找的曲线的名称。
     * @return 如果找到，返回指向该曲线数据的 const 指针；否则返回 nullptr。
     */
    const CurveData* getCurveByName(const std::string& curveName) const;

    /**
     * @brief 获取最新添加的一条曲线的只读数据指针。
     * @return 如果管理器中有曲线，返回指向最后一条曲线的 const 指针；否则返回 nullptr。
     */
    const CurveData* getLastCurve() const;

    /**
     * @brief 获取当前管理器中所有曲线的名称列表。
     * @return 一个包含所有曲线名称的字符串向量（按添加顺序列出）。
     */
    std::vector<std::string> getAllCurveNames() const;

    // --- 文件操作功能 ---

    // 新增：从单个文件导入一条曲线
    bool importCurveFromFile(const std::string& filePath);

    // 新增：将指定名称的曲线导出到单个文件
    bool exportCurveToFile(const std::string& curveName, const std::string& filePath) const;

private:
    /**
     * @brief 从一个双列文本文件中加载一条光谱曲线。
     * @details 文件每行应包含两个数值(X和Y)，可用空格、制表符或逗号分隔。
     * @param filePath 要加载的文件路径 (e.g., "my_spectrum.csv")。
     * @param outSpectrum [输出] 用于接收加载的光谱数据。
     * @return 如果加载成功，返回 true。
     */
    static bool loadSpectrum(const std::string& filePath,
        CurveData& outSpectrum);

    /**
     * @brief 将一条光谱的XY数据保存至文件。
     * @details 会根据文件扩展名自动选择分隔符 (逗号 for .csv, 制表符 for .txt)。
     * @param filePath 要保存的文件路径 (e.g., "result.txt")。
     * @param spectrum 要保存的光谱数据。
     * @return 如果保存成功，返回 true。
     */
    static bool saveSpectrum(const std::string& filePath,
        const CurveData& spectrum);

    
private:
    // --- 单例模式实现 ---
    CurveDataManager();
    ~CurveDataManager();
    CurveDataManager(const CurveDataManager&) = delete;
    CurveDataManager& operator=(const CurveDataManager&) = delete;

    // --- 内部数据存储 ---
    // 使用 std::vector 存储曲线，可以保证元素的顺序与添加的顺序一致。
    std::vector<CurveData> m_curveRegistry;
};

#endif // CURVEDATAMANAGER_H