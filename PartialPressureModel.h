#ifndef PARTIALPRESSUREMODEL_H
#define PARTIALPRESSUREMODEL_H

#include "IGasModel.h" 
#include <ctime>

/**
* @class PartialPressureModel
* @brief "分压法"的具体实现，没有截距。
*
* 模型方程为: 信号(Y) = k * 分压(X)
* 其中 k 为斜率。
*/
class PartialPressureModel : public IGasModel
{
public:
	/**
	* @brief PartialPressureModel 的构造函数。
	* @param modelName 模型的唯一标识名称。
	* @param gasName 模型对应的气体类型。
	* @param slope 模型的斜率 'k'。
	*/
	PartialPressureModel(const std::string& modelName, 
						 const std::string& gasName,
						 double characteristicPeak,
						 double slope,
						 const std::vector<std::string>& participatingCurveNames = {});

	// --- 重写基类的纯虚函数 ---

	/**
	* @brief 根据仪器信号，计算出气体浓度(ppm)。
	*
	* 该方法分两步执行：
	* 1. 根据模型反算出分压：分压(X) = 信号(Y) / k
	* 2. 根据分压和总压换算出浓度：ppm = (分压 / 总压) * 10^6
	* @param rawSignal 从仪器采集到的原始信号值 (Y)。
	* @param context 包含总压等额外计算参数的上下文。
	* @return 最终计算出的气体浓度值 (ppm)。
	*/
	double calculateConcentration(double rawSignal, const CalculationContext& context) const override;

	std::string getModelName() const override;
	std::string getGasName() const override;
	std::string getModelType() const override;

	// --- PartialPressureModel 特有的方法 ---

	/**
	* @brief 获取模型的斜率 (参数 'k')。
	* @return 斜率值。
	*/
	double getSlope() const;
	double getCharacteristicPeak() const; // <-- 新增方法
	std::time_t getCreationTimestamp() const;
	const std::vector<std::string>& getParticipatingCurveNames() const;  //获取参与计算的曲线名称列表

private:
	std::string m_modelName;
	std::string m_gasName;
	std::string m_modelType;
	double      m_slope; // 斜率 (k)
						 // 注意：这里没有 m_intercept 成员变量
	double      m_characteristicPeak; // 特征峰位置
	std::vector<std::string> m_participatingCurveNames;
	std::time_t m_creationTimestamp;
};

#endif // PARTIALPRESSUREMODEL_H