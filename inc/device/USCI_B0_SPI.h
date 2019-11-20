/*
 * USCI_B0_SPI.h
 *
 *  Created on: 2018Äê7ÔÂ2ÈÕ
 *      Author: Sewell Lin
 */

#ifndef INC_DEVICE_USCI_B0_SPI_H_
#define INC_DEVICE_USCI_B0_SPI_H_

extern void USCI_B0_SPI_init(void);
extern unsigned char USCI_B0_SPI_transmit_frame(unsigned char *p_buff, unsigned char num);
extern unsigned char USCI_B0_SPI_receive_frame(unsigned char *p_buff, unsigned char num);

extern void USCI_B0_SPI_rx_isr_handle(void);
extern void USCI_B0_SPI_tx_isr_handle(void);



#endif /* INC_DEVICE_USCI_B0_SPI_H_ */
