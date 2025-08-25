#ifndef IGASMODEL_H
#define IGASMODEL_H

#include <string>
#include <vector>
// �����ڼ���ʱ���ݶ�������������Ľṹ��
struct CalculationContext {
	// ���ڷ�ѹ����������õ���ѹ������Ũ�ȷ�������Ժ��ԡ�
	double totalPressure_Pa = 101325.0; // Ĭ��Ϊһ����׼����ѹ,�ɸ�����Ҫ�޸�
};

/**
* @class IGasModel
* @brief ���о���ģ����ĳ������ӿڡ�
*
* ����������ģ�Ͷ������ṩ��ͨ�ù��ܣ������Ũ�ȡ���ȡ���Ƶȡ�
* ʹ�ô��麯����ǿ���������ʵ����Щ���ܡ�
*/
class IGasModel
{
public:
	virtual ~IGasModel() = default;

	/**
	* @brief ���������źţ����������Ũ�ȡ�
	* @param rawSignal �������ɼ�����ԭʼ�ź�ֵ (Y)��
	* @param context ��������������������ѹ���������ġ�
	* @return �����������Ũ��ֵ (X)��
	*/
	virtual double calculateConcentration(double rawSignalY, const CalculationContext& context) const = 0;

	virtual std::string getModelName() const = 0;
	virtual std::string getGasName() const = 0;
	virtual std::string getModelType() const = 0;
};

#endif // IGASMODEL_H