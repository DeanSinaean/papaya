#include "io.h"
#include "ehci.h"

ehci_t ehci[MAX_EHCIS];
//we don't have malloc yet, so...
struct ehci_port_t ports[16];//TODO:what is the maximum number?

void ehciIRQHandler(void)
{
	int i;
	for(i=0;i<MAX_EHCIS;i++)
	{
		ehci_t e=echi[i];
		uint32_t intr=cfgIORead32(e.ioBase,USB_STS);
		if (intr!=0) {
				oprintf("USB_INT on ehci %d\n",i);
			if(intr & BIT(USB_INT)){
				oprintf("USB_INT 0\n");
				e.transComplete=1;
			}
			if(intr & BIT(USB_ERR_INT)){
				oprintf("USB_ERR_INT 1\n");
			}
			if(intr & BIT(USB_PORT_CHANGE_INT)){
				oprintf("USB_INT 2\n");
			}
			if(intr & BIT(USB_FRLIST_RO_INT)){
				oprintf("USB_INT 3\n");
			}
			if(intr & BIT(USB_HS_ERR_INT)){
				oprintf("USB_INT 4\n");
			}
			if(intr & BIT(USB_ASYNC_ADVANCE_INT)){
				oprintf("USB_INT 5\n");
			}

		}
	}
}

uint8_t getIRQ(uint32_t regBase)
{
	return (uint8_t)(cfgIORead32(rebBase,0x3C)&0xff);
}

void findEhci(void)
{
	unsigned int bus,device,function;
	uint32_t USBBASE,HCCR,EECP,USBLEGSUP,USBLEGCTLSTS;
	int i;
	for(i=0;i<MAX_EHCIS;i++) {
		ehci[i].ioBase=0;// initialize ehci list
	}
	i=0;
	for(bus=0;bus<0xff;bus++)
		for(device=0;device<0x1f;device++)
			for(function=0;function<8;function++)
			{
				uint32_t regBase=0x80000000 | (bus<<16) | (device<<11) | (function<<8) ;
				outl(regBase,0xcf8);
				value=inl(0xcfc);
				if( value !=0xffffffff) {
					outl(regBase|CFG_CLASSCODE,0xcf8);
					value=inl(0xcfc);
					if ((value & 0x0C032000)!=0x0c032000) {
						//this is not an ehci device.
					}else {
						ehci[i].ioBase=regBase;
						ehci[i].irq=getIRQ(regBase);
						ehci[i].mmBase=cfgIORead32(regBase,CFG_MM_BASE);
						ehci[i].OpRegs=(void *)(ehci[i].mmBase)+*(uint8_t *)ehci[i].mmBase;
						ehci[i].CapRegs=mmBase;
						i++;
					}
				}
			}
}

void step1_EhciInit(ehci * e)
{
	uint32_t cmd=cfgIORead32(e->ioBase,0x04);
	cfgIOWrite32(e->ioBase,0x04,cmd | 0x02 |0x04); //enable mmbase, enable bus master
	//TODO:install irq handler, now fixed at 27 and 2b which may only suite my case
}

void step2_resetEhci(ehci *e)
{
#define MMREG(x,y)  (*(uint32_t *)(x+y)) 
	MMREG(e->mmBase+USB_CMD)&=~(1<<CMD_RUNSTOP);//stop processing
	//wait stopped
	while(!(MMREG(e->mmBase+USB_STS) & (1<<STS_HCHalted))) {
		//set a timer to finish this
	}//TODO:wait a while before check it again
	MMREG(e->mmBase+USB_CMD) |= (1<<CMD_HCRESET);//reset
	while( MMREG(e->mmBase+USB_CMD) & CMD_HCRESET != 0) {
		//CMD_HCRESET will be set to 0 if reset operation is finished.
		//TODO:wait a while before check it again
		//set a timer to finish this
	}
}

void step3_requestSemaphore(ehci *e)
{
	uint8_t eecp=(e->CapRegs->HCCPARAMS>>8) && 0xFF;
	if(eecp >=0x40)  {//there is a valid eecp
		uint8_t eecp_id=0;
		while(eecp) {
			eecp_id=cfgIORead32(e->ioBase,eecp) & 0xFF;//first byte is eecp_id
			if(eecp_id==0x01) break;//got it
			eecp=(cfgIORead32(e->ioBase,eecp)>>8)&0xFF;//second byte indicates another eecp;
		}
		uint32_t USBLEGSUP=cfgIORead32(e->ioBase,eecp);
		if( eecp_id==0x01 && (USBLEGSUP & (1<<16))) {// BIOS owns semaphore
			cfgIOWrite32(e->ioBase, eecp, USBLEGSUP | (1<<24));//OS request to have semaphore
		 	while( cfgIORead32(e->ioBase,eecp) & (1<<16)) {//wait while BIOS still holds semaphore 	
				//TODO: sleep some milliseconds
			}
			while(!(cfgIORead32(e->ioBase,eecp) & (1<<24))) {//wait while OS hasn't got the semaphore
				//TODO: sleep some milliseconds
			}
			cfgIOWrite32(e->ioBase,eecp+4,0);//try to disable SMI
		}else{
			oprintf("BIOS does own the EHCI, No action taken.\n");
		}
	}else{
		oprintf("No valid eecp found.\n");
	}
}

void step4_startEHCI(ehci_t * e)
{
	e->OpRegs->CTRLDSSEGMENT=0;
	e->OpRegs->USBINTR=STS_ASYNC_INT|STS_HOST_SYSTEM_ERROR|STS_PORT_CHANGE|STS_USBERRINT|STS_USBINT;
	e->OpRegs->USBCMD|=CMD_8_MICROFRAME;
	if( e->OpRegs->USBSTS & STS_HCHALTED)
	{
		e->OpRegs->USBCMD|=CMD_RUN_STOP;
	}
	e->OpRegs->CONFIGFLAG=CF;
	//TODO: sleep 100 ms
}

void ehci_enablePorts(ehci_t * e)
{
	e->ports=ports;
	for(uint8_t j=0;j<e->numPorts;j++)
	{
		e->ports[j].ehci=e;
		e->ports[j].port.type=&USB_EHCI;
		e->ports[j].port.data=e->ports+j;
		e->ports[j].port.insertedDisk=0;
		attachPort(&e->ports[j].port);
		ehci_initializeAsyncScheduler(e);
	}
	e->enabledPortFlag=true;
	for(uint8_t j=0;j<e->numPorts;j++)
	{
//		ehci_checkPortLineStatus(e,j);
	}
}

										
