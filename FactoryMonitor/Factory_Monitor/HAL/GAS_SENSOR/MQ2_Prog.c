
#include "ADC_Interface.h"
#include "DIO_Interface.h"

#include "MQ2_Interface.h"

#define F_CPU 16000000UL
#include <util/delay.h>

float LPGCurve[3] = {2.3,0.20,-0.45};   //two points from LPG curve are taken point1:(200,1.6) point2(10000,0.26)
                                        //take log of each point (lg200, lg 1.6)=(2.3,0.20)  (lg10000,lg0.26)=(4,-0.58)
                                        //find the slope using these points. take point1 as reference   
                                        //data format:{ x, y, slope};  


float SmokeCurve[3] ={2.3,0.53,-0.43};  //two points from smoke curve are taken point1:(200,3.4) point2(10000,0.62) 
                                        //take log of each point (lg200, lg3.4)=(2.3,0.53)  (lg10000,lg0.63)=(4,-0.20)
                                        //find the slope using these points. take point1 as reference   
                                        //data format:{ x, y, slope};

// sensor and load resistor forms a voltage divider. so using analog value and load value
// we will find sensor resistor.


void HAL_MQ_voidInit(void)
{
	M_DIO_Void_SetPinDirection(PA1_PIN, INPUT);
	M_DIO_Void_SetPinPullUpRes(PA1_PIN,DISABLE);
}

f32 ResistanceCalculation(u32 raw_adc){ 
  return ( ((f32)RL_VALUE*(1023-raw_adc)/raw_adc));   // we will find sensor resistor.
}



float ReadSensor(){
  u32 i;
  f32 rs=0;

  for (i=0;i<3;i++) {                                 // take multiple readings and average it.
    rs += ResistanceCalculation(ADC_Read(MQ_CHANNEL));   // rs changes according to gas concentration.
    _delay_ms(50);
  }
  rs = rs/5;
  return rs;  
}

u32 GetGasPercentage(f32 rs_ro_ratio, u32 gas_id){
	rs_ro_ratio = rs_ro_ratio / Ro;
  if ( gas_id == LPG ) {
     return GetPercentage(rs_ro_ratio,LPGCurve);
  } 
  else if( gas_id == SMOKE ) {
     return GetPercentage(rs_ro_ratio,SmokeCurve);
  }    

  return 0;
}



u32  GetPercentage(f32 rs_ro_ratio, f32 *curve){                                                                          
  return (pow(10,( ((log(rs_ro_ratio)-curve[1])/curve[2]) + curve[0]))); 
}