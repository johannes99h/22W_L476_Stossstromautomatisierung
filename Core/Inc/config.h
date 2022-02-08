#include 	"stm32l4xx_hal.h"

extern 		TIM_HandleTypeDef htim2;
extern 		TIM_HandleTypeDef htim3;

uint8_t 	counterSeconds;
uint8_t 	maxSeconds;
uint8_t		counterActivations;
uint8_t		maxActivations;

void 		HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim);

int 		activateOutput();
void 		errorHandler();
