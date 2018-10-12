#ifndef _DFE_PORTDEFINE_H
#define _DFE_PORTDEFINE_H

#define DFE_PORT_PARA_DECIMATOR_NCOFREQ_H									0x1082
#define DFE_PORT_PARA_DECIMATOR_NCOFREQ_L									0x1083
#define DFE_PORT_PARA_DAGC_GOAL														0x1086
#define DFE_PORT_PARA_DDC_NCOFREQ_H												0x108A
#define DFE_PORT_PARA_DDC_NCOFREQ_L												0x108B
#define DFE_PORT_PARA_AD_IQFLAG														0x108C
#define DFE_PORT_PARA_RESET																0x1098
#define DFE_PORT_PARA_TESTIO															0x1099
#define DFE_PORT_PARA_RECONLY_AUTOMODE_DAGCINTERVAL				0x109F
#define DFE_PORT_PARA_RECONLY_AUTOMODE_DDCFREQDELTA				0x10A0
#define DFE_PORT_PARA_BASEBAND_IQFLAG											0x10A1
#define DFE_PORT_DEC_FREQ_DELTA														0x10A2
#define DFE_PORT_PARA_DAGC_MAXGAINDELTA_FLAG							0x10A3  //'1' Active
#define DFE_PORT_PARA_DAGC_MAXGAINDELTA										0x10A4 
#define DFE_PORT_PARA_CONSTANTDAGC												0x10A7
#define DFE_PORT_PARA_RF_PLL480_REG3_AD0									0x1096
#define DFE_PORT_PARA_RF_PLL280_REG3_AD0									0x1097
#define DFE_PORT_PARA_RF_PLL480_REG3_AD1									0x10A8
#define DFE_PORT_PARA_RF_PLL280_REG3_AD1									0x10A9
#define DFE_PORT_PARA_ADSEL																0x10AA 
/*
	-- '0' : AD0; '1' AD1 
	-- Note : if sel AD0, PORT_PARA_RF_PLL480_REG3_AD1 and PORT_PARA_RF_PLL280_REG3_AD1 are invalid 
	-- Note : if sel AD1, PORT_PARA_RF_PLL480_REG3_AD0 and PORT_PARA_RF_PLL280_REG3_AD0 are invalid 
	-- Therefore, PORT_PARA_ADSEL should be set first 
*/
#define DFE_PORT_PARA_RECSTART														0x10B1
#define DFE_PORT_PARA_HBF16_VALID													0x10C0

/* Public */
#define DFE_PORT_PARA_AAGC_GOAL														0x1084
#define DFE_PORT_PARA_AAGC_CTRLD_MAX_MIN									0x1085
#define DFE_PORT_PARA_RECONLY_AUTOMODE_AAGCINTERVAL				0x10A5
#define DFE_PORT_PARA_AAFC																0x10A6


#endif
