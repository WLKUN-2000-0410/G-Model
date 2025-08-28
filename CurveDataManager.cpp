// CurveDataManager.cpp

#include "CurveDataManager.h"
#include <algorithm> // 需要包含此头文件以使用 std::find_if
#include <iostream>  // 用于打印一些调试信息（可选）
#include <fstream>      // 用于文件输入输出流 (ifstream, ofstream)
#include <sstream>      // 用于字符串流，方便解析每行数据
#include <iomanip>     //用于设置输出精度
#include <cctype>       // 添加此头文件以使用 std::tolower

// --- 单例模式实现 ---

CurveDataManager::CurveDataManager()
{
    // 构造函数，可以在这里进行一些初始化操作
}

CurveDataManager::~CurveDataManager()
{
    // 析构函数，可以在这里进行清理操作
}

// 静态方法，用于获取全局唯一的实例
CurveDataManager& CurveDataManager::GetInstance()
{
    // C++11及以后版本保证了这种局部静态变量的初始化是线程安全的
    static CurveDataManager instance;
    return instance;
}


// --- 核心管理功能实现 ---

/**
 * @brief 添加一条新曲线到管理器中。
 */
bool CurveDataManager::addCurve(const CurveData& newCurve)
{
    // 检查传入的曲线名称是否为空
    if (newCurve.name.empty()) {
        std::cerr << "Error: Curve name cannot be empty." << std::endl;
        return false;
    }

    // 检查曲线名称是否已经存在（遍历vector查找）
    for (const auto& existingCurve : m_curveRegistry) {
        if (existingCurve.name == newCurve.name) {
            // 如果找到了同名曲线，则添加失败
            std::cerr << "Error: Curve with name '" << newCurve.name << "' already exists." << std::endl;
            return false;
        }
    }

    // 如果名称不重复，则将新曲线添加到vector的末尾
    m_curveRegistry.push_back(newCurve);
    std::cout << "Curve '" << newCurve.name << "' added successfully." << std::endl;
    return true;
}

/**
 * @brief 根据名称删除一条曲线。
 */
bool CurveDataManager::deleteCurve(const std::string& curveName)
{
    // 使用 std::find_if 和 lambda 表达式来查找要删除的曲线
    auto it = std::find_if(m_curveRegistry.begin(), m_curveRegistry.end(),
        [&curveName](const CurveData& curve) {
            return curve.name == curveName;
        });

    // 如果找到了匹配的曲线 (iterator不等于end)
    if (it != m_curveRegistry.end()) {
        // 从vector中移除该元素
        m_curveRegistry.erase(it);
        std::cout << "Curve '" << curveName << "' deleted successfully." << std::endl;
        return true;
    }

    // 如果遍历完没有找到，则删除失败
    std::cerr << "Error: Curve with name '" << curveName << "' not found for deletion." << std::endl;
    return false;
}
/**
 * @brief 检查指定的曲线名称是否已经存在。
 */
bool CurveDataManager::isCurveNameExists(const std::string& curveName) const
{
    // 使用 std::find_if 查找是否存在同名曲线
    auto it = std::find_if(m_curveRegistry.begin(), m_curveRegistry.end(),
        [&curveName](const CurveData& curve) {
            return curve.name == curveName;
        });

    // 如果迭代器不等于end，意味着找到了，名称已存在
    return it != m_curveRegistry.end();
}

/**
 * @brief 重命名一条已存在的曲线。
 */
bool CurveDataManager::renameCurve(const std::string& oldName, const std::string& newName)
{
    // 1. 检查新名称是否有效（不能为空）
    if (newName.empty()) {
        return false; // 新名称不能为空
    }

    // 2. 检查新名称是否已经存在 (调用我们刚刚创建的函数)
    //    同时确保新旧名称不相同
    if (oldName != newName && isCurveNameExists(newName)) {
        return false; // 新名称已被其他曲线占用
    }

    // 3. 查找要重命名的旧曲线
    auto it = std::find_if(m_curveRegistry.begin(), m_curveRegistry.end(),
        [&oldName](const CurveData& curve) {
            return curve.name == oldName;
        });

    // 4. 如果找到了旧曲线
    if (it != m_curveRegistry.end()) {
        // 直接修改该曲线结构体中的 name 成员
        it->name = newName;
        return true;
    }

    // 5. 如果未找到旧曲线，则重命名失败
    return false;
}
const CurveData* CurveDataManager::getCurveByName(const std::string& curveName) const
{
    // 使用 std::find_if 查找匹配名称的曲线
    auto it = std::find_if(m_curveRegistry.begin(), m_curveRegistry.end(),
        [&curveName](const CurveData& curve) {
            return curve.name == curveName;
        });

    // 如果找到了
    if (it != m_curveRegistry.end()) {
        // 返回指向该元素的指针。
        // &(*it) 的意思是：先用 *it 解引用迭代器得到对象本身，再用 & 取该对象的地址。
        return &(*it);
    }

    // 如果没有找到，返回空指针
    return nullptr;
}

/**
 * @brief 获取最新添加的一条曲线的只读数据指针。
 */
const CurveData* CurveDataManager::getLastCurve() const
{
    // 首先检查 vector 是否为空
    if (m_curveRegistry.empty()) {
        // 如果是空的，没有任何曲线，返回空指针
        return nullptr;
    }

    // 如果不为空，使用 .back() 方法直接获取最后一个元素的引用，
    // 然后用 & 操作符取其地址，返回指针。
    return &m_curveRegistry.back();
}
std::vector<std::string> CurveDataManager::getAllCurveNames() const
{
    // 1. 创建一个空的字符串向量，用于存放所有名称
    std::vector<std::string> names;

    // 2. (可选但推荐) 为向量预留空间，可以提高性能，避免多次内存重新分配
    names.reserve(m_curveRegistry.size());

    // 3. 遍历内部存储曲线的 m_curveRegistry 向量
    for (const auto& curve : m_curveRegistry) {
        // 4. 将每条曲线的 name 成员添加到 names 向量中
        names.push_back(curve.name);
    }

    // 5. 返回填充好的名称列表
    return names;
}

/**
 * @brief [辅助函数] 获取文件路径的小写扩展名。
 * @note 这是一个静态辅助函数，仅在此文件内部可见和使用。
 * @param filePath 文件路径字符串。
 * @return 文件扩展名 (例如, ".csv" 或 ".txt")。
 */
static std::string getFileExtension(const std::string& filePath) {
    size_t dotPos = filePath.find_last_of('.');
    if (dotPos != std::string::npos) {
        std::string ext = filePath.substr(dotPos);
        // 转换为小写以便于比较
        for (char& c : ext) {
            c = static_cast<char>(std::tolower(c));
        }
        return ext;
    }
    return ""; // 没有找到扩展名
}

// --- FileManager 类成员函数的实现 ---

bool CurveDataManager::loadSpectrum(const std::string& filePath, CurveData& outSpectrum)
{
    // 1. 创建一个输入文件流对象
    std::ifstream inFile(filePath);

    // 2. 检查文件是否成功打开
    if (!inFile.is_open()) {
        std::cerr << "错误 [FileManager]: 无法打开文件进行读取: " << filePath << std::endl;
        return false;
    }

    // 3. 清空外部传入的vector，确保它们只包含本次加载的数据
    outSpectrum.x_values.clear();
    outSpectrum.y_values.clear();

    std::string line;
    int lineNumber = 0;
    // 4. 逐行读取文件
    while (std::getline(inFile, line)) {
        lineNumber++;

        // 跳过空行或注释行 (例如以'#'开头的行)
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // 预处理：将逗号替换为空格，以统一处理分隔符
        for (char& c : line) {
            if (c == ',') {
                c = ' ';
            }
        }

        // 使用字符串流来解析处理过的行
        std::stringstream ss(line);
        double x, y;

        // 尝试从行中提取两个double值
        if (ss >> x >> y) {
            // 如果成功，将数据存入输出的Spectrum对象中
            outSpectrum.x_values.push_back(x);
            outSpectrum.y_values.push_back(y);
        }
        else {
            // 如果行不为空但无法解析出两个数值，则发出警告
            std::cerr << "警告 [FileManager]: 文件 " << filePath << " 的第 " << lineNumber
                << " 行数据格式不正确，已跳过此行。" << std::endl;
        }
    }

    // 5. 关闭文件
    inFile.close();

    // 6. 检查是否成功加载了任何数据
    if (outSpectrum.x_values.empty()) {
        std::cerr << "错误 [FileManager]: 未能在文件 " << filePath << " 中找到任何有效的数据行。" << std::endl;
        return false;
    }

    return true;
}

bool CurveDataManager::saveSpectrum(const std::string& filePath, const CurveData& spectrum)
{
    // 1. 检查输入数据的X轴和Y轴点数是否一致
    if (spectrum.x_values.size() != spectrum.y_values.size()) {
        std::cerr << "错误 [FileManager]: 无法保存，因为X轴和Y轴的数据点数量不匹配。" << std::endl;
        return false;
    }

    // 2. 根据文件扩展名自动选择分隔符
    char delimiter = '\t'; // 默认为制表符 (适用于.txt)
    if (getFileExtension(filePath) == ".csv") {
        delimiter = ','; // 如果是.csv文件，则使用逗号
    }

    // 3. 创建一个输出文件流对象
    std::ofstream outFile(filePath);

    // 4. 检查文件是否成功打开
    if (!outFile.is_open()) {
        std::cerr << "错误 [FileManager]: 无法打开文件进行写入: " << filePath << std::endl;
        return false;
    }

    // 5. 设置浮点数输出的精度，以防数据损失
    outFile << std::fixed << std::setprecision(8);

    // 6. 逐行写入数据
    for (size_t i = 0; i < spectrum.x_values.size(); ++i) {
        outFile << spectrum.x_values[i] << delimiter << spectrum.y_values[i] << '\n';
    }

    // 7. 关闭文件
    outFile.close();

    return true;
}
/**
 * @brief 从文件中导入一条曲线并添加到管理器。
 * @param filePath 要导入的文件的完整路径。
 * @return 如果导入成功，返回 true；否则返回 false。
 */
bool CurveDataManager::importCurveFromFile(const std::string& filePath)
{
    // 1. 提取文件名作为曲线名称
    std::string curveName;

    // 查找最后一个路径分隔符（兼容Windows和Unix）
    size_t lastSlash = filePath.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        // 如果找到了路径分隔符，从它之后开始就是文件名
        curveName = filePath.substr(lastSlash + 1);
    }
    else {
        // 如果没有路径分隔符，整个字符串就是文件名
        curveName = filePath;
    }

    // 移除文件扩展名
    size_t lastDot = curveName.find_last_of('.');
    if (lastDot != std::string::npos) {
        curveName = curveName.substr(0, lastDot);
    }

    // 2. 检查曲线名是否为空
    if (curveName.empty()) {
        std::cerr << "错误: 无法从文件路径中提取有效的曲线名称。" << std::endl;
        return false;
    }

    // 3. 处理重名情况：如果曲线名已存在，自动添加数字后缀
    std::string finalName = curveName;
    int counter = 1;
    while (isCurveNameExists(finalName)) {
        finalName = curveName + "_" + std::to_string(counter);
        counter++;
    }

    // 4. 创建一个新的 CurveData 对象
    CurveData newCurve;
    newCurve.name = finalName;

    // 5. 调用 loadSpectrum 函数来加载文件数据
    if (!loadSpectrum(filePath, newCurve)) {
        std::cerr << "错误: 无法从文件 " << filePath << " 加载曲线数据。" << std::endl;
        return false;
    }

    // 6. 将加载的曲线添加到管理器中
    // 这里不需要再检查重名，因为我们已经处理过了
    m_curveRegistry.push_back(newCurve);

    // 如果最终名称与原始名称不同，说明有重名，给出提示
    if (finalName != curveName) {
        std::cout << "注意: 曲线名称 '" << curveName
            << "' 已存在，已自动重命名为 '" << finalName << "'。" << std::endl;
    }

    std::cout << "成功从文件 '" << filePath << "' 导入曲线 '" << finalName << "'。" << std::endl;
    return true;
}

/**
 * @brief 将管理器中指定名称的曲线导出到单个文件。
 * @param curveName 要导出的曲线的名称。
 * @param filePath 导出文件的目标路径。
 * @return 如果导出成功，返回 true；否则返回 false。
 */
bool CurveDataManager::exportCurveToFile(const std::string& curveName, const std::string& filePath) const
{
    // 1. 查找指定名称的曲线
    const CurveData* curvePtr = getCurveByName(curveName);

    // 2. 检查曲线是否存在
    if (curvePtr == nullptr) {
        std::cerr << "错误: 未找到名称为 '" << curveName << "' 的曲线。" << std::endl;
        return false;
    }

    // 3. 检查曲线数据是否有效
    if (curvePtr->x_values.empty() || curvePtr->y_values.empty()) {
        std::cerr << "错误: 曲线 '" << curveName << "' 的数据为空。" << std::endl;
        return false;
    }

    // 4. 检查数据点数量是否一致
    if (curvePtr->x_values.size() != curvePtr->y_values.size()) {
        std::cerr << "错误: 曲线 '" << curveName << "' 的X轴和Y轴数据点数量不匹配。" << std::endl;
        return false;
    }

    // 5. 调用 saveSpectrum 函数来保存数据到文件
    if (!saveSpectrum(filePath, *curvePtr)) {
        std::cerr << "错误: 无法将曲线 '" << curveName << "' 导出到文件 '" << filePath << "'。" << std::endl;
        return false;
    }

    std::cout << "成功将曲线 '" << curveName << "' 导出到文件 '" << filePath << "'。" << std::endl;
    return true;
}