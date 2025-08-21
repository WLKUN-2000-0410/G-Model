#include <iostream>
#include <stdexcept>
#include <fstream> 
#include <sstream>   // �������ַ�������
#include <algorithm> // �����ַ�������
#include <cctype>
#include <iomanip> // Ϊ��ʹ�� std::put_time
#include "ModelManager.h"
#include "ConcentrationModel.h"         //Ũ��ģ��
// #include "PartialPressureModel.h"    //��ѹģ��


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
	const std::vector<DataPoint>& calibrationPoints)
{
	if (m_modelRegistry.count(modelName) > 0) {
		return false;
	}

	try {
		if (modelType == ModelType::Concentration) {
			ConcentrationModel newModel = ModelFitter::fitConcentrationModel(modelName, gasName, calibrationPoints);
			m_modelRegistry[modelName] = std::make_unique<ConcentrationModel>(std::move(newModel));

		}
		else if (modelType == ModelType::PartialPressure) { // <<-- �����ķ�֧
															// �����µ���Ϸ���
			PartialPressureModel newModel = ModelFitter::fitPartialPressureModel(modelName, gasName, calibrationPoints);
			// ���� PartialPressureModel ���͵Ķ���
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
	// 1. ����������Ƿ���Ч���ѱ�ռ��
	if (newName.empty() || m_modelRegistry.count(newName) > 0 || oldName == newName) {
		return false;
	}

	// 2. ���Ҿ����ֶ�Ӧ��ģ��
	auto it = m_modelRegistry.find(oldName);
	if (it == m_modelRegistry.end()) {
		return false; // �����Ʋ�����
	}

	// 3. ������ָ�������Ȩ�� map ת�Ƶ�һ����ʱ������
	//    std::move ��ʹ map �е�ָ���Ϊ�գ��������������� modelToMove ��
	std::unique_ptr<IGasModel> modelToMove = std::move(it->second);

	// 4. ���ڿ��԰�ȫ�ش� map ���Ƴ��ɵ���Ŀ
	m_modelRegistry.erase(it);

	// 5. ���ݾ�ģ�͵����ͣ�����һ��ӵ�������ֵ���ģ��ʵ��
	//    ������߼��ǰ�ȫ�ģ���Ϊ��������һ��ȫ�µĶ���
	std::unique_ptr<IGasModel> newModelWithNewName;
	if (modelToMove->getModelType() == "Ũ�ȷ�") {
		const ConcentrationModel* oldConcModel = static_cast<const ConcentrationModel*>(modelToMove.get());
		newModelWithNewName = std::make_unique<ConcentrationModel>(
			newName,
			oldConcModel->getGasName(),
			oldConcModel->getSlope(),
			oldConcModel->getIntercept()
			);
	}
	// ... δ����������ӶԷ�ѹ���Ĵ��� ...
	else {
		// �������δ֪���ͣ��޷�����������ʧ��
		return false;
	}

	// 6. �����ȫ�µ�ģ��ʵ�����µ�������Ϊ�������뵽 map ��
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

		if (modelPtr->getModelType() == "Ũ�ȷ�") {
			const ConcentrationModel* concModel = dynamic_cast<const ConcentrationModel*>(modelPtr.get());
			if (concModel) {
				// �� time_t ת��Ϊ tm �ṹ���Խ��и�ʽ��
				std::time_t ts = concModel->getCreationTimestamp();
				std::tm tm_struct;
				localtime_s(&tm_struct, &ts); // C++11 ��ȫ�汾

											  // ʹ�� std::put_time ���и�ʽ�����
				outFile << "Slope=" << concModel->getSlope() << std::endl;
				outFile << "Intercept=" << concModel->getIntercept() << std::endl;
				outFile << "Timestamp=" << std::put_time(&tm_struct, "%Y-%m-%d %H:%M:%S") << std::endl;
			}
		}
		else if (modelPtr->getModelType() == "��ѹ��") { // <<-- �����ķ�֧
			const PartialPressureModel* ppModel = dynamic_cast<const PartialPressureModel*>(modelPtr.get());
			if (ppModel) {
				std::time_t ts = ppModel->getCreationTimestamp();
				std::tm tm_struct;
				localtime_s(&tm_struct, &ts);

				outFile << "Slope=" << ppModel->getSlope() << std::endl;
				// ע�⣺��ѹ��ģ��û�нؾ࣬�������ǲ�д�� Intercept
				outFile << "Timestamp=" << std::put_time(&tm_struct, "%Y-%m-%d %H:%M:%S") << std::endl;
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
	double currentSlope = 0.0, currentIntercept = 0.0;
	// time_t currentTimestamp = 0; // ��ʱ������ʱ����Ļָ�
	bool is_reading_model = false;

	auto finalizeCurrentModel = [&]() {
		if (is_reading_model && !currentModelName.empty()) {
			try {
				// --- �����޸ģ�ʹ�� if-else if �ṹ����ͬ���͵�ģ�� ---

				if (currentModelType == "Ũ�ȷ�") {
					auto model = std::make_unique<ConcentrationModel>(
						currentModelName, currentGasName, currentSlope, currentIntercept
						);
					m_modelRegistry[currentModelName] = std::move(model);
				}
				else if (currentModelType == "��ѹ��") { // <<-- �����ķ�֧
													  // ע�⣺��ѹ��ģ�Ͳ���Ҫ currentIntercept ����
					auto model = std::make_unique<PartialPressureModel>(
						currentModelName, currentGasName, currentSlope
						);
					m_modelRegistry[currentModelName] = std::move(model);
				}

			}
			catch (const std::exception& e) {
				std::cerr << "Error creating model '" << currentModelName << "': " << e.what() << std::endl;
			}
		}
		is_reading_model = false;
		currentModelName.clear(); // ����
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
				else if (key == "Slope") currentSlope = std::stod(value);
				else if (key == "Intercept") currentIntercept = std::stod(value); // ��ʹ��ѹ��û�нؾ࣬Ҳ��ȫ�ض�ȡ������
																				  // else if (key == "Timestamp") { /* �����߼����Լ������� */ }
			}
			catch (const std::exception& e) {
				std::cerr << "Error parsing value for key '" << key << "': " << value << std::endl;
			}
		}
	}
	// ȷ���ļ��е����һ��ģ��Ҳ������
	finalizeCurrentModel();

	inFile.close();
	std::cout << "Successfully loaded " << m_modelRegistry.size() << " models from " << filePath << std::endl;
	return true;
}