#ifndef MY_SENSOR_H
#define MY_SENSOR_H
void sensor_init(void);

//gps
void gps_init(void);
void gps_get_data(void);
extern WbDeviceTag gps;
extern const double *value;
extern double sensor_v;

//pen
void pen_init(void);
extern WbDeviceTag pen;
extern int gps_point_count;

#endif
