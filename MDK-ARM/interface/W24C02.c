#include "W24C02.h"


/**
 * @brief 写一个字节
 * 
 * @param byte_addr 字节地址
 * @param data 数据
 */
void W24C02_WriteByte(uint8_t byte_addr, uint8_t data){
    //调用HAL方法完成写入
   HAL_I2C_Mem_Write(&hi2c2, W24C02_ADDR, byte_addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 1000);
   //比较老的存储器需要延时
    HAL_Delay(10);
}

/**
 * @brief 读一个字节
 * 
 * @param byte_addr 字节地址
 * @param data 数据
 */
uint8_t W24C02_ReadByte(uint8_t byte_addr){
    uint8_t data;
    //调用HAL方法完成读取
    HAL_I2C_Mem_Read(&hi2c2, W24C02_ADDR, byte_addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 1000);
    return data;
}

/**
 * @brief 写多个字节 不能超过一页且从头开始写
 * 
 * @param byte_addr 字节地址
 * @param data 数据
 * @param len 长度
 */
void W24C02_WriteBytes(uint8_t byte_addr, uint8_t *data, uint8_t len){
    if((byte_addr%16) + len > 16){
        debug_printf("写入长度超过一页\r\n");
    }

    HAL_I2C_Mem_Write(&hi2c2, W24C02_ADDR, byte_addr, I2C_MEMADD_SIZE_8BIT, data, len, 1000);

    //比较老的存储器需要延时    
    HAL_Delay(10);
}

/**
 * @brief 读多个字节
 * 
 * @param byte_addr 字节地址
 * @param data 数据
 * @param len 长度
 */
void W24C02_ReadBytes(uint8_t byte_addr, uint8_t *data, uint8_t len){
    HAL_I2C_Mem_Read(&hi2c2, W24C02_ADDR, byte_addr, I2C_MEMADD_SIZE_8BIT, data, len, 1000);
    
}
