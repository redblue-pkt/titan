#ifndef ROTORCALC_H
#define ROTORCALC_H

double factorial_div(double value, int x)
{
	if(!x)
		return 1;
	else
	{
		while(x > 1)
			value = value / x--;
	}
	return value;
}

double powerd(double x, int y)
{
	int i=0;
	double ans=1.0;

	if(!y)
		return 1.000;
	else
	{
		while(i < y)
		{
			i++;
			ans = ans * x;
		}
	}
	return ans;
}

double SIN(double x)
{
	int i=0;
	int j=1;
	int sign=1;
	double y1 = 0.0;
	double diff = 1000.0;

	if(x < 0.0)
	{
		x = -1 * x;
		sign = -1;
	}

	while (x > 360.0 * M_PI / 180)
		x = x - 360 * M_PI / 180;

	if(x > (270.0 * M_PI / 180))
	{
		sign = sign * -1;
		x = 360.0 * M_PI / 180 - x;
	}
	else if(x > (180.0 * M_PI / 180))
	{
		sign = sign * -1;
		x = x - 180.0 * M_PI / 180;
	}
	else if(x > (90.0 * M_PI / 180))
		x = 180.0 * M_PI / 180 - x;

	while(powerd(diff, 2) > 1.0E-16)
	{
		i++;
		diff = j * factorial_div(powerd( x, (2 * i - 1)) ,(2 * i - 1));
		y1 = y1 + diff;
		j = -1 * j;
	}
	return(sign * y1);
}

double COS(double x)
{
	return SIN(90 * M_PI / 180 - x);
}

double ATAN(double x)
{
	int i = 0; //counter for terms in binomial series
	int j = 1; //sign of nth term in series
	int k = 0;
	int sign = 1; //sign of the input x
	double y = 0.0; //the output
	double deltay = 1.0; //the value of the next term in the series
	double addangle = 0.0; //used if arctan > 22.5 degrees

	if(x < 0.0)
	{
		x = -1 * x;
		sign = -1;
	}

	while(x > 0.3249196962)
	{
		k++;
		x = (x - 0.3249196962) / (1 + x * 0.3249196962);
	}

	addangle = k * 18.0 * M_PI / 180;

	while(powerd(deltay, 2) > 1.0E-16)
	{
		i++;
		deltay = j * powerd( x, (2 * i - 1)) / (2 * i - 1);
		y = y + deltay;
		j = -1 * j;
	}
	return(sign * (y + addangle));
}

double ASIN(double x)
{
	return 2 * ATAN( x / (1 + sqrt(1.0 - x * x)));
}

double RAD(double nr)
{
	return nr * M_PI / 180;
}

double DEG(double nr)
{
	return nr * 180 / M_PI;
}

double REV(double nr)
{
	return nr - floor(nr / 360.0) * 360;
}

double calcElevation(double SatLon, double SiteLat, double SiteLon, int Height_over_ocean)
{
	double a0 = 0.58804392;
	double a1 = -0.17941557;
	double a2 = 0.29906946E-1;
	double a3 = -0.25187400E-2;
	double a4 = 0.82622101E-4;
	double f = 1.00 / 298.257; //Earth flattning factor
	double r_sat = 42164.57; // Distance from earth centre to satellite
	double r_eq = 6378.14; //Earth radius
	double sinRadSiteLat=SIN(RAD(SiteLat));
	double cosRadSiteLat=COS(RAD(SiteLat)); 
	double Rstation = r_eq / ( sqrt( 1.00 - f * (2.00 - f) * sinRadSiteLat * sinRadSiteLat));
	double Ra = (Rstation + Height_over_ocean) * cosRadSiteLat;
	double Rz = Rstation*(1.00-f)*(1.00-f)*sinRadSiteLat; 
	double alfa_rx = r_sat * COS(RAD(SatLon-SiteLon)) - Ra;
	double alfa_ry = r_sat * SIN(RAD(SatLon-SiteLon));
	double alfa_rz = -Rz;
	double alfa_r_north = -alfa_rx * sinRadSiteLat + alfa_rz * cosRadSiteLat;
	double alfa_r_zenith = alfa_rx * cosRadSiteLat + alfa_rz * sinRadSiteLat; 
	double El_geometric = DEG(ATAN(alfa_r_zenith / sqrt(alfa_r_north * alfa_r_north + alfa_ry * alfa_ry))); 
	double x = fabs(El_geometric + 0.589);
	double refraction=fabs(a0 + a1 * x + a2 * x * x + a3 * x * x * x + a4 * x * x * x * x); 
	double El_observed = 0.00;

	if(El_geometric > 10.2)
		El_observed = El_geometric + 0.01617 * (COS(RAD(fabs(El_geometric))) / SIN(RAD(fabs(El_geometric))));
	else
		El_observed = El_geometric + refraction;

	if(alfa_r_zenith < -3000)
		El_observed = -99;

	return El_observed;
}

double calcAzimuth(double SatLon, double SiteLat, double SiteLon, int Height_over_ocean)
{
	double f = 1.00 / 298.257; //Earth flattning factor 
	double r_sat = 42164.57; //Distance from earth centre to satellite 
	double r_eq = 6378.14; //Earth radius
	double sinRadSiteLat = SIN(RAD(SiteLat));
	double cosRadSiteLat = COS(RAD(SiteLat));
	double Rstation = r_eq / ( sqrt( 1 - f * (2 - f) * sinRadSiteLat * sinRadSiteLat));
	double Ra = (Rstation + Height_over_ocean) * cosRadSiteLat;
	double Rz = Rstation * (1 - f) * (1 - f) * sinRadSiteLat;
	double alfa_rx = r_sat * COS(RAD(SatLon - SiteLon)) - Ra;
	double alfa_ry = r_sat * SIN(RAD(SatLon - SiteLon));
	double alfa_rz = -Rz;
	double alfa_r_north = -alfa_rx * sinRadSiteLat + alfa_rz * cosRadSiteLat; 
	double Azimuth = 0.00;

	if(alfa_r_north < 0)
		Azimuth = 180 + DEG(ATAN(alfa_ry / alfa_r_north));
	else
		Azimuth = REV(360 + DEG(ATAN(alfa_ry / alfa_r_north)));

	return Azimuth;
}

double calcDeclination(double SiteLat, double Azimuth, double Elevation)
{
	return DEG(ASIN(SIN(RAD(Elevation)) *
		SIN(RAD(SiteLat)) +
		COS(RAD(Elevation)) *
		COS(RAD(SiteLat)) +
		COS(RAD(Azimuth))));
}

double calcSatHourangle(double SatLon, double SiteLat, double SiteLon)
{
	double Azimuth = calcAzimuth(SatLon, SiteLat, SiteLon, 0);
	double Elevation = calcElevation( SatLon, SiteLat, SiteLon, 0);
	double a = - COS(RAD(Elevation)) * SIN(RAD(Azimuth));
	double b = SIN(RAD(Elevation)) * COS(RAD(SiteLat)) -
		COS(RAD(Elevation)) * SIN(RAD(SiteLat)) * COS(RAD(Azimuth));
	double ret = 180 + DEG(ATAN(a / b));

	if(Azimuth > 270)
	{
		ret = ((ret - 180) + 360);
		if(ret > 360)
			ret = 360 - (ret - 360);
	}

	if(Azimuth < 90)
		ret = (180 - ret);

	return ret;
}

#endif
