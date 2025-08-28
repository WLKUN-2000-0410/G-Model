#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <string>
#include <vector>
#include <map>
#include <memory>		 // 用于智能指针 std::unique_ptr

#include "IGasModel.h"   // 引入模型基类接口
#include "ModelFitter.h" // 引入数据点结构体 DataPoint

// 定义一个枚举来清晰地表示模型类型
enum class ModelType {
	Concentration,
	PartialPressure
};

/**
* @class ModelManager
* @brief 负责管理所有气体校准模型的生命周期。
*
* 这是一个高级管理类，封装了模型的创建、存储、加载、
* 获取和重命名等所有管理功能。它是建模模块的主要对外接口。
*/
class ModelManager
{
public:
	ModelManager();
	~ModelManager();

	/**
	* @brief 根据校准数据创建一个新模型，并将其添加到管理器中。
	* @param modelType 要创建的模型类型（例如 ModelType::Concentration）。
	* @param modelName 新模型的唯一名称。
	* @param gasName 模型对应的气体类型。
	* @param calibrationPoints 用于拟合的校准数据点。
	* @return 如果成功创建并添加则返回 true，如果模型名称已存在则返回 false。
	*/
	bool createNewModel(
		ModelType modelType,
		const std::string& modelName,
		const std::string& gasName,
		double characteristicPeak,
		const std::vector<DataPoint>& calibrationPoints
	);

	/**
	* @brief 重命名一个已存在的模型。
	* @param oldName 要重命名的模型的当前名称。
	* @param newName 模型的新名称。
	* @return 如果成功返回 true，否则返回 false。
	*/
	bool renameModel(const std::string& oldName, const std::string& newName);

	/**
	* @brief 删除一个模型。
	* @param modelName 要删除的模型的名称。
	* @return 如果成功删除返回 true，如果模型不存在则返回 false。
	*/
	bool deleteModel(const std::string& modelName);

	/**
	* @brief 根据模型名称获取一个只读的模型指针。
	* @param modelName 模型的名称。
	* @return 返回一个指向IGasModel的常量指针，如果不存在则返回 nullptr。
	*/
	const IGasModel* getModel(const std::string& modelName) const;

	/**
	* @brief 获取所有模型的名称列表。
	* @return 一个包含所有模型名称的向量。
	*/
	std::vector<std::string> getAllModelNames() const;

	/**
	* @brief 从文件中加载所有模型。
	* @param filePath 文件路径。
	* @return 如果加载成功返回 true。
	*/
	bool loadModelsFromFile(const std::string& filePath);

	/**
	* @brief 将当前所有模型保存到文件。
	* @param filePath 文件路径。
	* @return 如果保存成功返回 true。
	*/
	bool saveModelsToFile(const std::string& filePath) const;

	std::vector<std::string> getModelNamesByType(const std::string& modelType) const;


private:
	// 使用 std::map 来存储模型。
	// 键是模型的唯一名称 (std::string)。
	// 值是模型的智能指针 (std::unique_ptr)，它能自动管理内存。
	std::map<std::string, std::unique_ptr<IGasModel>> m_modelRegistry;
};

#endif // MODELMANAGER_H
