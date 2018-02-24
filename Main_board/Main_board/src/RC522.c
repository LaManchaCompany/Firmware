/*
 * RC522.c
 *
 * Created: 12-09-16 19:36:41
 *  Author: Jérémie
 */ 

#include <avr/io.h>

#include "RC522.h"
#include "Soft_SPI.h"

//command set
#define Idle_CMD 				0x00
#define Mem_CMD					0x01
#define GenerateRandomId_CMD	0x02
#define CalcCRC_CMD				0x03
#define Transmit_CMD			0x04
#define NoCmdChange_CMD			0x07
#define Receive_CMD				0x08
#define Transceive_CMD			0x0C
#define Reserved_CMD			0x0D
#define MFAuthent_CMD			0x0E
#define SoftReset_CMD			0x0F

//Page 0 ==> Command and Status
#define Page0_Reserved_1 	0x00
#define CommandReg			0x01
#define ComIEnReg			0x02
#define DivIEnReg			0x03
#define ComIrqReg			0x04
#define DivIrqReg			0x05
#define ErrorReg			0x06
#define Status1Reg			0x07
#define Status2Reg			0x08
#define FIFODataReg			0x09
#define FIFOLevelReg		0x0A
#define WaterLevelReg		0x0B
#define ControlReg			0x0C
#define BitFramingReg		0x0D
#define CollReg				0x0E
#define Page0_Reserved_2	0x0F

//Page 1 ==> Command
#define Page1_Reserved_1	0x10
#define ModeReg				0x11
#define TxModeReg			0x12
#define RxModeReg			0x13
#define TxControlReg		0x14
#define TxASKReg			0x15
#define TxSelReg			0x16
#define RxSelReg			0x17
#define RxThresholdReg		0x18
#define	DemodReg			0x19
#define Page1_Reserved_2	0x1A
#define Page1_Reserved_3	0x1B
#define MfTxReg				0x1C
#define MfRxReg				0x1D
#define Page1_Reserved_4	0x1E
#define SerialSpeedReg		0x1F

//Page 2 ==> CFG
#define Page2_Reserved_1	0x20
#define CRCResultReg_1		0x21
#define CRCResultReg_2		0x22
#define Page2_Reserved_2	0x23
#define ModWidthReg			0x24
#define Page2_Reserved_3	0x25
#define RFCfgReg			0x26
#define GsNReg				0x27
#define CWGsPReg			0x28
#define ModGsPReg			0x29
#define TModeReg			0x2A
#define TPrescalerReg		0x2B
#define TReloadReg_1		0x2C
#define TReloadReg_2		0x2D
#define TCounterValReg_1	0x2E
#define TCounterValReg_2 	0x2F

//Page 3 ==> TestRegister
#define Page3_Reserved_1 	0x30
#define TestSel1Reg			0x31
#define TestSel2Reg			0x32
#define TestPinEnReg		0x33
#define TestPinValueReg		0x34
#define TestBusReg			0x35
#define AutoTestReg			0x36
#define VersionReg			0x37
#define AnalogTestReg		0x38
#define TestDAC1Reg			0x39
#define TestDAC2Reg			0x3A
#define TestADCReg			0x3B
#define Page3_Reserved_2 	0x3C
#define Page3_Reserved_3	0x3D
#define Page3_Reserved_4	0x3E
#define Page3_Reserved_5	0x3F

void ClearBitMask(uint8_t reg,uint8_t mask);
void WriteRawRC(uint8_t Address, uint8_t value);
void SetBitMask(uint8_t reg,uint8_t mask);
char PcdComMF522(uint8_t Command,
uint8_t *pInData,
uint8_t InLenByte,
uint8_t *pOutData,
unsigned int  *pOutLenBit);
void CalulateCRC(uint8_t *pIndata,uint8_t len,uint8_t *pOutData);
uint8_t ReadRawRC(uint8_t Address);
void MFRC522_AntennaOn(void);



/*********************************************************************
    * Function:        char MFRC522_Request(uint8_t req_code,uint8_t *pTagType)
    *
    * PreCondition:     SPI has been configured 
    *
    * Input:		    uint8_t req_code - 
    *               uint8_t *pTagType - 
    *
    * Output:		    char - return MI_OK if success
    *               pTagType - return card type 
    *                                           0x4400 = Mifare_UltraLight       
    *                                           0x0400 = Mifare_One(S50)
    *                                           0x0200 = Mifare_One(S70)
    *                                           0x0800 = Mifare_Pro(X)
    *                                           0x4403 = Mifare_DESFire
    *    
    * Side Effects:	    none
    *
    * Overview:		    This function search card and return card types
    *
    * Note:			    None
    ********************************************************************/
char MFRC522_Request(uint8_t req_code,uint8_t *pTagType)
{
	char status;  
	unsigned int unLen;
	uint8_t ucComMF522Buf[MAXRLEN]; 

	ClearBitMask(Status2Reg,0x08);
	WriteRawRC(BitFramingReg,0x07);
	SetBitMask(TxControlReg,0x03);
 
	ucComMF522Buf[0] = req_code;

	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);

	if ((status == MI_OK) && (unLen == 0x10))
	{    
		*pTagType     = ucComMF522Buf[0];
		*(pTagType+1) = ucComMF522Buf[1];
	}
	else
	{   status = MI_ERR;   }
   
	return status;
}
 
  /*********************************************************************
    * Function:        char MFRC522_Anticoll(uint8_t *pSnr)
    *
    * PreCondition:     none 
    *
    * Input:		    uint8_t *pSnr  
    *
    * Output:		    return MI_OK if success
    *               return the 4 bytes serial number     
    *    
    * Side Effects:	    none
    *
    * Overview:		    This function prevent conflict and return the 4 bytes serial number
    *
    * Note:			    None
    ********************************************************************/
char MFRC522_Anticoll(uint8_t *pSnr)
{
    char status;
    uint8_t i,snr_check=0;
    unsigned int unLen;
	
    uint8_t ucComMF522Buf[MAXRLEN]; 
    

    ClearBitMask(Status2Reg,0x08);
    WriteRawRC(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
         }
         if (snr_check != ucComMF522Buf[i])
         {   status = MI_ERR;    }
    }
    
    SetBitMask(CollReg,0x80);
    return status;
}

/*********************************************************************
    * Function:       char MFRC522_Select(uint8_t *pSnr)
    *
    * PreCondition:     SPI has been configured 
    *
    * Input:		    uint8_t *pSnr
    *
    * Output:		    char - return MI_OK if success
    *               
    *    
    * Side Effects:	    none
    *
    * Overview:		    
    *
    * Note:			    None
    ********************************************************************/
char MFRC522_Select(uint8_t *pSnr)
{
    char status;
    uint8_t i;
    unsigned int unLen;
    uint8_t ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }
    else
    {   status = MI_ERR;    }

    return status;
}

 /*********************************************************************
    * Function:   char MFRC522_AuthState(uint8_t auth_mode,uint8_t addr,uint8_t *pKey,uint8_t *pSnr)
    *
    * PreCondition:     none 
    *
    * Input:		    uint8_t auth_mode -   Password Authentication Mode
    *                                        0x60 = A key authentication
                                             0x61 = B key authentication    
    *               uint8_t addr  -      Block Address
    *               uint8_t *pKey  -     Sector Password
    *               uint8_t *pSnr  -    4 bytes serial number
    *
    * Output:		    char - return MI_OK if success
    *               
    *    
    * Side Effects:	    none
    *
    * Overview:		    This function verify card password
    *
    * Note:			    None
    ********************************************************************/             
char MFRC522_AuthState(uint8_t auth_mode,uint8_t addr,uint8_t *pKey,uint8_t *pSnr)
{
    char status;
    unsigned int unLen;
    uint8_t i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
    //memcpy(&ucComMF522Buf[2], pKey, 6); 
    //memcpy(&ucComMF522Buf[8], pSnr, 4); 
    
    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {   status = MI_ERR;   }
    
    return status;
}

 /*********************************************************************
    * Function:        char MFRC522_Read(uint8_t addr, uint8_t *pData)
    *
    * PreCondition:     none
    *
    * Input:		    uint8_t addr   - block address
    *               uint8_t *pData  - block data
    *
    * Output:		    char - return MI_OK if success
    *    
    * Side Effects:	    none
    *
    * Overview:		    This function read block data
    *
    * Note:			    None
    ********************************************************************/
char MFRC522_Read(uint8_t addr, uint8_t *pData)
{
    char  status;
    unsigned int unLen;
    uint8_t i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
   // {   memcpy(pData, ucComMF522Buf, 16);   }
    {
        for (i=0; i<16; i++)
        {    *(pData+i) = ucComMF522Buf[i];   }
    }
    else
    {   status = MI_ERR;   }
    
    return status;
}

 /*********************************************************************
    * Function:      char MFRC522_Write(uint8_t addr,uint8_t *pData)
    *
    * PreCondition:     SPI has been configured 
    *
    * Input:		    uint8_t addr  - block address
    *               uint8_t *pData  - data to write
    *
    * Output:		    char - return MI_OK if success
    *                                           
    *    
    * Side Effects:	    none
    *
    * Overview:		    This function write a block of data to addr
    *
    * Note:			    None
    ********************************************************************/               
char MFRC522_Write(uint8_t addr,uint8_t *pData)
{
    uint8_t status;
    unsigned int unLen;
    uint8_t i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
       // memcpy(ucComMF522Buf, pData, 16);
        for (i=0; i<16; i++)
        {    
        	ucComMF522Buf[i] = *(pData+i);   
        }
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    
    return status;
}

/*
 * Function：MFRC522_Halt
 * Description：Command the cards into sleep mode
 * Input parameters：null
 * return：MI_OK
 */
 /*********************************************************************
    * Function:       char MFRC522_Halt(void)
    *
    * PreCondition:     none
    *
    * Input:		    none 
    *
    * Output:		    none
    *    
    * Side Effects:	    none
    *
    * Overview:		    This function command the cards into sleep mode
    *
    * Note:			    None
    ********************************************************************/
char MFRC522_Halt(void)
{
    char status;
    unsigned int unLen;
    uint8_t ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    return MI_OK;
}

 /*********************************************************************
    * Function:   void CalulateCRC(uint8_t *pIndata,uint8_t len,uint8_t *pOutData)
    *
    * PreCondition:     none
    *
    * Input:		    uint8_t *pIndata - input datas
    *               uint8_t len       - data length
    *               uint8_t *pOutData  - output data
    *
    * Output:		    uint8_t - 2 bytes CRC result
    *    
    * Side Effects:	    none
    *
    * Overview:		    This function calculate the CRC
    *
    * Note:			    None
    ********************************************************************/
void CalulateCRC(uint8_t *pIndata,uint8_t len,uint8_t *pOutData)
{
    uint8_t i,n;
    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++)
    {   WriteRawRC(FIFODataReg, *(pIndata+i));   }
    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOutData[0] = ReadRawRC(CRCResultRegL);
    pOutData[1] = ReadRawRC(CRCResultRegM);
}

/*********************************************************************
    * Function:        char MFRC522_Reset(void)
    *
    * PreCondition:     none
    *
    * Input:		    none 
    *
    * Output:		    return MI_OK
    *    
    * Side Effects:	    reset the RC522
    *
    * Overview:		    This function reset the RC522
    *
    * Note:			    None
    ********************************************************************/
char MFRC522_Reset(void)
{
  	//SET_RC522RST;
    //delay_ns(10);
	  //CLR_RC522RST;
    //delay_ns(10);
	  //SET_RC522RST;
    //delay_ns(10);
    WriteRawRC(CommandReg,PCD_RESETPHASE);
	wait_SPI();
   // delay_ns(10);
    WriteRawRC(ModeReg,0x3D);           
    WriteRawRC(TReloadRegL,30);           
    WriteRawRC(TReloadRegH,0);
    WriteRawRC(TModeReg,0x8D);
    WriteRawRC(TPrescalerReg,0x3E);
	  WriteRawRC(TxAutoReg,0x40);
    return MI_OK;
}
   /*********************************************************************
    * Function:        char MFRC522_ConfigISOType(uint8_t type)
    *
    * PreCondition:     none
    *
    * Input:		    uint8_t type
    *
    * Output:		  return MI_OK if type == 'A' 
    *          
    * Side Effects:	    none
    *
    * Overview:		    This function configure the ISO type
    *
    * Note:			    None
    ********************************************************************/
char MFRC522_ConfigISOType(uint8_t type)
{
   if (type == 'A')                 
   { 
       ClearBitMask(Status2Reg,0x08);
       WriteRawRC(ModeReg,0x3D);
       WriteRawRC(RxSelReg,0x86);
       WriteRawRC(RFCfgReg,0x7F);
   	   WriteRawRC(TReloadRegL,30); 
	     WriteRawRC(TReloadRegH,0);
       WriteRawRC(TModeReg,0x8D);
	     WriteRawRC(TPrescalerReg,0x3E);
	    wait_SPI();
       MFRC522_AntennaOn();
   }
   else{ return -1; }
   
   return MI_OK;
}
   /*********************************************************************
    * Function:        unsigned char ReadRawRC(uint8_t Address)
    *
    * PreCondition:     SPI has been configured 
    *
    * Input:		    uint8_t Address - register address
    *
    * Output:		    return a byte of data read from the register
    *    
    * Side Effects:	    none
    *
    * Overview:		    This function read a byte of data register
    *
    * Note:			    None
    ********************************************************************/
unsigned char ReadRawRC(uint8_t Address)
{
    uint8_t ucAddr;
    uint8_t ucResult=0;
	////Soft_SPI_chip_enable();
	// CLR_SPI_CS;
    ucAddr = ((Address<<1)&0x7E)|0x80;
	
		wait_SPI();
		PORTC.OUTCLR = 0b00010000;
		wait_SPI();
	
	spi(ucAddr);
	ucResult = spi(0x00);
	
		 wait_SPI();
		 PORTC.OUTSET = 0b00010000;
		 wait_SPI();
	////Soft_SPI_transmit(ucAddr);
   //	SPIWriteByte(ucAddr);
	////ucResult=  Soft_SPI_transmit(0x00);
	  //ucResult=SPIReadByte();
	  //// Soft_SPI_chip_disable();
	 // SET_SPI_CS;
    return ucResult;
}

   /*********************************************************************
    * Function:        void WriteRawRC(uint8_t Address, uint8_t value)
    *
    * PreCondition:     none 
    *
    * Input:		    uint8_t Address  -   register address
    *               uint8_t value    -    the value to be written
    *
    * Output:		    none 
    *                                      
    *    
    * Side Effects:	    none
    *
    * Overview:		    This function write a byte into a given register
    *
    * Note:			    None
    ********************************************************************/
void WriteRawRC(uint8_t Address, uint8_t value)
{  
    uint8_t ucAddr;

	////Soft_SPI_chip_enable();
	 // CLR_SPI_CS;
    ucAddr = ((Address<<1)&0x7E);
	
	wait_SPI();
	PORTC.OUTCLR = 0b00010000;
	wait_SPI();

	 spi(ucAddr);
	 spi(value);
	 
	 
	 wait_SPI();
	 PORTC.OUTSET = 0b00010000;
	 wait_SPI();
	 ////Soft_SPI_transmit(ucAddr);
	//// Soft_SPI_transmit(value);
	 // SPIWriteByte(ucAddr);
	  //SPIWriteByte(value);
	  //SET_SPI_CS;
	  ////Soft_SPI_chip_disable();
}

   /*********************************************************************
    * Function:        void SetBitMask(uint8_t reg,uint8_t mask) 
    *
    * PreCondition:     none
    *
    * Input:		   uint8_t reg  - register address 
    *              uint8_t mask - bit mask
    *
    * Output:		    none
    *    
    * Side Effects:	    none
    *
    * Overview:		    This function set a bit mask on RC522 register
    *
    * Note:			    None
    ********************************************************************/
void SetBitMask(uint8_t reg,uint8_t mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg,tmp | mask);  // set bit mask
}

   /*********************************************************************
    * Function:        void ClearBitMask(uint8_t reg,uint8_t mask)
    *
    * PreCondition:     none
    *
    * Input:		    uint8_t reg - register address
    *               uint8_t mask - set value
    *
    * Output:		    none
    *    
    * Side Effects:	    none
    *
    * Overview:		    This function clear a bit mask on RC522 register
    *
    * Note:			    None
    ********************************************************************/
void ClearBitMask(uint8_t reg,uint8_t mask)  
{
    char  tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp & ~mask);  // clear bit mask
} 

   /*********************************************************************
    * Function:        char PcdComMF522(uint8_t Command, 
    *                                    uint8_t *pInData, 
    *                                     uint8_t InLenByte, 
    *                                      uint8_t *pOutData, 
    *                                       unsigned int *pOutLenBit)
    *
    * PreCondition:     SPI has been configured 
    *
    * Input:		    uint8_t Command   -  command type
    *               uint8_t *pInData  -  input data
    *               uint8_t InLenByte -  input data length
    *               uint8_t *pOutData -  output Data
    *               unsigned int *pOutLenBit -  output data length
    *
    * Output:		    char - return MI_OK if success
    *               
    *    
    * Side Effects:	    none
    *
    * Overview:		    This function search card and return card types
    *
    * Note:			    None
    ********************************************************************/
char PcdComMF522(uint8_t Command, 
                 uint8_t *pInData, 
                 uint8_t InLenByte,
                 uint8_t *pOutData, 
                 unsigned int *pOutLenBit)
{
    char status = MI_ERR;
    uint8_t irqEn   = 0x00;
    uint8_t waitFor = 0x00;
    uint8_t lastBits;
    uint8_t n;
    unsigned int i;
    switch (Command)
    {
        case PCD_AUTHENT:
			irqEn   = 0x12;
			waitFor = 0x10;
			break;
		case PCD_TRANSCEIVE:
			irqEn   = 0x77;
			waitFor = 0x30;
			break;
		default:
			break;
    }
   
    WriteRawRC(ComIEnReg,irqEn|0x80);
    ClearBitMask(ComIrqReg,0x80);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    
    for (i=0; i<InLenByte; i++)
    {   WriteRawRC(FIFODataReg, pInData[i]);    }
    WriteRawRC(CommandReg, Command);
   
    
    if (Command == PCD_TRANSCEIVE)
    {    SetBitMask(BitFramingReg,0x80);  }
    
	i = 2000;
    do 
    {
        n = ReadRawRC(ComIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor));
    ClearBitMask(BitFramingReg,0x80);

    if (i!=0)
    {    
        if(!(ReadRawRC(ErrorReg)&0x1B))
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {   status = MI_NOTAGERR;   }
            if (Command == PCD_TRANSCEIVE)
            {
               	n = ReadRawRC(FIFOLevelReg);
              	lastBits = ReadRawRC(ControlReg) & 0x07;
                if (lastBits)
                {   *pOutLenBit = (n-1)*8 + lastBits;   }
                else
                {   *pOutLenBit = n*8;   }
                if (n == 0)
                {   n = 1;    }
                if (n > MAXRLEN)
                {   n = MAXRLEN;   }
                for (i=0; i<n; i++)
                {   pOutData[i] = ReadRawRC(FIFODataReg);    }
            }
        }
        else
        {   status = MI_ERR;   }
        
    }
    SetBitMask(ControlReg,0x80);           // stop timer now
    WriteRawRC(CommandReg,PCD_IDLE); 
    return status;
}

/*********************************************************************
    * Function:        void MFRC522_AntennaOn(void)
    *
    * PreCondition:     none
    *
    * Input:		    none
    *
    * Output:		    none
    *    
    * Side Effects:	    Antenna On
    *
    * Overview:	  	This function command the RC522 to switch on the antenna
    *
    * Note:			    None
    ********************************************************************/
void MFRC522_AntennaOn(void)
{
    uint8_t i;
    i = ReadRawRC(TxControlReg);
    if (!(i & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}

/*********************************************************************
    * Function:        void MFRC522_AntennaOff(void)
    *
    * PreCondition:     none 
    *
    * Input:		   none
    *
    * Output:		    none
    *    
    * Side Effects:	  Antenna Off
    *
    * Overview:		 This function command the RC522 to switch off the antenna
    *
    * Note:			    None
    ********************************************************************/
void MFRC522_AntennaOff(void)
{
	ClearBitMask(TxControlReg, 0x03);
}

void RFID_Init(void)
{
	MFRC522_Reset();
	MFRC522_AntennaOff();
	MFRC522_AntennaOn();
	MFRC522_ConfigISOType( 'A' );
}
