#include "lcd1602.h"
#include "main.h"
#include "delay.h"

#define DATA_GPIO_Port GPIOA
#define DATA_4BIT_MODE

static inline void LCD_RS_L()
{
    HAL_GPIO_WritePin(LCD1602_RS_GPIO_Port, LCD1602_RS_Pin, GPIO_PIN_RESET);
}

static inline void LCD_RS_H()
{
    HAL_GPIO_WritePin(LCD1602_RS_GPIO_Port, LCD1602_RS_Pin, GPIO_PIN_SET);
}

static inline void LCD_RW_L()
{
    HAL_GPIO_WritePin(LCD1602_RW_GPIO_Port, LCD1602_RW_Pin, GPIO_PIN_RESET);
}

static inline void LCD_RW_H()
{
    HAL_GPIO_WritePin(LCD1602_RW_GPIO_Port, LCD1602_RW_Pin, GPIO_PIN_SET);
}

static inline void LCD_EN_L()
{
    HAL_GPIO_WritePin(LCD1602_EN_GPIO_Port, LCD1602_EN_Pin, GPIO_PIN_RESET);
}

static inline void LCD_EN_H()
{
    HAL_GPIO_WritePin(LCD1602_EN_GPIO_Port, LCD1602_EN_Pin, GPIO_PIN_SET);
}

static inline void LCD_W_Data(uint8_t d)
{
    // 直接写输出数据寄存器操作bit4-bit7
    DATA_GPIO_Port->ODR = d;
}

static int LCD_GetBusyStatus()
{
    LCD_RS_L();
    LCD_RW_H();
    int status = HAL_GPIO_ReadPin(DATA_GPIO_Port, GPIO_PIN_7);
    LCD_EN_H();
    LCD_EN_L();

    return status;
}

void LCD_delay(void)
{
    delay_us(500);
}

#ifdef DATA_8BIT_MODE
void LCD_SendCmd(uint8_t cmd)
{
    LCD_RS_L();
    LCD_RW_L();
    LCD_W_Data(cmd);
    LCD_EN_H();
    LCD_delay();
    LCD_EN_L();
}

void LCD_SendData(uint8_t data)
{
    // while (LCD_GetBusyStatus() == 1);
    LCD_RS_H();
    LCD_RW_L();
    LCD_W_Data(data);
    LCD_EN_H();
    LCD_delay();
    LCD_EN_L();
}
#else
void LCD_SendCmd(uint8_t cmd)
{
    LCD_EN_L();
    LCD_RS_L();

    LCD_W_Data(cmd&0xF0);
    LCD_EN_H();
    HAL_Delay(15);
    LCD_EN_L();

    LCD_W_Data((cmd<<4)|0xF0);
    LCD_EN_H();
    HAL_Delay(15);
    LCD_EN_L();
}

void LCD_SendData(uint8_t data)
{
    LCD_EN_L();
    LCD_RS_H();

    LCD_W_Data(data&0xF0);
    LCD_EN_H();
    HAL_Delay(15);
    LCD_EN_L();

    LCD_W_Data((data<<4)|0xF0);
    LCD_EN_H();
    HAL_Delay(15);
    LCD_EN_L();
}
#endif

void LCD_Init()
{
#ifdef DATA_8BIT_MODE
    // 8-bit
    // LCD_delay();
    LCD_SendCmd(0x38);
    LCD_SendCmd(0x01);
    LCD_SendCmd(0x06);
    LCD_SendCmd(0x0F);
#else
    // 4-bit mode initialisation
    HAL_Delay(50);
    LCD_SendCmd(0x30);
    HAL_Delay(5);
    LCD_SendCmd(0x30);
    HAL_Delay(1);
    LCD_SendCmd(0x30);
    HAL_Delay(10);
    LCD_SendCmd(0x20);
    HAL_Delay(10);

    // display initialisation
    LCD_SendCmd(0x28);
    HAL_Delay(1);
    LCD_SendCmd(0x08);
    HAL_Delay(1);
    LCD_SendCmd(0x01);
    HAL_Delay(1);
    LCD_SendCmd(0x06);
    HAL_Delay(1);
    LCD_SendCmd(0x0C);
    HAL_Delay(1);
#endif
}

void LCD_ShowString(const char *str)
{
    while (*str)
    {
        LCD_SendData(*str++);
    }
}