#ifndef MODELFITTER_H
#define MODELFITTER_H

#include "ConcentrationModel.h" // ��Ҫ����������Ϊ����Ҫ����������͵Ķ���
#include "PartialPressureModel.h"
#include <vector>               // ���ڽ���һ�����ݵ��б�

/**
* @struct DataPoint
* @brief ���ڱ�ʾ����У׼���ݵ�(X,Y)�Ľṹ�塣
*
*/
struct DataPoint {
	std::string curveName; // �������� 
	double x_value; // �Ա��� X (������Ũ�� �� ��ѹ)
	double y_value; // ����� Y (�����ź�)

	// ���캯��
	DataPoint(double x, double y, const std::string& curve)
		: x_value(x), y_value(y), curveName(curve) {
	}
	// ����ԭ��ʹ�÷�ʽ
	DataPoint(double x, double y)
		: x_value(x), y_value(y), curveName("") {
	}
};

/**
* @class ModelFitter
* @brief һ����̬�����࣬���ڸ���У׼���ݴ���ģ�͡�
*
* ������װ�˺��ĵ���ѧ����㷨�����磺��С���˷�����-- ai
* ʵ�����ĵ����ᵽ�ġ��Զ�������߹��ܡ���
*/
class ModelFitter
{
public:
	/**
	* @brief ��һ��У׼���ݽ���������ϣ�������һ��ConcentrationModel����
	* @param modelName Ҫ������ģ�͵�Ψһ���ơ�
	* @param gasName ģ�Ͷ�Ӧ���������͡�
	* @param calibrationPoints һ���������У׼���ݵ��������
	* @return һ��������Ͻ�� (б��a, �ؾ�b) �� ConcentrationModel ����
	* @throw std::invalid_argument ������ݵ�����2�����޷�����������ϡ�
	*/
	static ConcentrationModel fitConcentrationModel(
		const std::string& modelName,
		const std::string& gasName,
		double characteristicPeak,
		const std::vector<DataPoint>& calibrationPoints
	);

	/**
	* @brief ������Ϊ����ѹ����ִ��ǿ��ͨ��ԭ������Իع� (y=kx)��
	*/
	static PartialPressureModel fitPartialPressureModel(
		const std::string& modelName,
		const std::string& gasName,
		double characteristicPeak,
		const std::vector<DataPoint>& calibrationPoints
	);

private:
	// �����캯����Ϊ˽�У���ֹ�ⲿ���� ModelFitter ��ʵ����
	// ��ǿ������ֻ��һ����̬���ܵļ��ϡ�
	ModelFitter() = default;
};

#endif // MODELFITTER_H
