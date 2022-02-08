#include <config.h>



uint8_t 	counterSeconds 		= 0;
uint16_t	counterActivations 	= 0;
uint8_t		maxActivations 		= 255;



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim2)
{
	// Inkrementierung einmal pro Minute per ISR
	counterSeconds++;

	if(counterActivations == maxActivations)
	{
		HAL_TIM_Base_Stop_IT(&htim2);
		// UART-Output
		break;
	}

	HAL_UART_Transmit(&huart4, (uint8_t *)counterSeconds, sizeof(counterSeconds), 10);

	if(counterSeconds == 60)
	{
		if(activateOutput() == HAL_OK)
		{
			// Aktivierungscounter erhöhen & Sekundenzähler zurücksetzen
			counterActivations++;
			counterSeconds = 0;
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim3)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
}



int activateOutput()
{
	// Pin Toggle (Reset per TIM3-ISR)
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

	// Start des Delay-Timers
	HAL_TIM_Base_Start_IT(&htim3);

	// Sekundenzähler neustarten
	HAL_TIM_Base_Start_IT(&htim2);

	return HAL_OK;
}

void errorHandler()
{

}
