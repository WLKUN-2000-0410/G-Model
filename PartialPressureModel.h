#ifndef PARTIALPRESSUREMODEL_H
#define PARTIALPRESSUREMODEL_H

#include "IGasModel.h" 
#include <ctime>

/**
* @class PartialPressureModel
* @brief "��ѹ��"�ľ���ʵ�֣�û�нؾࡣ
*
* ģ�ͷ���Ϊ: �ź�(Y) = k * ��ѹ(X)
* ���� k Ϊб�ʡ�
*/
class PartialPressureModel : public IGasModel
{
public:
	/**
	* @brief PartialPressureModel �Ĺ��캯����
	* @param modelName ģ�͵�Ψһ��ʶ���ơ�
	* @param gasName ģ�Ͷ�Ӧ���������͡�
	* @param slope ģ�͵�б�� 'k'��
	*/
	PartialPressureModel(const std::string& modelName, 
						 const std::string& gasName,
						 double characteristicPeak,
						 double slope,
						 const std::vector<std::string>& participatingCurveNames = {});

	// --- ��д����Ĵ��麯�� ---

	/**
	* @brief ���������źţ����������Ũ��(ppm)��
	*
	* �÷���������ִ�У�
	* 1. ����ģ�ͷ������ѹ����ѹ(X) = �ź�(Y) / k
	* 2. ���ݷ�ѹ����ѹ�����Ũ�ȣ�ppm = (��ѹ / ��ѹ) * 10^6
	* @param rawSignal �������ɼ�����ԭʼ�ź�ֵ (Y)��
	* @param context ������ѹ�ȶ����������������ġ�
	* @return ���ռ����������Ũ��ֵ (ppm)��
	*/
	double calculateConcentration(double rawSignal, const CalculationContext& context) const override;

	std::string getModelName() const override;
	std::string getGasName() const override;
	std::string getModelType() const override;

	// --- PartialPressureModel ���еķ��� ---

	/**
	* @brief ��ȡģ�͵�б�� (���� 'k')��
	* @return б��ֵ��
	*/
	double getSlope() const;
	double getCharacteristicPeak() const; // <-- ��������
	std::time_t getCreationTimestamp() const;
	const std::vector<std::string>& getParticipatingCurveNames() const;  //��ȡ�����������������б�

private:
	std::string m_modelName;
	std::string m_gasName;
	std::string m_modelType;
	double      m_slope; // б�� (k)
						 // ע�⣺����û�� m_intercept ��Ա����
	double      m_characteristicPeak; // ������λ��
	std::vector<std::string> m_participatingCurveNames;
	std::time_t m_creationTimestamp;
};

#endif // PARTIALPRESSUREMODEL_H