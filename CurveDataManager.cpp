// CurveDataManager.cpp

#include "CurveDataManager.h"
#include <algorithm> // ��Ҫ������ͷ�ļ���ʹ�� std::find_if
#include <iostream>  // ���ڴ�ӡһЩ������Ϣ����ѡ��
#include <fstream>      // �����ļ���������� (ifstream, ofstream)
#include <sstream>      // �����ַ��������������ÿ������
#include <iomanip>     //���������������
#include <cctype>       // ��Ӵ�ͷ�ļ���ʹ�� std::tolower

// --- ����ģʽʵ�� ---

CurveDataManager::CurveDataManager()
{
    // ���캯�����������������һЩ��ʼ������
}

CurveDataManager::~CurveDataManager()
{
    // ������������������������������
}

// ��̬���������ڻ�ȡȫ��Ψһ��ʵ��
CurveDataManager& CurveDataManager::GetInstance()
{
    // C++11���Ժ�汾��֤�����־ֲ���̬�����ĳ�ʼ�����̰߳�ȫ��
    static CurveDataManager instance;
    return instance;
}


// --- ���Ĺ�����ʵ�� ---

/**
 * @brief ���һ�������ߵ��������С�
 */
bool CurveDataManager::addCurve(const CurveData& newCurve)
{
    // ��鴫������������Ƿ�Ϊ��
    if (newCurve.name.empty()) {
        std::cerr << "Error: Curve name cannot be empty." << std::endl;
        return false;
    }

    // ������������Ƿ��Ѿ����ڣ�����vector���ң�
    for (const auto& existingCurve : m_curveRegistry) {
        if (existingCurve.name == newCurve.name) {
            // ����ҵ���ͬ�����ߣ������ʧ��
            std::cerr << "Error: Curve with name '" << newCurve.name << "' already exists." << std::endl;
            return false;
        }
    }

    // ������Ʋ��ظ�������������ӵ�vector��ĩβ
    m_curveRegistry.push_back(newCurve);
    std::cout << "Curve '" << newCurve.name << "' added successfully." << std::endl;
    return true;
}

/**
 * @brief ��������ɾ��һ�����ߡ�
 */
bool CurveDataManager::deleteCurve(const std::string& curveName)
{
    // ʹ�� std::find_if �� lambda ���ʽ������Ҫɾ��������
    auto it = std::find_if(m_curveRegistry.begin(), m_curveRegistry.end(),
        [&curveName](const CurveData& curve) {
            return curve.name == curveName;
        });

    // ����ҵ���ƥ������� (iterator������end)
    if (it != m_curveRegistry.end()) {
        // ��vector���Ƴ���Ԫ��
        m_curveRegistry.erase(it);
        std::cout << "Curve '" << curveName << "' deleted successfully." << std::endl;
        return true;
    }

    // ���������û���ҵ�����ɾ��ʧ��
    std::cerr << "Error: Curve with name '" << curveName << "' not found for deletion." << std::endl;
    return false;
}
/**
 * @brief ���ָ�������������Ƿ��Ѿ����ڡ�
 */
bool CurveDataManager::isCurveNameExists(const std::string& curveName) const
{
    // ʹ�� std::find_if �����Ƿ����ͬ������
    auto it = std::find_if(m_curveRegistry.begin(), m_curveRegistry.end(),
        [&curveName](const CurveData& curve) {
            return curve.name == curveName;
        });

    // ���������������end����ζ���ҵ��ˣ������Ѵ���
    return it != m_curveRegistry.end();
}

/**
 * @brief ������һ���Ѵ��ڵ����ߡ�
 */
bool CurveDataManager::renameCurve(const std::string& oldName, const std::string& newName)
{
    // 1. ����������Ƿ���Ч������Ϊ�գ�
    if (newName.empty()) {
        return false; // �����Ʋ���Ϊ��
    }

    // 2. ����������Ƿ��Ѿ����� (�������Ǹոմ����ĺ���)
    //    ͬʱȷ���¾����Ʋ���ͬ
    if (oldName != newName && isCurveNameExists(newName)) {
        return false; // �������ѱ���������ռ��
    }

    // 3. ����Ҫ�������ľ�����
    auto it = std::find_if(m_curveRegistry.begin(), m_curveRegistry.end(),
        [&oldName](const CurveData& curve) {
            return curve.name == oldName;
        });

    // 4. ����ҵ��˾�����
    if (it != m_curveRegistry.end()) {
        // ֱ���޸ĸ����߽ṹ���е� name ��Ա
        it->name = newName;
        return true;
    }

    // 5. ���δ�ҵ������ߣ���������ʧ��
    return false;
}
const CurveData* CurveDataManager::getCurveByName(const std::string& curveName) const
{
    // ʹ�� std::find_if ����ƥ�����Ƶ�����
    auto it = std::find_if(m_curveRegistry.begin(), m_curveRegistry.end(),
        [&curveName](const CurveData& curve) {
            return curve.name == curveName;
        });

    // ����ҵ���
    if (it != m_curveRegistry.end()) {
        // ����ָ���Ԫ�ص�ָ�롣
        // &(*it) ����˼�ǣ����� *it �����õ������õ����������� & ȡ�ö���ĵ�ַ��
        return &(*it);
    }

    // ���û���ҵ������ؿ�ָ��
    return nullptr;
}

/**
 * @brief ��ȡ������ӵ�һ�����ߵ�ֻ������ָ�롣
 */
const CurveData* CurveDataManager::getLastCurve() const
{
    // ���ȼ�� vector �Ƿ�Ϊ��
    if (m_curveRegistry.empty()) {
        // ����ǿյģ�û���κ����ߣ����ؿ�ָ��
        return nullptr;
    }

    // �����Ϊ�գ�ʹ�� .back() ����ֱ�ӻ�ȡ���һ��Ԫ�ص����ã�
    // Ȼ���� & ������ȡ���ַ������ָ�롣
    return &m_curveRegistry.back();
}
std::vector<std::string> CurveDataManager::getAllCurveNames() const
{
    // 1. ����һ���յ��ַ������������ڴ����������
    std::vector<std::string> names;

    // 2. (��ѡ���Ƽ�) Ϊ����Ԥ���ռ䣬����������ܣ��������ڴ����·���
    names.reserve(m_curveRegistry.size());

    // 3. �����ڲ��洢���ߵ� m_curveRegistry ����
    for (const auto& curve : m_curveRegistry) {
        // 4. ��ÿ�����ߵ� name ��Ա��ӵ� names ������
        names.push_back(curve.name);
    }

    // 5. �������õ������б�
    return names;
}

/**
 * @brief [��������] ��ȡ�ļ�·����Сд��չ����
 * @note ����һ����̬�������������ڴ��ļ��ڲ��ɼ���ʹ�á�
 * @param filePath �ļ�·���ַ�����
 * @return �ļ���չ�� (����, ".csv" �� ".txt")��
 */
static std::string getFileExtension(const std::string& filePath) {
    size_t dotPos = filePath.find_last_of('.');
    if (dotPos != std::string::npos) {
        std::string ext = filePath.substr(dotPos);
        // ת��ΪСд�Ա��ڱȽ�
        for (char& c : ext) {
            c = static_cast<char>(std::tolower(c));
        }
        return ext;
    }
    return ""; // û���ҵ���չ��
}

// --- FileManager ���Ա������ʵ�� ---

bool CurveDataManager::loadSpectrum(const std::string& filePath, CurveData& outSpectrum)
{
    // 1. ����һ�������ļ�������
    std::ifstream inFile(filePath);

    // 2. ����ļ��Ƿ�ɹ���
    if (!inFile.is_open()) {
        std::cerr << "���� [FileManager]: �޷����ļ����ж�ȡ: " << filePath << std::endl;
        return false;
    }

    // 3. ����ⲿ�����vector��ȷ������ֻ�������μ��ص�����
    outSpectrum.x_values.clear();
    outSpectrum.y_values.clear();

    std::string line;
    int lineNumber = 0;
    // 4. ���ж�ȡ�ļ�
    while (std::getline(inFile, line)) {
        lineNumber++;

        // �������л�ע���� (������'#'��ͷ����)
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Ԥ�����������滻Ϊ�ո���ͳһ����ָ���
        for (char& c : line) {
            if (c == ',') {
                c = ' ';
            }
        }

        // ʹ���ַ��������������������
        std::stringstream ss(line);
        double x, y;

        // ���Դ�������ȡ����doubleֵ
        if (ss >> x >> y) {
            // ����ɹ��������ݴ��������Spectrum������
            outSpectrum.x_values.push_back(x);
            outSpectrum.y_values.push_back(y);
        }
        else {
            // ����в�Ϊ�յ��޷�������������ֵ���򷢳�����
            std::cerr << "���� [FileManager]: �ļ� " << filePath << " �ĵ� " << lineNumber
                << " �����ݸ�ʽ����ȷ�����������С�" << std::endl;
        }
    }

    // 5. �ر��ļ�
    inFile.close();

    // 6. ����Ƿ�ɹ��������κ�����
    if (outSpectrum.x_values.empty()) {
        std::cerr << "���� [FileManager]: δ�����ļ� " << filePath << " ���ҵ��κ���Ч�������С�" << std::endl;
        return false;
    }

    return true;
}

bool CurveDataManager::saveSpectrum(const std::string& filePath, const CurveData& spectrum)
{
    // 1. ����������ݵ�X���Y������Ƿ�һ��
    if (spectrum.x_values.size() != spectrum.y_values.size()) {
        std::cerr << "���� [FileManager]: �޷����棬��ΪX���Y������ݵ�������ƥ�䡣" << std::endl;
        return false;
    }

    // 2. �����ļ���չ���Զ�ѡ��ָ���
    char delimiter = '\t'; // Ĭ��Ϊ�Ʊ�� (������.txt)
    if (getFileExtension(filePath) == ".csv") {
        delimiter = ','; // �����.csv�ļ�����ʹ�ö���
    }

    // 3. ����һ������ļ�������
    std::ofstream outFile(filePath);

    // 4. ����ļ��Ƿ�ɹ���
    if (!outFile.is_open()) {
        std::cerr << "���� [FileManager]: �޷����ļ�����д��: " << filePath << std::endl;
        return false;
    }

    // 5. ���ø���������ľ��ȣ��Է�������ʧ
    outFile << std::fixed << std::setprecision(8);

    // 6. ����д������
    for (size_t i = 0; i < spectrum.x_values.size(); ++i) {
        outFile << spectrum.x_values[i] << delimiter << spectrum.y_values[i] << '\n';
    }

    // 7. �ر��ļ�
    outFile.close();

    return true;
}
/**
 * @brief ���ļ��е���һ�����߲���ӵ���������
 * @param filePath Ҫ������ļ�������·����
 * @return �������ɹ������� true�����򷵻� false��
 */
bool CurveDataManager::importCurveFromFile(const std::string& filePath)
{
    // 1. ��ȡ�ļ�����Ϊ��������
    std::string curveName;

    // �������һ��·���ָ���������Windows��Unix��
    size_t lastSlash = filePath.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        // ����ҵ���·���ָ���������֮��ʼ�����ļ���
        curveName = filePath.substr(lastSlash + 1);
    }
    else {
        // ���û��·���ָ����������ַ��������ļ���
        curveName = filePath;
    }

    // �Ƴ��ļ���չ��
    size_t lastDot = curveName.find_last_of('.');
    if (lastDot != std::string::npos) {
        curveName = curveName.substr(0, lastDot);
    }

    // 2. ����������Ƿ�Ϊ��
    if (curveName.empty()) {
        std::cerr << "����: �޷����ļ�·������ȡ��Ч���������ơ�" << std::endl;
        return false;
    }

    // 3. �����������������������Ѵ��ڣ��Զ�������ֺ�׺
    std::string finalName = curveName;
    int counter = 1;
    while (isCurveNameExists(finalName)) {
        finalName = curveName + "_" + std::to_string(counter);
        counter++;
    }

    // 4. ����һ���µ� CurveData ����
    CurveData newCurve;
    newCurve.name = finalName;

    // 5. ���� loadSpectrum �����������ļ�����
    if (!loadSpectrum(filePath, newCurve)) {
        std::cerr << "����: �޷����ļ� " << filePath << " �����������ݡ�" << std::endl;
        return false;
    }

    // 6. �����ص�������ӵ���������
    // ���ﲻ��Ҫ�ټ����������Ϊ�����Ѿ��������
    m_curveRegistry.push_back(newCurve);

    // �������������ԭʼ���Ʋ�ͬ��˵����������������ʾ
    if (finalName != curveName) {
        std::cout << "ע��: �������� '" << curveName
            << "' �Ѵ��ڣ����Զ�������Ϊ '" << finalName << "'��" << std::endl;
    }

    std::cout << "�ɹ����ļ� '" << filePath << "' �������� '" << finalName << "'��" << std::endl;
    return true;
}

/**
 * @brief ����������ָ�����Ƶ����ߵ����������ļ���
 * @param curveName Ҫ���������ߵ����ơ�
 * @param filePath �����ļ���Ŀ��·����
 * @return ��������ɹ������� true�����򷵻� false��
 */
bool CurveDataManager::exportCurveToFile(const std::string& curveName, const std::string& filePath) const
{
    // 1. ����ָ�����Ƶ�����
    const CurveData* curvePtr = getCurveByName(curveName);

    // 2. ��������Ƿ����
    if (curvePtr == nullptr) {
        std::cerr << "����: δ�ҵ�����Ϊ '" << curveName << "' �����ߡ�" << std::endl;
        return false;
    }

    // 3. ������������Ƿ���Ч
    if (curvePtr->x_values.empty() || curvePtr->y_values.empty()) {
        std::cerr << "����: ���� '" << curveName << "' ������Ϊ�ա�" << std::endl;
        return false;
    }

    // 4. ������ݵ������Ƿ�һ��
    if (curvePtr->x_values.size() != curvePtr->y_values.size()) {
        std::cerr << "����: ���� '" << curveName << "' ��X���Y�����ݵ�������ƥ�䡣" << std::endl;
        return false;
    }

    // 5. ���� saveSpectrum �������������ݵ��ļ�
    if (!saveSpectrum(filePath, *curvePtr)) {
        std::cerr << "����: �޷������� '" << curveName << "' �������ļ� '" << filePath << "'��" << std::endl;
        return false;
    }

    std::cout << "�ɹ������� '" << curveName << "' �������ļ� '" << filePath << "'��" << std::endl;
    return true;
}