- 使用arduino nano制作gps速度显示模块
- gps模块选用NEO-7M
- 数码管选用tm1637共阴数码管
- 实现功能，速度小于1时显示gps连接卫星数量，大于1时显示实时速度，保留一位小数点
- gps模块启动时配置为ubx模式，提高数据刷新率
- 使用到的库

GyverTM1637 <https://github.com/GyverLibs/GyverTM1637>

UbxGps <https://github.com/loginov-rocks/UbxGps>