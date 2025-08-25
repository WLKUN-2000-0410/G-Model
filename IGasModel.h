#ifndef IGASMODEL_H
#define IGASMODEL_H

#include <string>
#include <vector>
// 用于在计算时传递额外参数的上下文结构体
struct CalculationContext {
	// 对于分压法，这里会用到总压；对于浓度法，则可以忽略。
	double totalPressure_Pa = 101325.0; // 默认为一个标准大气压,可根据需要修改
};

/**
* @class IGasModel
* @brief 所有具体模型类的抽象基类接口。
*
* 定义了所有模型都必须提供的通用功能，如计算浓度、获取名称等。
* 使用纯虚函数来强制子类必须实现这些功能。
*/
class IGasModel
{
public:
	virtual ~IGasModel() = default;

	/**
	* @brief 根据仪器信号，计算出气体浓度。
	* @param rawSignal 从仪器采集到的原始信号值 (Y)。
	* @param context 包含额外计算参数（如总压）的上下文。
	* @return 计算出的气体浓度值 (X)。
	*/
	virtual double calculateConcentration(double rawSignalY, const CalculationContext& context) const = 0;

	virtual std::string getModelName() const = 0;
	virtual std::string getGasName() const = 0;
	virtual std::string getModelType() const = 0;
};

#endif // IGASMODEL_H