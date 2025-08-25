#include "PartialPressureModel.h"
#include <stdexcept> // 用于抛出异常
#include <limits>    // 用于 std::numeric_limits

PartialPressureModel::PartialPressureModel(const std::string& modelName, const std::string& gasName, double characteristicPeak, double slope, const std::vector<std::string>& participatingCurveNames)
	: m_modelName(modelName),
	m_gasName(gasName),
	m_modelType("分压法"),
	m_characteristicPeak(characteristicPeak),
	m_slope(slope),
	m_participatingCurveNames(participatingCurveNames),
	m_creationTimestamp(std::time(nullptr)) // 初始化时记录当前时间
{
	// 构造函数中进行校验：斜率 k 不能为0，否则无法计算
	if (std::abs(m_slope) < std::numeric_limits<double>::epsilon()) {
		throw std::invalid_argument("Model slope (k) cannot be zero for PartialPressureModel.");
	}
}

double PartialPressureModel::calculateConcentration(double rawSignal, const CalculationContext& context) const
{
	// --- 步骤 1: 根据模型反算出分压 ---
	// 模型: 信号(Y) = k * 分压(X)
	// 逆运算: 分压(X) = 信号(Y) / k
	double partialPressure_Pa = rawSignal / m_slope;

	// --- 步骤 2: 根据分压和总压换算出浓度(ppm) ---
	// 检查总压有效性，防止除以零
	if (std::abs(context.totalPressure_Pa) < std::numeric_limits<double>::epsilon()) {
		// 在实际应用中，可以根据需求返回一个错误码或抛出异常
		// 这里我们返回 NAN (Not a Number) 来表示一个无效的计算结果
		return std::numeric_limits<double>::quiet_NaN();
	}

	// 换算公式: ppm = (分压 / 总压) * 10^6
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
	// 返回这个类的特定类型名称
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
double PartialPressureModel::getCharacteristicPeak() const // <-- 实现新增方法
{
	return m_characteristicPeak;
}
const std::vector<std::string>& PartialPressureModel::getParticipatingCurveNames() const
{
	return m_participatingCurveNames;
}