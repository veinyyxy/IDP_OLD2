#pragma once
class InterpolationRadarDataInterface
{
public:
	virtual void Profiles() = 0;
	virtual bool GetSectionParam() = 0;
	virtual double	Get_Angle_Of_Direction(double dx, double dy) = 0;
	virtual double	Get_Length(double dx, double dy) = 0;
	virtual void SetStartEnd(double* x1, double* y1, double* x2, double* y2) = 0;
	virtual void SetStartEnd(double* x1, double* y1, double* x2, double* y2,double dR,double pc_x,double pc_y) = 0;
};

