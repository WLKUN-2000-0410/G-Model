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
	ConcentrationModel(const std::string& modelName, const std::string& gasName, double slope, double intercept);

	double calculateConcentration(double rawSignal, const CalculationContext& context) const override;
	std::string getModelName() const override;
	std::string getGasName() const override;
	std::string getModelType() const override;

	// 以下是 ConcentrationModel 特有的方法
	double getSlope() const;                   //get k
	double getIntercept() const;               //get b
	std::time_t getCreationTimestamp() const;  //get time

private:
	std::string m_modelName;                   
	std::string m_gasName;
	double      m_slope;
	double      m_intercept;
	std::time_t m_creationTimestamp;
};

#endif // CONCENTRATIONMODEL_H
