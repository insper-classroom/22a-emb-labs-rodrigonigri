/*
 * defines.h
 *
 * Created: 14/04/2022 00:08:53
 *  Author: rodri
 */ 


#ifndef DEFINES_H_
#define DEFINES_H_

// OLED
// BOTAO1 --> PD28
#define BUT_PIO1				PIOD
#define BUT_PIO1_ID				ID_PIOD
#define BUT_PIO1_IDX			28
#define BUT_PIO_1_IDX_MASK		(1u << BUT_PIO1_IDX)


// SENSOR
// TRIG --> PA24
// ECHO --> PA2

#define TRIG_PIO				PIOA
#define TRIG_PIO_ID				ID_PIOA
#define TRIG_PIO_IDX			24
#define TRIG_PIO_IDX_MASK		(1u << TRIG_PIO_IDX)

#define ECHO_PIO				PIOA
#define ECHO_PIO_ID				ID_PIOA
#define ECHO_PIO_IDX			2
#define ECHO_PIO_IDX_MASK		(1u << ECHO_PIO_IDX)




#endif /* DEFINES_H_ */