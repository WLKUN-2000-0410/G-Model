// CurveDataManager.h

#ifndef CURVEDATAMANAGER_H
#define CURVEDATAMANAGER_H

#include <string>
#include <vector>
// #include <map> // ������Ҫ map

/**
 * @struct CurveData
 * @brief ���ڴ洢�����������ݵĽṹ�塣
 */
struct CurveData
{
    std::string name;                 // ���ߵ�Ψһ����
    std::vector<double> x_values;     // X�����ݵ�
    std::vector<double> y_values;     // Y�����ݵ�
};

/**
 * @class CurveDataManager
 * @brief (����) �������ǰ�Ự��������ʱ���ߵ��������ڡ�
 *
 * (����C) �ڲ�ʹ�� std::vector �洢����֤�����ߵ����˳��
 */
class CurveDataManager
{
public:
    /**
     * @brief ��ȡ CurveDataManager ��ȫ��Ψһʵ����
     */
    static CurveDataManager& GetInstance();

    // --- ���Ĺ����� ---

    /**
     * @brief ���һ�������ߵ��������С�
     * �ڲ������������������������Ѵ��ڣ������ʧ�ܡ�
     * @param newCurve Ҫ��ӵ��������ݡ�
     * @return ����������Ʋ��ظ�����ӳɹ������� true�����򷵻� false��
     */
    bool addCurve(const CurveData& newCurve);

    /**
     * @brief ��������ɾ��һ�����ߡ�
     * @param curveName Ҫɾ�������ߵ����ơ�
     * @return ����ҵ����ɹ�ɾ�������� true�����򷵻� false��
     */
    bool deleteCurve(const std::string& curveName);
    
    /**
     * @brief ����������Ƿ��Ѿ����ڡ�
     * @param curveName Ҫ������������
	 * @return ����Ѿ����ڣ����� true�����򷵻� false��
     */
    bool isCurveNameExists(const std::string& curveName) const;

    /**
     * @brief ������һ���Ѵ��ڵ����ߡ�
     * @param oldName ���ߵĵ�ǰ���ơ�
     * @param newName ���ߵ������ơ�
     * @return ����������ɹ������� true����������Ʋ����ڻ��������Ѵ��ڣ����� false��
     */
    bool renameCurve(const std::string& oldName, const std::string& newName);

    // --- ���ݲ�ѯ���� ---

    /**
     * @brief �������ƻ�ȡһ�����ߵ�ֻ������ָ�롣
     * @param curveName Ҫ���ҵ����ߵ����ơ�
     * @return ����ҵ�������ָ����������ݵ� const ָ�룻���򷵻� nullptr��
     */
    const CurveData* getCurveByName(const std::string& curveName) const;

    /**
     * @brief ��ȡ������ӵ�һ�����ߵ�ֻ������ָ�롣
     * @return ����������������ߣ�����ָ�����һ�����ߵ� const ָ�룻���򷵻� nullptr��
     */
    const CurveData* getLastCurve() const;

    /**
     * @brief ��ȡ��ǰ���������������ߵ������б�
     * @return һ�����������������Ƶ��ַ��������������˳���г�����
     */
    std::vector<std::string> getAllCurveNames() const;

    // --- �ļ��������� ---

    // �������ӵ����ļ�����һ������
    bool importCurveFromFile(const std::string& filePath);

    // ��������ָ�����Ƶ����ߵ����������ļ�
    bool exportCurveToFile(const std::string& curveName, const std::string& filePath) const;

private:
    /**
     * @brief ��һ��˫���ı��ļ��м���һ���������ߡ�
     * @details �ļ�ÿ��Ӧ����������ֵ(X��Y)�����ÿո��Ʊ���򶺺ŷָ���
     * @param filePath Ҫ���ص��ļ�·�� (e.g., "my_spectrum.csv")��
     * @param outSpectrum [���] ���ڽ��ռ��صĹ������ݡ�
     * @return ������سɹ������� true��
     */
    static bool loadSpectrum(const std::string& filePath,
        CurveData& outSpectrum);

    /**
     * @brief ��һ�����׵�XY���ݱ������ļ���
     * @details ������ļ���չ���Զ�ѡ��ָ��� (���� for .csv, �Ʊ�� for .txt)��
     * @param filePath Ҫ������ļ�·�� (e.g., "result.txt")��
     * @param spectrum Ҫ����Ĺ������ݡ�
     * @return �������ɹ������� true��
     */
    static bool saveSpectrum(const std::string& filePath,
        const CurveData& spectrum);

    
private:
    // --- ����ģʽʵ�� ---
    CurveDataManager();
    ~CurveDataManager();
    CurveDataManager(const CurveDataManager&) = delete;
    CurveDataManager& operator=(const CurveDataManager&) = delete;

    // --- �ڲ����ݴ洢 ---
    // ʹ�� std::vector �洢���ߣ����Ա�֤Ԫ�ص�˳������ӵ�˳��һ�¡�
    std::vector<CurveData> m_curveRegistry;
};

#endif // CURVEDATAMANAGER_H