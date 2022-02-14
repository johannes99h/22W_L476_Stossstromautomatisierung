#include <config.h>

uint8_t 	counterSeconds 		= 0;
uint8_t		maxSeconds			= 60;
uint8_t		counterActivations 	= 0;
uint8_t		maxActivations 		= 255;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if (htim == &htim2)
		{
		// Inkrementierung + Low-Pulse-Auslösung einmal pro Minute per ISR
		counterSeconds++;

		if(counterSeconds == maxSeconds)
		{
			if(activateOutput() == HAL_OK)
			{
				// Aktivierungscounter erhöhen & Sekundenzähler zurücksetzen
				counterSeconds = 0;

				// UART-Benachrichtigung
				uint8_t activationSuccessfulSize;
				char activationSuccessful[17];
				activationSuccessfulSize = sprintf(activationSuccessful, "Auslösung %d \n\r", counterActivations);
				HAL_UART_Transmit(&huart4, (uint8_t *)activationSuccessful, activationSuccessfulSize, 10);
			}
		}

		if(counterActivations == maxActivations)
		{
			// Stoppen des Sekundenzählers
			HAL_TIM_Base_Stop_IT(&htim2);

			// UART-Benachrichtigung -> Sequenz vollständig durchlaufen
			uint8_t sequenceFinishedSize;
			char sequenceFinished[35];
			sequenceFinishedSize = sprintf(sequenceFinished, "Sequenz vollständig durchlaufen\n\r");
			HAL_UART_Transmit(&huart4, (uint8_t *)sequenceFinished, sequenceFinishedSize, 10);

			// uint8_t bufferCrcSize;
			// char bufferCrc[8];
			// bufferCrcSize = sprintf(bufferCrc, "%ld", CRCtempC[ntcNumber]);
			// HAL_UART_Transmit(&huart1, (uint8_t *)bufferCrc, bufferCrcSize, 10);
		}
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
		counterActivations++;
		HAL_TIM_Base_Stop(&htim3);
	}

}

int activateOutput()
{
	// Pin Toggle (Reset per TIM3-ISR)
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);

	// Start des Delay-Timers
	HAL_TIM_Base_Start_IT(&htim3);

	// Sekundenzähler neustarten
	HAL_TIM_Base_Start_IT(&htim2);

	return HAL_OK;
}

void errorHandler()
{

}
