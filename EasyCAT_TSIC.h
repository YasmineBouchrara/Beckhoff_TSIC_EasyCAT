#ifndef CUSTOM_PDO_NAME_H
#define CUSTOM_PDO_NAME_H

//-------------------------------------------------------------------//
//                                                                   //
//     This file has been created by the Easy Configurator tool      //
//                                                                   //
//     Easy Configurator project EasyCAT_TSIC.prj
//                                                                   //
//-------------------------------------------------------------------//


#define CUST_BYTE_NUM_OUT	0
#define CUST_BYTE_NUM_IN	64
#define TOT_BYTE_NUM_ROUND_OUT	0
#define TOT_BYTE_NUM_ROUND_IN	64


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
		float       TSIC_5;
		float       TSIC_6;
		float       TSIC_7;
		float       TSIC_8;
		float       AVG_TSIC_1;
		float       AVG_TSIC_2;
		float       AVG_TSIC_3;
		float       AVG_TSIC_4;
		float       AVG_TSIC_5;
		float       AVG_TSIC_6;
		float       AVG_TSIC_7;
		float       AVG_TSIC_8;
	}Cust;
} PROCBUFFER_IN;

#endif