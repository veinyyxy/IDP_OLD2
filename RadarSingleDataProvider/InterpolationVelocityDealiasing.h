#pragma once
using namespace goto_gis;
class InterpolationVelocityDealiasing
{
public:
	InterpolationVelocityDealiasing(void);
	~InterpolationVelocityDealiasing(void);
	bool  QualityControl_fuzzy(const std::string& filename);//速度退模糊算法
};

