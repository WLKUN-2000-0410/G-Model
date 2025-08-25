#include <iostream>
#include <stdexcept>
#include <fstream> 
#include <sstream>   // 可用于字符串解析
#include <algorithm> // 用于字符串处理
#include <cctype>
#include <iomanip> // 为了使用 std::put_time
#include "ModelManager.h"
#include "ConcentrationModel.h"         //浓度模型
// #include "PartialPressureModel.h"    //分压模型


ModelManager::ModelManager()
{
}

ModelManager::~ModelManager()
{
}

bool ModelManager::createNewModel(
	ModelType modelType,
	const std::string& modelName,
	const std::string& gasName,
	double characteristicPeak,
	const std::vector<DataPoint>& calibrationPoints)
{
	if (m_modelRegistry.count(modelName) > 0) {
		return false;
	}

	try {
		if (modelType == ModelType::Concentration) {
			ConcentrationModel newModel = ModelFitter::fitConcentrationModel(modelName, gasName, characteristicPeak, calibrationPoints);
			m_modelRegistry[modelName] = std::make_unique<ConcentrationModel>(std::move(newModel));

		}
		else if (modelType == ModelType::PartialPressure) { // <<-- 新增的分支
															// 调用新的拟合方法
			PartialPressureModel newModel = ModelFitter::fitPartialPressureModel(modelName, gasName, characteristicPeak, calibrationPoints);
			// 存入 PartialPressureModel 类型的对象
			m_modelRegistry[modelName] = std::make_unique<PartialPressureModel>(std::move(newModel));

		}
		else {
			return false;
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error creating model '" << modelName << "': " << e.what() << std::endl;
		return false;
	}

	return true;
}


bool ModelManager::renameModel(const std::string& oldName, const std::string& newName)
{
	// 1. 检查新名字是否有效或已被占用
	if (newName.empty() || m_modelRegistry.count(newName) > 0 || oldName == newName) {
		return false;
	}

	// 2. 查找旧名字对应的模型
	auto it = m_modelRegistry.find(oldName);
	if (it == m_modelRegistry.end()) {
		return false; // 旧名称不存在
	}

	// 3. 将智能指针的所有权从 map 转移到一个临时变量中
	//    std::move 会使 map 中的指针变为空，但对象本身还存在于 modelToMove 中
	std::unique_ptr<IGasModel> modelToMove = std::move(it->second);

	// 4. 现在可以安全地从 map 中移除旧的条目
	m_modelRegistry.erase(it);

	// 5. 根据旧模型的类型，创建一个拥有新名字的新模型实例
	//    这里的逻辑是安全的，因为它创建了一个全新的对象
	std::unique_ptr<IGasModel> newModelWithNewName;
	if (modelToMove->getModelType() == "浓度法") {
		const ConcentrationModel* oldConcModel = static_cast<const ConcentrationModel*>(modelToMove.get());
		newModelWithNewName = std::make_unique<ConcentrationModel>(
			newName,
			oldConcModel->getGasName(),
			oldConcModel->getCharacteristicPeak(),
			oldConcModel->getSlope(),
			oldConcModel->getIntercept(),
			oldConcModel->getParticipatingCurveNames()
			);
	}
	// 对分压法的处理 ...
	else if (modelToMove->getModelType() == "分压法") { // <-- 补充对分压法的重命名逻辑
		const PartialPressureModel* oldPPModel = static_cast<const PartialPressureModel*>(modelToMove.get());
		newModelWithNewName = std::make_unique<PartialPressureModel>(
			newName,
			oldPPModel->getGasName(),
			oldPPModel->getCharacteristicPeak(), // <-- 复制特征峰
			oldPPModel->getSlope(),
			oldPPModel->getParticipatingCurveNames()
		);
	}
	else {
		// 如果遇到未知类型，无法处理，重命名失败
		return false;
	}

	// 6. 将这个全新的模型实例用新的名字作为键，插入到 map 中
	m_modelRegistry[newName] = std::move(newModelWithNewName);

	return true;
}
// ================================================================

bool ModelManager::deleteModel(const std::string& modelName)
{
	return m_modelRegistry.erase(modelName) > 0;
}

const IGasModel* ModelManager::getModel(const std::string& modelName) const
{
	auto it = m_modelRegistry.find(modelName);
	if (it != m_modelRegistry.end()) {
		return it->second.get();
	}
	return nullptr;
}

std::vector<std::string> ModelManager::getAllModelNames() const
{
	std::vector<std::string> names;
	names.reserve(m_modelRegistry.size());
	for (const auto& pair : m_modelRegistry) {
		names.push_back(pair.first);
	}
	return names;
}

void trim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !isspace(ch);
	}));
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !isspace(ch);
	}).base(), s.end());
}
bool ModelManager::saveModelsToFile(const std::string& filePath) const
{
	std::ofstream outFile(filePath);
	if (!outFile.is_open()) {
		std::cerr << "Error: Could not open file for writing: " << filePath << std::endl;
		return false;
	}

	for (const auto& pair : m_modelRegistry) {
		const auto& modelPtr = pair.second;
		outFile << "[" << modelPtr->getModelName() << "]" << std::endl;
		outFile << "GasName=" << modelPtr->getGasName() << std::endl;
		outFile << "ModelType=" << modelPtr->getModelType() << std::endl;

		if (modelPtr->getModelType() == "浓度法") {
			const ConcentrationModel* concModel = dynamic_cast<const ConcentrationModel*>(modelPtr.get());
			if (concModel) {
				// 将 time_t 转换为 tm 结构体以进行格式化
				std::time_t ts = concModel->getCreationTimestamp();
				std::tm tm_struct;
				localtime_s(&tm_struct, &ts); // C++11 安全版本

											  // 使用 std::put_time 进行格式化输出
				outFile << "CharacteristicPeak=" << concModel->getCharacteristicPeak() << std::endl;
				outFile << "Slope=" << concModel->getSlope() << std::endl;
				outFile << "Intercept=" << concModel->getIntercept() << std::endl;
				outFile << "Timestamp=" << std::put_time(&tm_struct, "%Y-%m-%d %H:%M:%S") << std::endl;
				// 新增：保存参与计算的曲线名称
				const auto& curveNames = concModel->getParticipatingCurveNames();
				if (!curveNames.empty()) {
					outFile << "ParticipatingCurves=";
					for (size_t i = 0; i < curveNames.size(); ++i) {
						if (i > 0) outFile << ";";  // 用分号分隔多个曲线名
						outFile << curveNames[i];
					}
					outFile << std::endl;
				}
			}
		}
		else if (modelPtr->getModelType() == "分压法") { // <<-- 新增的分支
			const PartialPressureModel* ppModel = dynamic_cast<const PartialPressureModel*>(modelPtr.get());
			if (ppModel) {
				std::time_t ts = ppModel->getCreationTimestamp();
				std::tm tm_struct;
				localtime_s(&tm_struct, &ts);
				outFile << "CharacteristicPeak=" << ppModel->getCharacteristicPeak() << std::endl;
				outFile << "Slope=" << ppModel->getSlope() << std::endl;
				// 注意：分压法模型没有截距，所以我们不写入 Intercept
				outFile << "Timestamp=" << std::put_time(&tm_struct, "%Y-%m-%d %H:%M:%S") << std::endl;
				// 新增：保存参与计算的曲线名称
				const auto& curveNames = ppModel->getParticipatingCurveNames();
				if (!curveNames.empty()) {
					outFile << "ParticipatingCurves=";
					for (size_t i = 0; i < curveNames.size(); ++i) {
						if (i > 0) outFile << ";";  // 用分号分隔多个曲线名
						outFile << curveNames[i];
					}
					outFile << std::endl;
				}
			}
		}
		outFile << std::endl;
	}
	outFile.close();
	std::cout << "Successfully saved " << m_modelRegistry.size() << " models to " << filePath << std::endl;
	return true;
}


bool ModelManager::loadModelsFromFile(const std::string& filePath)
{
	std::ifstream inFile(filePath);
	if (!inFile.is_open()) {
		std::cerr << "Error: Could not open file for reading: " << filePath << std::endl;
		return false;
	}
	m_modelRegistry.clear();
	std::string line;
	std::string currentModelName, currentGasName, currentModelType;
	std::vector<std::string> currentParticipatingCurves;
	double currentCharacteristicPeak = 0.0, currentSlope = 0.0, currentIntercept = 0.0;
	// time_t currentTimestamp = 0; // 暂时不处理时间戳的恢复
	bool is_reading_model = false;

	auto finalizeCurrentModel = [&]() {
		if (is_reading_model && !currentModelName.empty()) {
			try {
				// --- 核心修改：使用 if-else if 结构处理不同类型的模型 ---

				if (currentModelType == "浓度法") {
					auto model = std::make_unique<ConcentrationModel>(
						currentModelName, currentGasName, currentCharacteristicPeak, currentSlope, currentIntercept, currentParticipatingCurves
						);
					m_modelRegistry[currentModelName] = std::move(model);
				}
				else if (currentModelType == "分压法") { // <<-- 新增的分支
													  // 注意：分压法模型不需要 currentIntercept 参数
					auto model = std::make_unique<PartialPressureModel>(
						currentModelName, currentGasName, currentCharacteristicPeak, currentSlope, currentParticipatingCurves
						);
					m_modelRegistry[currentModelName] = std::move(model);
				}

			}
			catch (const std::exception& e) {
				std::cerr << "Error creating model '" << currentModelName << "': " << e.what() << std::endl;
			}
		}
		is_reading_model = false;
		currentModelName.clear(); // 重置
	};

	while (std::getline(inFile, line)) {
		trim(line);
		if (line.empty()) continue;

		if (line.front() == '[' && line.back() == ']') {
			finalizeCurrentModel();
			currentModelName = line.substr(1, line.length() - 2);
			trim(currentModelName);
			is_reading_model = true;
			continue;
		}

		auto delimiterPos = line.find('=');
		if (delimiterPos != std::string::npos) {
			std::string key = line.substr(0, delimiterPos);
			std::string value = line.substr(delimiterPos + 1);
			trim(key);
			trim(value);
			try {
				if (key == "GasName") currentGasName = value;
				else if (key == "ModelType") currentModelType = value;
				else if (key == "CharacteristicPeak") currentCharacteristicPeak = std::stod(value);
				else if (key == "Slope") currentSlope = std::stod(value);
				else if (key == "Intercept") currentIntercept = std::stod(value); // 即使分压法没有截距，也安全地读取并忽略
				else if (key == "ParticipatingCurves") {
					// 解析分号分隔的曲线名称
					currentParticipatingCurves.clear();
					std::stringstream ss(value);
					std::string curveName;
					while (std::getline(ss, curveName, ';')) {
						trim(curveName);
						if (!curveName.empty()) {
							currentParticipatingCurves.push_back(curveName);
						}
					}
				}
			}
			catch (const std::exception& e) {
				std::cerr << "Error parsing value for key '" << key << "': " << value << std::endl;
			}
		}
	}
	// 确保文件中的最后一个模型也被处理
	finalizeCurrentModel();
	currentParticipatingCurves.clear();
	inFile.close();
	std::cout << "Successfully loaded " << m_modelRegistry.size() << " models from " << filePath << std::endl;
	return true;
}