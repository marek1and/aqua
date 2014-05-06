#ifndef AVRLIBDEFS_H
#define AVRLIBDEFS_H

#ifndef outb
	#define	outb(addr, data)	addr = (data)
#endif
#ifndef inb
	#define	inb(addr)			(addr)
#endif
#ifndef outw
	#define	outw(addr, data)	addr = (data)
#endif
#ifndef inw
	#define	inw(addr)			(addr)
#endif
#ifndef BV
	#define BV(bit)			(1<<(bit))
#endif
#ifndef cbi
	#define cbi(reg,bit)	reg &= ~(BV(bit))
#endif
#ifndef sbi
	#define sbi(reg,bit)	reg |= (BV(bit))
#endif
#ifndef cli
	#define cli()			__asm__ __volatile__ ("cli" ::)
#endif
#ifndef sei
	#define sei()			__asm__ __volatile__ ("sei" ::)
#endif

#define GNUC_PACKED __attribute__((packed)) 

// port address helpers
#define DDR(x) ((x)-1)    // address of data direction register of port x
#define PIN(x) ((x)-2)    // address of input register of port x

// MIN/MAX/ABS macros
#define MIN(a,b)			((a<b)?(a):(b))
#define MAX(a,b)			((a>b)?(a):(b))
#define ABS(x)				((x>0)?(x):(-x))

// constants
#define PI		3.14159265359

#endif
