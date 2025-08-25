#include "ConcentrationModel.h"
#include <limits> // ���� std::numeric_limits
#include <stdexcept> // �����׳��쳣

ConcentrationModel::ConcentrationModel(const std::string& modelName, 
									   const std::string& gasName, 
									   double characteristicPeak, 
									   double slope, 
									   double intercept,
									   const std::vector<std::string>& participatingCurveNames)
	: m_modelName(modelName),
	m_gasName(gasName),
	m_modelType("Ũ�ȷ�"),
	m_characteristicPeak(characteristicPeak),
	m_slope(slope),
	m_intercept(intercept),
	m_participatingCurveNames(participatingCurveNames),
	m_creationTimestamp(std::time(nullptr)) // ��ʼ��ʱ��¼��ǰʱ��
{
	// ���캯�������һЩУ���߼�
	if (std::abs(m_slope) < std::numeric_limits<double>::epsilon()) {
		// ���б�ʽӽ���0�����ģ������Ч�ģ���Ϊ���޷����з�����㡣
		throw std::invalid_argument("Model slope cannot be zero.");
	}
}

double ConcentrationModel::calculateConcentration(double rawSignal, const CalculationContext& context) const
{
	// ���ڡ�Ũ�ȷ��������Ǻ��� context ����
	// ִ�к��ĵ�������: Ũ��(X) = (�ź�(Y) - b) / a
	return (rawSignal - m_intercept) / m_slope;
}

std::string ConcentrationModel::getModelName() const
{
	return m_modelName;
}

std::string ConcentrationModel::getGasName() const
{
	return m_gasName;
}

std::string ConcentrationModel::getModelType() const
{
	return m_modelType; // <-- �޸�Ϊ���س�Ա����
}

double ConcentrationModel::getSlope() const
{
	return m_slope;
}

double ConcentrationModel::getIntercept() const
{
	return m_intercept;
}

std::time_t ConcentrationModel::getCreationTimestamp() const
{
	return m_creationTimestamp;
}
double ConcentrationModel::getCharacteristicPeak() const // <-- ʵ����������
{
	return m_characteristicPeak;
}
const std::vector<std::string>& ConcentrationModel::getParticipatingCurveNames() const
{
	return m_participatingCurveNames;
}