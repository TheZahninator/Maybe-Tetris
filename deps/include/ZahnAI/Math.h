#define ZAHN_MAP(x, omin, omax, tmin, tmax) \
	(x - omin) / (omax - omin) * (tmax - tmin) + tmin