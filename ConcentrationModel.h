#ifndef CONCENTRATIONMODEL_H
#define CONCENTRATIONMODEL_H

#include "IGasModel.h" 
#include <ctime>

/**
* @class ConcentrationModel
* @brief "浓度法"的具体实现。
*
* 模型方程为: 信号(Y) = k * 浓度(X) + b
*/
class ConcentrationModel : public IGasModel
{
public:
	/**
	* @brief ConcentrationModel 的构造函数。
	* @param modelName 模型的唯一标识名称。
	* @param gasName 模型对应的气体类型。
	* @param slope 模型的斜率 'a'。
	* @param intercept 模型的截距 'b'。
	*/
	ConcentrationModel(const std::string& modelName, 
					   const std::string& gasName, 
					   double characteristicPeak, 
					   double slope,
					   double intercept,
					   const std::vector<std::string>& participatingCurveNames = {});

	double calculateConcentration(double rawSignal, const CalculationContext& context) const override;
	std::string getModelName() const override;
	std::string getGasName() const override;
	std::string getModelType() const override;


	// 以下是 ConcentrationModel 特有的方法
	double getSlope() const;											//get k
	double getIntercept() const;										//get b
	std::time_t getCreationTimestamp() const;							//get time
	double getCharacteristicPeak() const;								//获取特征峰X位置
	const std::vector<std::string>& getParticipatingCurveNames() const;	// 获取参与计算的曲线名称列表

private:
	std::string m_modelName;                            // 模型的唯一名称       
	std::string m_gasName;							    // 模型对应的气体类型/样品名称
	std::string m_modelType;					        // 模型类型（例如 "浓度法"）
	double      m_slope;							    // 模型的斜率 (a)
	double      m_intercept;					        // 模型的截距 (b)
	double      m_characteristicPeak;				    // 特征峰位置X
	std::vector<std::string> m_participatingCurveNames; // 参与拟合的曲线名称列表
	std::time_t m_creationTimestamp;				    // 模型创建时的时间戳，用于记录模型的创建时间
};

#endif // CONCENTRATIONMODEL_H
