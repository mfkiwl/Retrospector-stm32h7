void OTG_FS_IRQHandler(void) {
	usb.USBInterruptHandler();
}

/*// USART Decoder
void USART3_IRQHandler() {
	//if ((USART3->ISR & USART_ISR_RXNE_RXFNE) != 0 && !uartCmdRdy) {
	if (!uartCmdRdy) {
		uartCmd[uartCmdPos] = USART3->RDR; 				// accessing RDR automatically resets the receive flag
		if (uartCmd[uartCmdPos] == 10) {
			uartCmdRdy = true;
			uartCmdPos = 0;
		} else {
			uartCmdPos++;
		}
	}
}*/

// I2S Interrupt
void SPI2_IRQHandler() {

	sampleClock = !sampleClock;

	if (sampleClock) {
		DigitalDelay.samples[writePos] = (int16_t)(ADC_array[ADC_Audio_L] << 4) - adcZeroOffset;
		SPI2->DR = DigitalDelay.calcSample();		// Left Channel
		//SPI2->DR = testOutput;
	} else {

		SPI2->DR = testOutput;				// Right Channel
		testOutput += 100;
	}

	// Toggle LED for testing
	if (sampleClock)
		GPIOC->ODR |= GPIO_ODR_OD11;
	else
		GPIOC->ODR &= ~GPIO_ODR_OD11;

	nextSample = true;		// request next sample be prepared
}

void EXTI9_5_IRQHandler(void) {

	// Handle incoming clock pulse
	if (EXTI->PR & EXTI_PR_PR7) {
		clockInterval = SysTickVal - lastClock;
		lastClock = SysTickVal;
		newClock = true;
		EXTI->PR |= EXTI_PR_PR7;							// Clear interrupt pending
	}


}

// System interrupts
void NMI_Handler(void) {}

void HardFault_Handler(void) {
	while (1) {}
}
void MemManage_Handler(void) {
	while (1) {}
}
void BusFault_Handler(void) {
	while (1) {}
}
void UsageFault_Handler(void) {
	while (1) {}
}

void SVC_Handler(void) {}

void DebugMon_Handler(void) {}

void PendSV_Handler(void) {}

void SysTick_Handler(void) {
	++SysTickVal;
}
