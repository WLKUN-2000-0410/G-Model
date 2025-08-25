#include "ModelFitter.h"
#include <stdexcept>
#include <limits>

// --- 旧方法更新：使用新的 DataPoint 成员名 ---
ConcentrationModel ModelFitter::fitConcentrationModel(
	const std::string& modelName,
	const std::string& gasName,
	double characteristicPeak,
	const std::vector<DataPoint>& calibrationPoints)
{
	if (calibrationPoints.size() < 2) {
		throw std::invalid_argument("Standard linear regression (y=ax+b) requires at least 2 data points.");
	}

	double n = static_cast<double>(calibrationPoints.size());
	double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_x_squared = 0.0;

	for (const auto& point : calibrationPoints) {
		sum_x += point.x_value; // 更新为 x_value
		sum_y += point.y_value; // 更新为 y_value
		sum_xy += point.x_value * point.y_value;
		sum_x_squared += point.x_value * point.x_value;
	}

	double denominator = n * sum_x_squared - sum_x * sum_x;
	if (std::abs(denominator) < std::numeric_limits<double>::epsilon()) {
		throw std::runtime_error("Cannot fit model (y=ax+b): all X values are the same.");
	}

	double slope_a = (n * sum_xy - sum_x * sum_y) / denominator;
	double intercept_b = (sum_y - slope_a * sum_x) / n;

	// 收集参与计算的曲线名称
	std::vector<std::string> participatingCurves;
	for (const auto& point : calibrationPoints) {
		if (!point.curveName.empty()) {
			participatingCurves.push_back(point.curveName);
		}
	}

	return ConcentrationModel(modelName, gasName, characteristicPeak, slope_a, intercept_b,participatingCurves);
}


// --- 新增方法的实现 ---
PartialPressureModel ModelFitter::fitPartialPressureModel(
	const std::string& modelName,
	const std::string& gasName,
	double characteristicPeak, // <-- 新增参数
	const std::vector<DataPoint>& calibrationPoints)
{
	// 强制通过原点的拟合，理论上1个点就够，但多个点更稳定
	if (calibrationPoints.empty()) {
		throw std::invalid_argument("Regression through origin (y=kx) requires at least 1 data point.");
	}

	// 根据公式 k = Σ(xy) / Σ(x?) 计算
	double sum_xy = 0.0;
	double sum_x_squared = 0.0;

	for (const auto& point : calibrationPoints) {
		sum_xy += point.x_value * point.y_value;
		sum_x_squared += point.x_value * point.x_value;
	}

	// 检查分母
	if (std::abs(sum_x_squared) < std::numeric_limits<double>::epsilon()) {
		throw std::runtime_error("Cannot fit model (y=kx): all X values are zero.");
	}

	// 计算斜率 k
	double slope_k = sum_xy / sum_x_squared;

	// 收集参与计算的曲线名称
	std::vector<std::string> participatingCurves;
	for (const auto& point : calibrationPoints) {
		if (!point.curveName.empty()) {
			participatingCurves.push_back(point.curveName);
		}
	}

	return PartialPressureModel(modelName, gasName, characteristicPeak,slope_k,participatingCurves);
}