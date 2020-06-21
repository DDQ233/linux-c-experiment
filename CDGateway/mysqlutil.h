#ifndef __MYSQLUTIL_H__
#define __MYSQLUTIL_H__

typedef struct {
    char* deviceId;
    int deviceStatus;
    char* remark;
}DeviceInfo;

typedef struct {
    char* deviceId;
    char* data;
    char* remark;
    char* date;
}Data;

// Connect mysql server
MYSQL connectMysql(char *addr, char *username, char *pwd, char *database);
// Free result set
void freeResult(MYSQL_RES *pRes);
// Close resources
// void close(MYSQL *com_mysql);

// Query
MYSQL_RES* query(MYSQL com_mysql, char *sql);
// Insert one data
MYSQL_RES* addData(MYSQL com_mysql, Data data);
// Insert one device
MYSQL_RES* addDevice(MYSQL com_mysql, DeviceInfo deviceInfo);
// Update device information
MYSQL_RES* updateDevice(MYSQL com_mysql, DeviceInfo deviceInfo);
// Update device status, online or offline
MYSQL_RES* updateDeviceStatus(MYSQL com_mysql, DeviceInfo deviceInfo);
// Find all deivec information
MYSQL_RES* findAllDevice(MYSQL com_mysql);
// Find a device information by device id
MYSQL_RES* findDeviceById(MYSQL com_mysql, char* id);
// Delete data in 24h
MYSQL_RES* deleteDataIn24H(MYSQL com_mysql);
// Delete all data
MYSQL_RES* deleteAllData(MYSQL com_mysql);


#endif