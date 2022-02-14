#include <config.h>

uint8_t 	counterSeconds 		= 58;
uint8_t		maxSeconds			= 60;
uint8_t		counterActivations 	= 0;
uint8_t		maxActivations 		= 255;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if (htim == &htim2)
		{
		// Inkrementierung einmal pro Minute per ISR
		counterSeconds++;

		if(counterSeconds == maxSeconds)
		{
			if(activateOutput() == HAL_OK)
			{
				// Aktivierungscounter erhöhen & Sekundenzähler zurücksetzen
				counterSeconds = 54;
			}
		}

		if(counterActivations == maxActivations)
		{
			HAL_TIM_Base_Stop_IT(&htim2);
		}
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
		// HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		counterActivations++;
		HAL_TIM_Base_Stop(&htim3);
	}

}

int activateOutput()
{
	// Pin Toggle (Reset per TIM3-ISR)
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	// HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);

	// Start des Delay-Timers
	HAL_TIM_Base_Start_IT(&htim3);

	// Sekundenzähler neustarten
	HAL_TIM_Base_Start_IT(&htim2);

	return HAL_OK;
}

void errorHandler()
{

}
