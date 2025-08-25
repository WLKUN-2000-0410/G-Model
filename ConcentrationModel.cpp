#include "ConcentrationModel.h"
#include <limits> // 用于 std::numeric_limits
#include <stdexcept> // 用于抛出异常

ConcentrationModel::ConcentrationModel(const std::string& modelName, 
									   const std::string& gasName, 
									   double characteristicPeak, 
									   double slope, 
									   double intercept,
									   const std::vector<std::string>& participatingCurveNames)
	: m_modelName(modelName),
	m_gasName(gasName),
	m_modelType("浓度法"),
	m_characteristicPeak(characteristicPeak),
	m_slope(slope),
	m_intercept(intercept),
	m_participatingCurveNames(participatingCurveNames),
	m_creationTimestamp(std::time(nullptr)) // 初始化时记录当前时间
{
	// 构造函数中添加一些校验逻辑
	if (std::abs(m_slope) < std::numeric_limits<double>::epsilon()) {
		// 如果斜率接近于0，这个模型是无效的，因为它无法进行反向计算。
		throw std::invalid_argument("Model slope cannot be zero.");
	}
}

double ConcentrationModel::calculateConcentration(double rawSignal, const CalculationContext& context) const
{
	// 对于“浓度法”，我们忽略 context 参数
	// 执行核心的逆运算: 浓度(X) = (信号(Y) - b) / a
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
	return m_modelType; // <-- 修改为返回成员变量
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
double ConcentrationModel::getCharacteristicPeak() const // <-- 实现新增方法
{
	return m_characteristicPeak;
}
const std::vector<std::string>& ConcentrationModel::getParticipatingCurveNames() const
{
	return m_participatingCurveNames;
}