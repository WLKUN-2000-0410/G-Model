#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <string>
#include <vector>
#include <map>
#include <memory>		 // ��������ָ�� std::unique_ptr

#include "IGasModel.h"   // ����ģ�ͻ���ӿ�
#include "ModelFitter.h" // �������ݵ�ṹ�� DataPoint

// ����һ��ö���������ر�ʾģ������
enum class ModelType {
	Concentration,
	PartialPressure
};

/**
* @class ModelManager
* @brief ���������������У׼ģ�͵��������ڡ�
*
* ����һ���߼������࣬��װ��ģ�͵Ĵ������洢�����ء�
* ��ȡ�������������й����ܡ����ǽ�ģģ�����Ҫ����ӿڡ�
*/
class ModelManager
{
public:
	ModelManager();
	~ModelManager();

	/**
	* @brief ����У׼���ݴ���һ����ģ�ͣ���������ӵ��������С�
	* @param modelType Ҫ������ģ�����ͣ����� ModelType::Concentration����
	* @param modelName ��ģ�͵�Ψһ���ơ�
	* @param gasName ģ�Ͷ�Ӧ���������͡�
	* @param calibrationPoints ������ϵ�У׼���ݵ㡣
	* @return ����ɹ�����������򷵻� true�����ģ�������Ѵ����򷵻� false��
	*/
	bool createNewModel(
		ModelType modelType,
		const std::string& modelName,
		const std::string& gasName,
		double characteristicPeak,
		const std::vector<DataPoint>& calibrationPoints
	);

	/**
	* @brief ������һ���Ѵ��ڵ�ģ�͡�
	* @param oldName Ҫ��������ģ�͵ĵ�ǰ���ơ�
	* @param newName ģ�͵������ơ�
	* @return ����ɹ����� true�����򷵻� false��
	*/
	bool renameModel(const std::string& oldName, const std::string& newName);

	/**
	* @brief ɾ��һ��ģ�͡�
	* @param modelName Ҫɾ����ģ�͵����ơ�
	* @return ����ɹ�ɾ������ true�����ģ�Ͳ������򷵻� false��
	*/
	bool deleteModel(const std::string& modelName);

	/**
	* @brief ����ģ�����ƻ�ȡһ��ֻ����ģ��ָ�롣
	* @param modelName ģ�͵����ơ�
	* @return ����һ��ָ��IGasModel�ĳ���ָ�룬����������򷵻� nullptr��
	*/
	const IGasModel* getModel(const std::string& modelName) const;

	/**
	* @brief ��ȡ����ģ�͵������б�
	* @return һ����������ģ�����Ƶ�������
	*/
	std::vector<std::string> getAllModelNames() const;

	/**
	* @brief ���ļ��м�������ģ�͡�
	* @param filePath �ļ�·����
	* @return ������سɹ����� true��
	*/
	bool loadModelsFromFile(const std::string& filePath);

	/**
	* @brief ����ǰ����ģ�ͱ��浽�ļ���
	* @param filePath �ļ�·����
	* @return �������ɹ����� true��
	*/
	bool saveModelsToFile(const std::string& filePath) const;

	std::vector<std::string> getModelNamesByType(const std::string& modelType) const;


private:
	// ʹ�� std::map ���洢ģ�͡�
	// ����ģ�͵�Ψһ���� (std::string)��
	// ֵ��ģ�͵�����ָ�� (std::unique_ptr)�������Զ������ڴ档
	std::map<std::string, std::unique_ptr<IGasModel>> m_modelRegistry;
};

#endif // MODELMANAGER_H
