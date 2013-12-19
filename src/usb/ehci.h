//USBSTS
#define BIT(x) (1<<(x))
#define USB_CMD 			0x20
#define 	CMD_RS 			0
#define 	CMD_HCRESET 	1
#define 	CMD_FRLISTSIZE 	2
#define 	CMD_PERIODIC 	4
#define 	CMD_ASYNC 		5
#define 	CMD_DOORBELL 	6
#define 	CMD_INT_CTRL 	23

#define USB_STS 0x24
#define     STS_HCHalted 12
/*
#define     STS_
#define     STS_
#define     STS_
#define     STS_
*/
#define USB_INTR 0x28
#define USB_INT 0
#define USB_ERR_INT 1
#define USB_PORT_CHANGE_INT 2
#define USB_FRLIST_RO_INT 3
#define USB_HS_ERR_INT 4
#define USB_ASYNC_ADVANCE_INT 5

typedef struct _ehci {
	uint32_t ioBase;
#define USB_INTR 0x28
	uint32_t transComplete;
	uint8_t irq;
	uint32_t mmBase;
	ehci_CapRegs_t *CapRegs;
	ehci_OpRegs_t *OpRegs;
}ehci_t;

#define MAX_EHCIS  5
extern ehci_t  ehci[MAX_EHCIS];

typedef struct
{
	volatile uint8_t  CAPLENGTH;        // Core Capability Register Length
	volatile uint8_t  reserved;
	volatile uint16_t HCIVERSION;       // Core Interface Version Number
	volatile uint32_t HCSPARAMS;        // Core Structural Parameters //
	volatile uint32_t HCCPARAMS;        // Core Capability Parameters
	volatile uint32_t HCSPPORTROUTE_Hi; // Core Companion Port Route Description
	volatile uint32_t HCSPPORTROUTE_Lo; // Core Companion Port Route Description
} __attribute__((packed)) ehci_CapRegs_t;

typedef struct
{
	volatile uint32_t USBCMD;           // USB Command                     Core  // +00h
	volatile uint32_t USBSTS;           // USB Status                      Core  // +04h
	volatile uint32_t USBINTR;          // USB Interrupt Enable            Core  // +08h
	volatile uint32_t FRINDEX;          // USB Frame Index                 Core  // +0Ch
	volatile uint32_t CTRLDSSEGMENT;    // 4G Segment Selector             Core  // +10h
	volatile uint32_t PERIODICLISTBASE; // Frame List Base Address         Core  // +14h
	volatile uint32_t ASYNCLISTADDR;    // Next Asynchronous List Address  Core  // +18h
	volatile uint32_t reserved1;                                                 // +1Ch
	volatile uint32_t reserved2;                                                 // +20h
	volatile uint32_t reserved3;                                                 // +24h
	volatile uint32_t reserved4;                                                 // +28h
	volatile uint32_t reserved5;                                                 // +2Ch
	volatile uint32_t reserved6;                                                 // +30h
	volatile uint32_t reserved7;                                                 // +34h
	volatile uint32_t reserved8;                                                 // +38h
	volatile uint32_t reserved9;                                                 // +3Ch
	volatile uint32_t CONFIGFLAG;       // Configure Flag Register         Aux   // +40h
	volatile uint32_t PORTSC[16];       // Port Status/Control             Aux   // +44h
} __attribute__((packed))  ehci_OpRegs_t;


void findEhci(void);
