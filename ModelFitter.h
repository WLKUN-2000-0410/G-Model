#ifndef MODELFITTER_H
#define MODELFITTER_H

#include "ConcentrationModel.h" // 需要包含它，因为我们要创建这个类型的对象
#include "PartialPressureModel.h"
#include <vector>               // 用于接收一个数据点列表

/**
* @struct DataPoint
* @brief 用于表示单个校准数据点(X,Y)的结构体。
*
*/
struct DataPoint {
	std::string curveName; // 曲线名称 
	double x_value; // 自变量 X (可以是浓度 或 分压)
	double y_value; // 因变量 Y (仪器信号)

	// 构造函数
	DataPoint(double x, double y, const std::string& curve)
		: x_value(x), y_value(y), curveName(curve) {
	}
	// 兼容原有使用方式
	DataPoint(double x, double y)
		: x_value(x), y_value(y), curveName("") {
	}
};

/**
* @class ModelFitter
* @brief 一个静态工具类，用于根据校准数据创建模型。
*
* 这个类封装了核心的数学拟合算法（例如：最小二乘法），-- ai
* 实现了文档中提到的“自动拟合曲线功能”。
*/
class ModelFitter
{
public:
	/**
	* @brief 对一组校准数据进行线性拟合，并创建一个ConcentrationModel对象。
	* @param modelName 要创建的模型的唯一名称。
	* @param gasName 模型对应的气体类型。
	* @param calibrationPoints 一个包含多个校准数据点的向量。
	* @return 一个包含拟合结果 (斜率a, 截距b) 的 ConcentrationModel 对象。
	* @throw std::invalid_argument 如果数据点少于2个，无法进行线性拟合。
	*/
	static ConcentrationModel fitConcentrationModel(
		const std::string& modelName,
		const std::string& gasName,
		double characteristicPeak,
		const std::vector<DataPoint>& calibrationPoints
	);

	/**
	* @brief 新增：为“分压法”执行强制通过原点的线性回归 (y=kx)。
	*/
	static PartialPressureModel fitPartialPressureModel(
		const std::string& modelName,
		const std::string& gasName,
		double characteristicPeak,
		const std::vector<DataPoint>& calibrationPoints
	);

private:
	// 将构造函数设为私有，防止外部创建 ModelFitter 的实例。
	// 这强调了它只是一个静态功能的集合。
	ModelFitter() = default;
};

#endif // MODELFITTER_H
