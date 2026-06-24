#ifndef CUSTOM_PDO_NAME_H
#define CUSTOM_PDO_NAME_H

//-------------------------------------------------------------------//
//                                                                   //
//     Custom EasyCAT process data for the 4-sensor TSIC setup       //
//                                                                   //
//     Inputs from EasyCAT/Arduino to Beckhoff:                      //
//       TSIC_1, TSIC_2, TSIC_3, TSIC_4 as float values              //
//                                                                   //
//     The matching .xml and .bin files must be created with the     //
//     Easy Configurator and must use the same PDO entries.          //
//                                                                   //
//-------------------------------------------------------------------//


#define CUST_BYTE_NUM_OUT	0
#define CUST_BYTE_NUM_IN	16
#define TOT_BYTE_NUM_ROUND_OUT	0
#define TOT_BYTE_NUM_ROUND_IN	16


typedef union												//---- output buffer ----
{
	uint8_t  Byte [TOT_BYTE_NUM_ROUND_OUT];
	struct
	{
	}Cust;
} PROCBUFFER_OUT;


typedef union												//---- input buffer ----
{
	uint8_t  Byte [TOT_BYTE_NUM_ROUND_IN];
	struct
	{
		float       TSIC_1;
		float       TSIC_2;
		float       TSIC_3;
		float       TSIC_4;
	}Cust;
} PROCBUFFER_IN;

#endif
