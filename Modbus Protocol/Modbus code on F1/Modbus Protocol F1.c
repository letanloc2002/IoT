/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdint.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
uint16_t CRC16_Check_A(uint8_t Num); //Ham CRC 16
uint8_t EoF = 0;
uint16_t CRC16_Check_Prepare_Send(uint8_t Num) ;//Ham CRC 16
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
volatile char buffer = 0x00;
volatile char data_buff_0_R[100];
volatile uint8_t count_R = 0;
uint8_t CRC_test[3] = "OKE";
uint8_t BUFF_2_T[100];
uint8_t Prepare_data_send[100];
uint8_t size_funtion = 0;
uint8_t ResetIndexBuffer = 0;

uint16_t Slave_Register[20] = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 	
};
/* USER CODE BEGIN 0 */
uint8_t hex4_to_dec(uint16_t hexa)
{
	uint8_t first = hexa&0x0F;
	uint8_t second =(hexa >> 4)&0x0F;
	uint8_t third =(hexa >> 8)&0x0F;
	uint8_t fourth =(hexa >> 12)&0x0F;
	uint8_t dec = first + second*16 + third*16*16 + fourth*16*16*16;
	return dec;
	
}

uint8_t hex2_to_dec(uint8_t hexa)
{
	uint8_t first = hexa&0x0F;
	uint8_t second =(hexa >> 4)&0x0F;
	uint8_t dec = first + second*16 ;
	return dec;
	
}

unsigned int combineHex(unsigned char high, unsigned char low)
{
    // K?t h?p giá tr? HIGH và LOW thành m?t giá tr? 16 bit
    unsigned int result = ((unsigned int)high << 8) | low;

    return result;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	HAL_UART_Receive_IT(&huart2,(uint8_t*)&buffer,1);
	uint16_t check_CRC = 0x11;
	uint8_t check_CRC_HIGH = 0;
	uint8_t check_CRC_LOW = 0;
	uint8_t received_CRC_HIGH = 1;
	uint8_t received_CRC_LOW = 1;
	uint16_t start_address = 0;
	uint16_t number_registers = 0;
	uint8_t number_byte_respond = 0;
	uint16_t CRC_respond = 0x11;
	uint8_t check_CRC_respond_LOW = 0xFF;
	uint8_t check_CRC_respond_HIGH = 0xFF;
	
  while (1)
  {
		//Reset bien reset index buffer
		ResetIndexBuffer = 0;
		
    /* USER CODE END WHILE */
		for (int i = 0; i < 100; i++)
		{
			BUFF_2_T[i] = data_buff_0_R[i];
		}
		//Kiem tra dia chi slave
		if (BUFF_2_T[0] == 0x05)
		{
			//Kiem tra do dai cau lenh can doc
			if (BUFF_2_T[1] == 0x03) 
			{
				size_funtion = 8;
				//Tach CRC tinh duoc (bi dao nguoc ????)
				check_CRC = CRC16_Check_A(size_funtion - 2);
				check_CRC_LOW = (check_CRC >> 8)&0xFF;
				check_CRC_HIGH = check_CRC&0xFF;
				
				//Gan CRC nhan duoc
				received_CRC_HIGH = BUFF_2_T[size_funtion - 2];
				received_CRC_LOW = BUFF_2_T[size_funtion - 1];
				
				//Neu CRC ok thi lam viec
				if ((check_CRC_HIGH == received_CRC_HIGH)&&(check_CRC_LOW == received_CRC_LOW))
				{
						//Kiem tra Dia chi thanh ghi dau tien can doc
						start_address = hex4_to_dec(combineHex(BUFF_2_T[2], BUFF_2_T[3]));
					
						//Gui so data can gui
						number_byte_respond = combineHex(BUFF_2_T[4],BUFF_2_T[5])*2;
						//HAL_UART_Transmit(&huart2, (uint8_t*)&number_byte_respond, 1, 100);
						
						//Gui data can gui
						for (int j = 0; j < number_byte_respond/2 ; j++)
						{
							uint8_t Slave_Register_HIGH = (Slave_Register[j + start_address] >> 8)& 0x00FF;
							uint8_t Slave_Register_LOW = Slave_Register[j + start_address]& 0x00FF;
							
							Prepare_data_send[2*j + 3] = Slave_Register_HIGH;
							Prepare_data_send[2*j + 1 + 3] = Slave_Register_LOW;
						}
						
						//Gui CRC
						Prepare_data_send[0] = BUFF_2_T[0];
						Prepare_data_send[1] = BUFF_2_T[1];
						Prepare_data_send[2] = number_byte_respond;
						
						CRC_respond = CRC16_Check_Prepare_Send(2*(BUFF_2_T[5] - 1) + 1 + 3 + 1);
						
						HAL_UART_Transmit(&huart2, (uint8_t*)&Prepare_data_send,2*(BUFF_2_T[5] - 1) + 1 + 3 + 1, 100);
						//Tach CRC tinh duoc (bi dao nguoc ????)
						check_CRC_respond_HIGH = (CRC_respond >> 8)&0xFF;
						check_CRC_respond_LOW = CRC_respond&0xFF;
						HAL_UART_Transmit(&huart2, (uint8_t*)&check_CRC_respond_LOW,1, 100);
						HAL_UART_Transmit(&huart2, (uint8_t*)&check_CRC_respond_HIGH,1, 100);
				}
			}	
			
			
			if (BUFF_2_T[1] == 0x10) 
			{
				size_funtion = hex2_to_dec(combineHex(BUFF_2_T[4],BUFF_2_T[5])*2) + 9;
				//Tach CRC tinh duoc (bi dao nguoc ????)
				check_CRC = CRC16_Check_A(size_funtion - 2);
				check_CRC_LOW = (check_CRC >> 8)&0xFF;
				check_CRC_HIGH = check_CRC&0xFF;
				
				//Gan CRC nhan duoc
				received_CRC_HIGH = BUFF_2_T[size_funtion - 2];
				received_CRC_LOW = BUFF_2_T[size_funtion - 1];
				
				
				//Neu CRC ok thi lam viec
				if ((check_CRC_HIGH == received_CRC_HIGH)&&(check_CRC_LOW == received_CRC_LOW))
				{
						start_address = hex4_to_dec(combineHex(BUFF_2_T[2], BUFF_2_T[3]));
						number_byte_respond = combineHex(BUFF_2_T[4],BUFF_2_T[5])*2;
					
					//Write data received 
					//Data nhan duoc luu trong BUFF_2_T, sau do phai gan vao bo nho Slave_Register
					for (int index = 0; index < number_byte_respond/2; index++)
					{
						Slave_Register[index + start_address] = combineHex(BUFF_2_T[index*2 + 7], BUFF_2_T[index*2 + 8]);
					}
					
					//Respond to master
					for (int m = 0; m < 6; m++)
					{
						Prepare_data_send[m] = BUFF_2_T[m];
					}
					HAL_UART_Transmit(&huart2, (uint8_t*)&Prepare_data_send,6, 100);
					
					//Caculate CRC
					CRC_respond = CRC16_Check_Prepare_Send(6);
					
					//Tach CRC tinh duoc (bi dao nguoc ????)
					check_CRC_respond_HIGH = (CRC_respond >> 8)&0xFF;
					check_CRC_respond_LOW = CRC_respond&0xFF;
					HAL_UART_Transmit(&huart2, (uint8_t*)&check_CRC_respond_LOW,1, 100);
					HAL_UART_Transmit(&huart2, (uint8_t*)&check_CRC_respond_HIGH,1, 100);
						
				}
				
			}
			
			
		}			
			
		HAL_Delay(1000);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
uint16_t CRC16_Check_A(uint8_t Num) //Ham CRC 16
 {
    uint8_t x,y,i; uint16_t Crc; //Bien Cuc Bo
   
    Crc=0xFFFF; //Init
   
    for(x=0;x<Num;++x) //Lap Vong
    {        
       y=BUFF_2_T[x]; //Lay Byte Trong Bo Dem CRC 16
       Crc=Crc^y; //Calculate the CRC
       
       for(i=8;i!=0;--i) //Shift Right
       {
          if((Crc&0x0001)!=0) //If the LSB is set
          {
             Crc>>=1; //Shift right and XOR 0xA001
             Crc^=0xA001;
          }
          else //Else LSB is not set  
          {
             Crc>>=1; //Just shift right
          }  
       }
    }
         
    //Swapping of the high and low CRC bytes      
    return Crc; //Tra Ve CRC_16 Tinh Duoc
 } 
 
uint16_t CRC16_Check_Prepare_Send(uint8_t Num) //Ham CRC 16
 {
    uint8_t x,y,i; uint16_t Crc; //Bien Cuc Bo
   
    Crc=0xFFFF; //Init
   
    for(x=0;x<Num;++x) //Lap Vong
    {        
       y=Prepare_data_send[x]; //Lay Byte Trong Bo Dem CRC 16
       Crc=Crc^y; //Calculate the CRC
       
       for(i=8;i!=0;--i) //Shift Right
       {
          if((Crc&0x0001)!=0) //If the LSB is set
          {
             Crc>>=1; //Shift right and XOR 0xA001
             Crc^=0xA001;
          }
          else //Else LSB is not set  
          {
             Crc>>=1; //Just shift right
          }  
       }
    }
         
    //Swapping of the high and low CRC bytes      
    return Crc; //Tra Ve CRC_16 Tinh Duoc
 }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == huart2.Instance) // check interrupt of serial port 2
	{
		if (ResetIndexBuffer == 0) 
		{
			count_R=0;
			ResetIndexBuffer = 1;
			data_buff_0_R[count_R] = buffer;
		}
		else
		{
		  count_R++;
			data_buff_0_R[count_R] = buffer;

		}
		HAL_UART_Receive_IT(&huart2, (uint8_t*)&buffer, 1);
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
