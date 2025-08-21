#ifndef CONCENTRATIONMODEL_H
#define CONCENTRATIONMODEL_H

#include "IGasModel.h" 
#include <ctime>

/**
* @class ConcentrationModel
* @brief "Ũ�ȷ�"�ľ���ʵ�֡�
*
* ģ�ͷ���Ϊ: �ź�(Y) = k * Ũ��(X) + b
*/
class ConcentrationModel : public IGasModel
{
public:
	/**
	* @brief ConcentrationModel �Ĺ��캯����
	* @param modelName ģ�͵�Ψһ��ʶ���ơ�
	* @param gasName ģ�Ͷ�Ӧ���������͡�
	* @param slope ģ�͵�б�� 'a'��
	* @param intercept ģ�͵Ľؾ� 'b'��
	*/
	ConcentrationModel(const std::string& modelName, const std::string& gasName, double slope, double intercept);

	double calculateConcentration(double rawSignal, const CalculationContext& context) const override;
	std::string getModelName() const override;
	std::string getGasName() const override;
	std::string getModelType() const override;

	// ������ ConcentrationModel ���еķ���
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
