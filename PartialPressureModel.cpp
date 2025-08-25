#include "PartialPressureModel.h"
#include <stdexcept> // �����׳��쳣
#include <limits>    // ���� std::numeric_limits

PartialPressureModel::PartialPressureModel(const std::string& modelName, const std::string& gasName, double characteristicPeak, double slope, const std::vector<std::string>& participatingCurveNames)
	: m_modelName(modelName),
	m_gasName(gasName),
	m_modelType("��ѹ��"),
	m_characteristicPeak(characteristicPeak),
	m_slope(slope),
	m_participatingCurveNames(participatingCurveNames),
	m_creationTimestamp(std::time(nullptr)) // ��ʼ��ʱ��¼��ǰʱ��
{
	// ���캯���н���У�飺б�� k ����Ϊ0�������޷�����
	if (std::abs(m_slope) < std::numeric_limits<double>::epsilon()) {
		throw std::invalid_argument("Model slope (k) cannot be zero for PartialPressureModel.");
	}
}

double PartialPressureModel::calculateConcentration(double rawSignal, const CalculationContext& context) const
{
	// --- ���� 1: ����ģ�ͷ������ѹ ---
	// ģ��: �ź�(Y) = k * ��ѹ(X)
	// ������: ��ѹ(X) = �ź�(Y) / k
	double partialPressure_Pa = rawSignal / m_slope;

	// --- ���� 2: ���ݷ�ѹ����ѹ�����Ũ��(ppm) ---
	// �����ѹ��Ч�ԣ���ֹ������
	if (std::abs(context.totalPressure_Pa) < std::numeric_limits<double>::epsilon()) {
		// ��ʵ��Ӧ���У����Ը������󷵻�һ����������׳��쳣
		// �������Ƿ��� NAN (Not a Number) ����ʾһ����Ч�ļ�����
		return std::numeric_limits<double>::quiet_NaN();
	}

	// ���㹫ʽ: ppm = (��ѹ / ��ѹ) * 10^6
	double concentration_ppm = (partialPressure_Pa / context.totalPressure_Pa) * 1e6;

	return concentration_ppm;
}

std::string PartialPressureModel::getModelName() const
{
	return m_modelName;
}

std::string PartialPressureModel::getGasName() const
{
	return m_gasName;
}

std::string PartialPressureModel::getModelType() const
{
	// �����������ض���������
	return m_modelType;
}

double PartialPressureModel::getSlope() const
{
	return m_slope;
}

std::time_t PartialPressureModel::getCreationTimestamp() const
{
	return m_creationTimestamp;
}
double PartialPressureModel::getCharacteristicPeak() const // <-- ʵ����������
{
	return m_characteristicPeak;
}
const std::vector<std::string>& PartialPressureModel::getParticipatingCurveNames() const
{
	return m_participatingCurveNames;
}