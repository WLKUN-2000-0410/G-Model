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


	// ������ ConcentrationModel ���еķ���
	double getSlope() const;											//get k
	double getIntercept() const;										//get b
	std::time_t getCreationTimestamp() const;							//get time
	double getCharacteristicPeak() const;								//��ȡ������Xλ��
	const std::vector<std::string>& getParticipatingCurveNames() const;	// ��ȡ�����������������б�

private:
	std::string m_modelName;                            // ģ�͵�Ψһ����       
	std::string m_gasName;							    // ģ�Ͷ�Ӧ����������/��Ʒ����
	std::string m_modelType;					        // ģ�����ͣ����� "Ũ�ȷ�"��
	double      m_slope;							    // ģ�͵�б�� (a)
	double      m_intercept;					        // ģ�͵Ľؾ� (b)
	double      m_characteristicPeak;				    // ������λ��X
	std::vector<std::string> m_participatingCurveNames; // ������ϵ����������б�
	std::time_t m_creationTimestamp;				    // ģ�ʹ���ʱ��ʱ��������ڼ�¼ģ�͵Ĵ���ʱ��
};

#endif // CONCENTRATIONMODEL_H
