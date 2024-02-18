#pragma once
#ifndef shReg_pg_h
#define shReg_pg_h

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)  || defined(__AVR_ATmega32U4__) || defined(__SAMD21__) //
	#include "pg_fastdio.h"
	#define digitalWF(a,b) digitalWriteFast(a,b)
	#define digitalRF(a) digitalReadFast(a)
#elif defined(_VARIANT_ARDUINO_STM32_) || defined(ARDUINO_ARCH_STM32)
	#define digitalWF(a,b) digitalWriteFast(digitalPinToPinName(a),b)
	#define digitalRF(a) digitalReadFast(digitalPinToPinName(a))
#endif
 //WIP object oriented
/*
class shiftReg
{
	public:
		shiftReg(bool type, uint8_t datap, uint8_t clockp, uint8_t latchp, uint8_t rDir);
		shiftReg(bool type, uint8_t datap, uint8_t clockp, uint8_t latchp, uint8_t rCount, uint8_t rDir);
		shiftReg(bool type, uint8_t datap[], uint8_t clockp, uint8_t latchp, uint8_t rCount, uint8_t parallel, uint8_t rDir);
		~shiftReg();
		
		void setDirection(uint8_t dir)	{this->regDirection = (dir==MSBFIRST)?MSBFIRST:LSBFIRST;}
		
		void setData(uint8_t dataIn)	{this->data8 = dataIn;}
		void setData(uint16_t dataIn)	{this->data16 = dataIn;}
		void setData(uint32_t dataIn)	{this->data32 = dataIn;}
		void setData(uint64_t dataIn)	{this->data64 = dataIn;}
		
		void addData(uint8_t bit);
		
		bool Out(uint8_t ds, int8_t bitOrder=-1);			// ++	//8b output
		bool Out(uint8_t ds[], int8_t bitOrder=-1);			// +	// rCount * 8b output
		bool Out(uint8_t **ds, int8_t bitOrder=-1);			//		// 
		bool Out16(uint16_t ds, int8_t bitOrder=-1);			//		//16b, rCount >=2
		bool Out32(uint32_t ds, int8_t bitOrder=-1);			//		//32b, rCount >=3
		bool Out64(uint64_t ds, int8_t bitOrder=-1);			//		//64b, rCount >4
		
		bool In(uint8_t &ds, int8_t bitOrder=-1);			// ++	//8b input
		bool In(uint8_t ds[], int8_t bitOrder=-1);			//		// rCount * 8b input
		bool In(uint8_t **ds, int8_t bitOrder=-1);			//		// 
		bool In16(uint16_t &ds, int8_t bitOrder=-1);			//		//16b, rCount >=2
		bool In32(uint32_t &ds, int8_t bitOrder=-1);			//		//32b, rCount >=3
		bool In64(uint64_t &ds, int8_t bitOrder=-1);			//		//64b, rCount >4
		
	private:
		uint8_t data8;
		uint16_t data16;
		uint32_t data32;
		uint64_t data64;
		
		uint8_t clkPin;
		uint8_t latchPin;
		uint8_t dataPin;
		uint8_t *dataPins;
		uint8_t regCount=1;	// registers in series
		uint8_t parallelLines=1;	// parallel lines
		bool regType=0; // 0= Parallel In Serial Out (input, eg. 74HC165), 1= Serial In Parallel Out (output, eg. 74HC595)
		bool regDirection=LSBFIRST;	//LSBFIRST / MSBFIRST
};
shiftReg::shiftReg(bool type, uint8_t datap, uint8_t clockp, uint8_t latchp, uint8_t rDir){
	this->regType = type;
	this->dataPin=datap;
	this->clkPin = clockp;
	this->latchPin = latchp;
	this->regDirection = rDir;
	
	pinMode(this->dataPin,this->regType);
	pinMode(this->clkPin,OUTPUT);
	pinMode(this->latchPin,OUTPUT);
}
shiftReg::shiftReg(bool type, uint8_t datap, uint8_t clockp, uint8_t latchp, uint8_t rCount, uint8_t rDir){
	this->regType = type;
	this->dataPin=datap;
	this->clkPin = clockp;
	this->latchPin = latchp;
	this->regCount = rCount;
	this->regDirection = rDir;
	
	pinMode(this->dataPin,this->regType);
	pinMode(this->clkPin,OUTPUT);
	pinMode(this->latchPin,OUTPUT);
}
shiftReg::shiftReg(bool type, uint8_t datap[], uint8_t clockp, uint8_t latchp, uint8_t rCount, uint8_t parallel, uint8_t rDir){
	this->regType = type;
	this->dataPins = new uint8_t[parallel];
	this->clkPin = clockp;
	this->latchPin = latchp;
	this->regCount = rCount;
	this->parallelLines = parallel;
	this->regDirection = rDir;

	for(uint8_t i=0;i<parallel;i++)
	{
		this->dataPins[i]=datap[i];
		pinMode(this->dataPins[i],this->regType);
	}
	pinMode(this->clkPin,OUTPUT);
	pinMode(this->latchPin,OUTPUT);
}
shiftReg::~shiftReg(){
	if(this->dataPins)
		delete dataPins;
}
//---------------------------------------------------------------------
void shiftReg::addData(uint8_t bit)
{
	if(this->regDirection == MSBFIRST)
	{
		
	}
	else
	{
	}
}
//---------------------------------------------------------------------
bool shiftReg::Out(uint8_t ds, int8_t bitOrder){
	if(this->regType == INPUT)
		return 0;
	
	digitalWF(this->latchPin,0);
	for(uint8_t i=0;i<8;i++)
	{
		if(bitOrder==LSBFIRST)
		{
			digitalWF(this->dataPin,ds&1);
			ds=ds>>1;
		}else
		{
			digitalWF(this->dataPin,ds&B10000000);
			ds=ds<<1;
		}
		digitalWF(this->clkPin,1);
		digitalWF(this->clkPin,0);	
	}	
	digitalWF(this->latchPin,1);
	return 1;
}
//---------------------------------------------------------------------
bool shiftReg::Out(uint8_t ds[], int8_t bitOrder){
	if(this->regType == INPUT)
		return 0;
	uint8_t data=0;
	digitalWF(this->latchPin,0);
	for(uint8_t arr=0;arr<regCount;arr++)
	{
		data=ds[arr];
		for(uint8_t i=0;i<8;i++)
		{
			if(bitOrder==LSBFIRST)
			{
				digitalWF(this->dataPin,data&1);
				data=data>>1;
			}else
			{
				digitalWF(this->dataPin,data&B10000000);
				data=data<<1;
			}
			digitalWF(this->clkPin,1);
			digitalWF(this->clkPin,0);
		}	
	}
	digitalWF(this->latchPin,1);
	return 1;
}
//---------------------------------------------------------------------
bool shiftReg::Out16(uint16_t ds, int8_t bitOrder){
	if(this->regType == INPUT)
		return 0;
	
	digitalWF(this->latchPin,0);
	for(uint8_t i=0;i<16;i++)
	{
		if(bitOrder==LSBFIRST)
		{
			digitalWF(this->dataPin,ds&1);
			ds=ds>>1;
		}else
		{
			digitalWF(this->dataPin,ds&0x8000);
			ds=ds<<1;
		}
		digitalWF(this->clkPin,1);
		digitalWF(this->clkPin,0);	
	}	
	digitalWF(this->latchPin,1);
	return 1;
}
//---------------------------------------------------------------------
bool shiftReg::Out32(uint32_t ds, int8_t bitOrder){
	if(this->regType == INPUT)
		return 0;

	if(regCount==0)
		regCount=4;
	else if(regCount>4)
		return 0;
	

	if(regCount<4 && bitOrder == MSBFIRST)
		ds=ds<<(32-regCount*8);
	
	digitalWF(this->latchPin,0);
	for(uint8_t i=0;i<8*regCount;i++)
	{
		if(bitOrder==LSBFIRST)
		{
			digitalWF(this->dataPin,ds&1);
			ds=ds>>1;
		}else
		{
			digitalWF(this->dataPin,ds&0x80000000);
			ds=ds<<1;
		}
		digitalWF(this->clkPin,1);
		digitalWF(this->clkPin,0);	
	}	
	digitalWF(this->latchPin,1);
	return 1;
}
//---------------------------------------------------------------------
bool shiftReg::Out64(uint64_t ds, int8_t bitOrder){
	if(this->regType == INPUT)
		return 0;
	if(regCount==0)
		regCount=8;
	else if(regCount>8)
		return 0;

	if(regCount<8 && bitOrder == MSBFIRST)
		ds=ds<<(64-regCount*8);
	
	digitalWF(this->latchPin,0);
	for(uint8_t i=0;i<8*regCount;i++)
	{
		if(bitOrder==LSBFIRST)
		{
			digitalWF(this->dataPin,ds&1);
			ds=ds>>1;
		}else
		{
			digitalWF(this->dataPin,ds&0x8000000000000000);
			ds=ds<<1;
		}
		digitalWF(this->clkPin,1);
		digitalWF(this->clkPin,0);	
	}	
	digitalWF(this->latchPin,1);
	return 1;
}
//---------------------------------------------------------------------
bool shiftReg::In(uint8_t &ds, int8_t bitOrder){
	if(this->regType == OUTPUT)
		return 0;
	
	digitalWF(this->latchPin,1);
	for(uint8_t i=0;i<8;i++)
    {
		if(bitOrder==LSBFIRST)
		{
			ds=ds>>1;
			if(digitalRF(this->dataPin))
				ds|= B10000000;
		}
		else
		{
			ds=ds<<1;
			ds|= digitalRF(this->dataPin);
		}
		digitalWF(this->clkPin,1);
#ifdef ARDUINO_ARCH_STM32
		digitalWF(this->clkPin,1);
#endif
		digitalWF(this->clkPin,0);			
    }
	digitalWF(this->latchPin,0);
	return 1;
}
//---------------------------------------------------------------------
bool shiftReg::In(uint8_t ds[], int8_t bitOrder){
	if(this->regType == OUTPUT)
		return 0;
	
	digitalWF(this->latchPin,1);
	for(uint8_t arr=0;arr<regCount;arr++)
	{
//		uint8_t data=ds[arr];
		for(uint8_t i=0;i<8;i++)
		{
			if(bitOrder==LSBFIRST)
			{
				ds[arr]=ds[arr]>>1;
				if(digitalRF(this->dataPin))
					ds[arr]|= B10000000;
			}
			else
			{
				ds[arr]=ds[arr]<<1;
				ds[arr]|= digitalRF(this->dataPin);
			}
			digitalWF(this->clkPin,1);
#ifdef ARDUINO_ARCH_STM32
			digitalWF(this->clkPin,1);
#endif
			digitalWF(this->clkPin,0);			
		}
	}
	digitalWF(this->latchPin,0);
	return 1;
}
//---------------------------------------------------------------------
bool shiftReg::In16(uint16_t &ds, int8_t bitOrder){
	if(this->regType == OUTPUT)
		return 0;
	
	digitalWF(this->latchPin,1);
	for(uint8_t i=0;i<16;i++)
    {
		if(bitOrder==LSBFIRST)
		{
			ds=ds>>1;
			if(digitalRF(this->dataPin))
				ds|= 0x8000;
		}
		else
		{
			ds=ds<<1;
			ds|= digitalRF(this->dataPin);
		}
		digitalWF(this->clkPin,1);
#ifdef ARDUINO_ARCH_STM32
		digitalWF(this->clkPin,1);
#endif
		digitalWF(this->clkPin,0);			
    }
	digitalWF(this->latchPin,0);
	return 1;
}
//---------------------------------------------------------------------
bool shiftReg::In32(uint32_t &ds, int8_t bitOrder){
	if(this->regType == OUTPUT)
		return 0;
	
	digitalWF(this->latchPin,1);
	for(uint8_t i=0;i<16;i++)
    {
		if(bitOrder==LSBFIRST)
		{
			ds=ds>>1;
			if(digitalRF(this->dataPin))
				ds|= 0x8000;
		}
		else
		{
			ds=ds<<1;
			ds|= digitalRF(this->dataPin);
		}
		digitalWF(this->clkPin,1);
#ifdef ARDUINO_ARCH_STM32
		digitalWF(this->clkPin,1);
#endif
		digitalWF(this->clkPin,0);			
    }
	digitalWF(this->latchPin,0);
	return 1;
}
//---------------------------------------------------------------------
bool shiftReg::In64(uint64_t &ds, int8_t bitOrder){
	if(this->regType == OUTPUT)
		return 0;
	
	digitalWF(this->latchPin,1);
	for(uint8_t i=0;i<16;i++)
    {
		if(bitOrder==LSBFIRST)
		{
			ds=ds>>1;
			if(digitalRF(this->dataPin))
				ds|= 0x8000;
		}
		else
		{
			ds=ds<<1;
			ds|= digitalRF(this->dataPin);
		}
		digitalWF(this->clkPin,1);
#ifdef ARDUINO_ARCH_STM32
		digitalWF(this->clkPin,1);
#endif
		digitalWF(this->clkPin,0);			
    }
	digitalWF(this->latchPin,0);
	return 1;
}
*/
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
static inline void regOutStart(uint8_t latchPin){
	digitalWF(latchPin,0);
}
//------------------------------------------------------------------------------------
static inline void regOutEnd(uint8_t latchPin){
	digitalWF(latchPin,1);
}
//------------------------------------------------------------------------------------
static inline void regInStart(uint8_t latchPin){
	digitalWF(latchPin,1);
}
//------------------------------------------------------------------------------------
static inline void regInEnd(uint8_t latchPin){
	digitalWF(latchPin,0);
}
//------------------------------------------------------------------------------------
static inline void regOut8(uint8_t dPin,uint8_t sPin, int8_t bOrder, uint8_t data){
	for(uint8_t i=0;i<8;i++)
	{
		if(bOrder==LSBFIRST)		{
			digitalWF(dPin,data&1);
			data=data>>1;
		}else		{
			digitalWF(dPin,data&0x80);
			data=data<<1;
		}
		digitalWF(sPin,1);
#ifdef ARDUINO_ARCH_STM32
//		__NOP();
		digitalWF(sPin,1);
#endif
		digitalWF(sPin,0);
	}
}
//------------------------------------------------------------------------------------
static inline uint8_t regIn8(uint8_t dPin,uint8_t sPin, int8_t bOrder){
	uint8_t ret;
	for(uint8_t i=0;i<8;i++)
    {
		if(bOrder==LSBFIRST)
		{
			ret=ret>>1;
			if(digitalRF(dPin))
				ret|= B10000000;
		}
		else
		{
			ret=ret<<1;
				ret|= digitalRF(dPin);
		}
		digitalWF(sPin,1);
#ifdef ARDUINO_ARCH_STM32
		digitalWF(sPin,1);
//		__NOP();
#endif
		digitalWF(sPin,0);
    }
	return ret;
}
//*************************************************    16    *****************************************************
static inline void regOut16(uint8_t dPin,uint8_t sPin, int8_t bOrder, uint16_t data){
	for(uint8_t i=0;i<16;i++)
	{
		if(bOrder==LSBFIRST)
		{
			digitalWF(dPin,data&1);
			data=data>>1;
		}else
		{
			digitalWF(dPin,data & 0x8000);
			data=data<<1;
		}
		digitalWF(sPin,1);
		digitalWF(sPin,0);
	}
}
//------------------------------------------------------------------------------------
static inline uint16_t regIn16(uint8_t dPin,uint8_t sPin, int8_t bOrder){
	uint16_t ret;
	for(uint8_t i=0;i<16;i++)
    {
		if(bOrder==LSBFIRST)
		{
			ret=ret>>1;
			if(digitalRF(dPin))
				ret|= 0x8000;
		}
		else
		{
			ret=ret<<1;
				ret|= digitalRF(dPin);
		}
		digitalWF(sPin,1);
#ifdef ARDUINO_ARCH_STM32
		digitalWF(sPin,1);
#endif
		digitalWF(sPin,0);
    }
	return ret;
}
//*************************************************    24    *****************************************************
void regOut24(uint8_t dPin,uint8_t sPin, int8_t bOrder, uint16_t data){
	if(bOrder == MSBFIRST)
		data<<=8;
	for(uint8_t i=0;i<24;i++)
	{
		if(bOrder==LSBFIRST)
		{
			digitalWF(dPin,data&1);
			data=data>>1;
		}else
		{
			digitalWF(dPin,data & 0x80000000);
			data=data<<1;
		}
		digitalWF(sPin,1);
		digitalWF(sPin,0);
	}	
}
//------------------------------------------------------------------------------------
uint32_t regIn24(uint8_t dPin,uint8_t sPin, int8_t bOrder){
	uint32_t ret;
	for(uint8_t i=0;i<24;i++)
    {
		if(bOrder==LSBFIRST)
		{
			ret=ret>>1;
			if(digitalRF(dPin))
				ret|= B10000000;
		}
		else
		{
			ret=ret<<1;
				ret|= digitalRF(dPin);
		}
		digitalWF(sPin,1);
#ifdef ARDUINO_ARCH_STM32
		digitalWF(sPin,1);
#endif
		digitalWF(sPin,0);
    }
	if(bOrder==LSBFIRST)
			ret=ret>>8;
	return ret;
}
//*************************************************    32    *****************************************************
void regOut32(uint8_t dPin,uint8_t sPin, int8_t bOrder, uint32_t data){
	for(uint8_t i=0;i<32;i++)
	{
		if(bOrder==LSBFIRST)
		{
			digitalWF(dPin,data&1);
			data=data>>1;
		}else
		{
			digitalWF(dPin,data & 0x80000000);
			data=data<<1;
		}
		digitalWF(sPin,1);
		digitalWF(sPin,0);
	}
}
//------------------------------------------------------------------------------------
uint32_t regIn32(uint8_t dPin,uint8_t sPin, int8_t bOrder){
	uint32_t ret;
	for(uint8_t i=0;i<32;i++)
    {
		if(bOrder==LSBFIRST)
		{
			ret=ret>>1;
			if(digitalRF(dPin))
				ret|= B10000000;
		}
		else
		{
			ret=ret<<1;
				ret|= digitalRF(dPin);
		}
		digitalWF(sPin,1);
#ifdef ARDUINO_ARCH_STM32
		digitalWF(sPin,1);
#endif
		digitalWF(sPin,0);
    }
	return ret;
}
//****************************************************************************************
#endif