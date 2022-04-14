/* main.c - Application main entry point */

/* Based on an example from Zephyr toolkit, modified by frank duignan
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/* This example advertises three services:
 * 0x1800 Generic ACCESS (GAP)
 * 0x1801 Generic Attribute (GATT - this is part of the software device and is not used nor is it apparently removable see https://devzone.nordicsemi.com/f/nordic-q-a/15076/removing-the-generic-attribute-0x1801-primary-service-if-the-service-changed-characteristic-is-not-present
 * And a custom service 1-2-3-4-0 
 * This custom service contains a custom characteristic called char_value
 */
 
 //*************************************************************************************************************
 
#include <zephyr/types.h> //includes a type of library
#include <stddef.h>	  //includes a type of library
#include <string.h>	  //includes a type of library
#include <errno.h> 	  //includes a type of library
#include <sys/printk.h>	  //includes a type of library
#include <sys/byteorder.h>//includes a type of library
#include <zephyr.h>	  //includes a type of library

#include <settings/settings.h>	//includes a type of library

#include <bluetooth/bluetooth.h>//includes a type of library
#include <bluetooth/hci.h>	//includes a type of library
#include <bluetooth/conn.h> 	//includes a type of library
#include <bluetooth/uuid.h>	//includes a type of library
#include <bluetooth/gatt.h>	//includes a type of library
#include <device.h>		//includes a type of library
#include <drivers/sensor.h>	//includes a type of library
#include <stdio.h>		//includes a type of library
#include "matrix.h" 		//includes matrix.h file in the directory assignment
#include "lsm303_ll.h"		//includes lsm303_ll.h file in the directory assignment
#include "buttons.h"		//includes a buttons.h file in the directory assignment



#define BT_UUID_CUSTOM_SERVICE_VAL BT_UUID_128_ENCODE(1, 2, 3, 4, (uint64_t)0) 		//defines the bluetooth identifier
#define BT_UUID_CUSTOM_SERVICE1_VAL BT_UUID_128_ENCODE(1, 2, 3, 4, (uint64_t)0x1000)	//defines the bluetooth identifier
#define BT_UUID_CUSTOM_CHAR_VAL    BT_UUID_128_ENCODE(1, 2, 3, 4, (uint64_t)5)	  	//defines the bluetooth identifier
#define BT_UUID_Y_ACCEL_ID  	   BT_UUID_128_ENCODE(1, 2, 3, 4, (uint64_t)1)		//defines the bluetooth identifier	
#define BT_UUID_X_ACCEL_ID  	   BT_UUID_128_ENCODE(1, 2, 3, 4, (uint64_t)2)		//defines the bluetooth identifier
#define BT_UUID_Z_ACCEL_ID  	   BT_UUID_128_ENCODE(1, 2, 3, 4, (uint64_t)3)		//defines the bluetooth identifier
#define BT_UUID_Y_MAG_ID  	   BT_UUID_128_ENCODE(1, 2, 3, 4, (uint64_t)1)		//defines the bluetooth identifier
#define BT_UUID_X_MAG_ID  	   BT_UUID_128_ENCODE(1, 2, 3, 4, (uint64_t)2)		//defines the bluetooth identifier
#define BT_UUID_Z_MAG_ID  	   BT_UUID_128_ENCODE(1, 2, 3, 4, (uint64_t)3)		//defines the bluetooth identifier
#define BT_UUID_STEPCOUNT_ID       BT_UUID_128_ENCODE(1, 2, 3, 4, (uint64_t)6)		//defines the bluetooth identifier
#define BT_UUID_MAG_ID       BT_UUID_128_ENCODE(1, 2, 3, 4, (uint64_t)6)		//defines the bluetooth identifier
#define BT_UUID_LED_SERVICE_VAL BT_UUID_128_ENCODE(1, 2, 3, 4, (uint64_t)0x3000)	//defines the bluetooth identifier
#define BT_UUID_LED_ID BT_UUID_128_ENCODE(1, 2, 3, 4, (uint64_t)0x3001)			//defines the bluetooth identifier
#define BT_UUID_BUTTON_SERVICE_VAL BT_UUID_128_ENCODE(1, 2, 3, 4, (uint64_t)20)		//defines the bluetooth identifier
#define BT_UUID_BUTTON_A_ID  	   BT_UUID_128_ENCODE(1, 2, 3, 4, (uint64_t)22)		//defines the bluetooth identifier
#define BT_UUID_BUTTON_B_ID  	   BT_UUID_128_ENCODE(1, 2, 3, 4, (uint64_t)24)		//defines the bluetooth identifier


static struct bt_uuid_128 my_service_uuid = BT_UUID_INIT_128( BT_UUID_CUSTOM_SERVICE_VAL);  	// the 128 bit UUID for this gatt value
static struct bt_uuid_128 my_service1_uuid = BT_UUID_INIT_128( BT_UUID_CUSTOM_SERVICE1_VAL);	// the 128 bit UUID for this gatt value
static struct bt_uuid_128 char_id=BT_UUID_INIT_128(BT_UUID_CUSTOM_CHAR_VAL); 		    	// the 128 bit UUID for this gatt value

static struct bt_uuid_128 stepcount_id=BT_UUID_INIT_128(BT_UUID_STEPCOUNT_ID);  		// the 128 bit UUID for this gatt value
static struct bt_uuid_128 y_accel_id=BT_UUID_INIT_128(BT_UUID_Y_ACCEL_ID); 			// the 128 bit UUID for this gatt value
static struct bt_uuid_128 x_accel_id=BT_UUID_INIT_128(BT_UUID_X_ACCEL_ID); 			// the 128 bit UUID for this gatt value
static struct bt_uuid_128 z_accel_id=BT_UUID_INIT_128(BT_UUID_Z_ACCEL_ID); 			// the 128 bit UUID for this gatt value

static struct bt_uuid_128 mag_id=BT_UUID_INIT_128(BT_UUID_MAG_ID);
static struct bt_uuid_128 y_mag_id=BT_UUID_INIT_128(BT_UUID_Y_MAG_ID); 				// the 128 bit UUID for this gatt value
static struct bt_uuid_128 x_mag_id=BT_UUID_INIT_128(BT_UUID_X_MAG_ID); 				// the 128 bit UUID for this gatt value
static struct bt_uuid_128 z_mag_id=BT_UUID_INIT_128(BT_UUID_Z_MAG_ID); 				// the 128 bit UUID for this gatt value

static struct bt_uuid_128 my_led_uuid = BT_UUID_INIT_128( BT_UUID_LED_SERVICE_VAL);		// the 128 bit UUID for this gatt value
static struct bt_uuid_128 led1_id = BT_UUID_INIT_128(BT_UUID_LED_ID); 		   		// the 128 bit UUID for this gatt value

static struct bt_uuid_128 my_button_uuid = BT_UUID_INIT_128( BT_UUID_BUTTON_SERVICE_VAL);
static struct bt_uuid_128 button_a_id=BT_UUID_INIT_128(BT_UUID_BUTTON_A_ID); 			// the 128 bit UUID for this gatt value
static struct bt_uuid_128 button_b_id=BT_UUID_INIT_128(BT_UUID_BUTTON_B_ID); 			// the 128 bit UUID for this gatt value


uint32_t char_stepcount; 	// the gatt characateristic value that is being shared over BLE 
uint32_t char_value;		// the gatt characateristic value that is being shared over BLE 
uint32_t stepcount_value=0; 	// the gatt characateristic value that is being shared over BLE  
uint32_t led1_value;		// the gatt characateristic value that is being shared over BLE  
uint32_t y_accel;		// the gatt characateristic value that is being shared over BLE  
uint32_t x_accel;		// the gatt characateristic value that is being shared over BLE  
uint32_t z_accel;		// the gatt characateristic value that is being shared over BLE  
uint32_t y_mag;			// the gatt characateristic value that is being shared over BLE  
uint32_t x_mag;			// the gatt characateristic value that is being shared over BLE  
uint32_t z_mag;			// the gatt characateristic value that is being shared over BLE  
uint32_t button_a;		// the gatt characateristic value that is being shared over BLE 
uint32_t button_b;		// the gatt characateristic value that is being shared over BLE 
uint32_t button_a_state=0;	// the gatt characateristic value that is being shared over BLE  
uint32_t button_b_state=0;	// the gatt characateristic value that is being shared over BLE  

static ssize_t read_stepcount(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset); 
//callback that is activated when the characteristic is read by central
static ssize_t write_stepcount(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags);
//callback that is activated when the characteristic is read by central
static ssize_t read_y_accel(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
//callback that is activated when the characteristic is read by central
static ssize_t read_x_accel(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
//callback that is activated when the characteristic is read by central
static ssize_t read_z_accel(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
//callback that is activated when the characteristic is read by central
static ssize_t read_y_mag(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
//callback that is activated when the characteristic is read by central
static ssize_t read_x_mag(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
//callback that is activated when the characteristic is read by central
static ssize_t read_z_mag(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
//callback that is activated when the characteristic is read by central
static ssize_t read_led(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
//callback that is activated when the characteristic is read by central
static ssize_t read_value(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
//callback that is activated when the characteristic is read by central
static ssize_t read_button_a(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
//callback that is activated when the characteristic is read by central
static ssize_t read_button_b(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
//callback that is activated when the characteristic is read by central


/* The bt_data structure type:
 * {
 * 	uint8_t type : The kind of data encoded in the following structure
 * 	uint8_t data_len : the length of the data encoded
 * 	const uint8_t *data : a pointer to the data
 * }
 * This is used for encoding advertising data
*/
/* The BT_DATA_BYTES macro
 * #define BT_DATA_BYTES(_type, _bytes...) BT_DATA(_type, ((uint8_t []) { _bytes }), sizeof((uint8_t []) { _bytes }))
 * #define BT_DATA(_type, _data, _data_len) \
 *       { \
 *               .type = (_type), \
 *               .data_len = (_data_len), \
 *               .data = (const uint8_t *)(_data), \
 *       }
 * BT_DATA_UUID16_ALL : value indicates that all UUID's are listed in the advertising packet
*/
// bt_data is an array of data structures used in advertising. Each data structure is formatted as described above
static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)), /* specify BLE advertising flags = discoverable, BR/EDR not supported (BLE only) */
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_CUSTOM_SERVICE_VAL /* A 128 Service UUID for the our custom service follows */),
};
	
/*
 * #define BT_GATT_CHARACTERISTIC(_uuid, _props, _perm, _read, _write, _value) 
 * 
 */
BT_GATT_SERVICE_DEFINE(my_service_svc,
	BT_GATT_PRIMARY_SERVICE(&my_service_uuid), 				//Creates a new service for stepcount and accel
		BT_GATT_CHARACTERISTIC(&stepcount_id.uuid,		
		BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE |  BT_GATT_CHRC_NOTIFY,
		BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
		read_stepcount, write_stepcount, &char_stepcount),		//enables the read and write features
		
		BT_GATT_CHARACTERISTIC(&y_accel_id.uuid,			//locates the address of the accel id
		BT_GATT_CHRC_READ,
		BT_GATT_PERM_READ,
		read_y_accel, NULL, &y_accel),					//only read is nessesary in this case
		
		BT_GATT_CHARACTERISTIC(&x_accel_id.uuid,			//locates the address of the accel id
		BT_GATT_CHRC_READ,
		BT_GATT_PERM_READ,
		read_x_accel, NULL, &x_accel),					//only read is nessesary in this case
		
		BT_GATT_CHARACTERISTIC(&z_accel_id.uuid,			//locates the address of the accel id
		BT_GATT_CHRC_READ,
		BT_GATT_PERM_READ,
		read_z_accel, NULL, &z_accel),					//only read is nessesary in this case
);

//*******************************************************************************************************************************


BT_GATT_SERVICE_DEFINE(my_service1_svc,
	BT_GATT_PRIMARY_SERVICE(&my_service1_uuid),				//Creates a new service for mag
		BT_GATT_CHARACTERISTIC(&mag_id.uuid,		
		BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE |  BT_GATT_CHRC_NOTIFY,
		BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
		read_value, NULL, &char_value),					//enables the read feature
		
		BT_GATT_CHARACTERISTIC(&y_mag_id.uuid,				//locates the address of the Mag id
		BT_GATT_CHRC_READ,
		BT_GATT_PERM_READ,
		read_y_mag, NULL, &y_mag),					//only read is nessesary in this case
		
		BT_GATT_CHARACTERISTIC(&x_mag_id.uuid,				//locates the address of the Mag id
		BT_GATT_CHRC_READ,
		BT_GATT_PERM_READ,
		read_x_mag, NULL, &x_mag),					//only read is nessesary in this case
		
		BT_GATT_CHARACTERISTIC(&z_mag_id.uuid,				//locates the address of the Mag id
		BT_GATT_CHRC_READ,
		BT_GATT_PERM_READ,
		read_z_mag, NULL, &z_mag),					//only read is nessesary in this case
);

BT_GATT_SERVICE_DEFINE(my_button_svc,				//creates a new service for button
	BT_GATT_PRIMARY_SERVICE(&my_button_uuid),
		BT_GATT_CHARACTERISTIC(&button_a_id.uuid,		
		BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
		BT_GATT_PERM_READ,
		read_button_a, NULL, &button_a_state),		//enables the read feature
		
		BT_GATT_CHARACTERISTIC(&button_b_id.uuid,		
		BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
		BT_GATT_PERM_READ,
		read_button_b, NULL, &button_b_state)		//enables the read feature
		
);

static ssize_t read_button_a(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
	printf("Got a read on button A state\n");	//prints on commandline when button A is pressed
	const char *value = (const char *)&button_a_state;// point at the value in memory
	return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(button_a_state));
}
static ssize_t read_button_b(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
	printf("Got a read on button B state\n");	//prints on commandline when button B is pressed
	const char *value = (const char *)&button_b_state;// point at the value in memory
	return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(button_b_state));
}

struct bt_conn *active_conn=NULL; // use this to maintain a reference to the connection with the central device (if any)
static ssize_t read_led(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
	printf("Got a read to led state %p\n",attr); 	//prints when reading the led state
	matrix_begin();					//function to initilise all LED's on microbit
	// Could use 'const char *value =  attr->user_data' also here if there is the char value is being maintained with the BLE STACK
	const char *value = (const char *)&led1_value; // point at the value in memory
	return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(led1_value)); // pass the value back up through the BLE stack
	
}
static ssize_t write_led(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
	
	int ret_matrix;					//creating a new variable called ret_matrix
	uint8_t *value = attr->user_data;		
	printf("Got a write to led state %p\n", value);	//when writing value this prints
	memcpy(value, buf, len); // copy the incoming value in the memory occupied by our characateristic variable
	uint8_t rows = 1;			//initilising rows
	uint8_t cols = 1;			//initilising cols
	
	if(led1_value == 0x31)			// if the value is written as a 1, the microbit see 0x31. 
	{
		rows =  0b11111;		// sets the rows to all on		
           	cols = 0b11111; 		//sets the cols to all off
		matrix_begin();			//initilises the LED's
		matrix_put_pattern(rows, ~cols);//put the pattern on the matrix (tilda, ~ inverts cols)
		
	}
	if(led1_value == 0x32)			// if the value is written as a 2, the microbit see 0x32.
	{
		rows =  0b10101;		//sets the 1st, 3rd and 5th rows to on
           	cols = 0b10101; 		//sets the 1st, 3rd and 5th cols to off
		matrix_begin();			//initilises the LED's
		matrix_put_pattern(rows, ~cols);//put the pattern on the matrix
		
		
	}
	return len;				//returns the len value
	
}

BT_GATT_SERVICE_DEFINE(my_led_svc,				//creates a new service for LED
	BT_GATT_PRIMARY_SERVICE(&my_led_uuid),
		BT_GATT_CHARACTERISTIC(&led1_id.uuid,		
		BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,
		BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
		read_led, write_led, &led1_value),		//enables the read and write feature
);

// Callback that is activated when the characteristic is read by central
static ssize_t read_stepcount(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
	printf("Got a read %p\n",attr);
	// Could use 'const char *value =  attr->user_data' also here if there is the char value is being maintained with the BLE STACK
	const char *value = (const char *)&char_stepcount; // point at the value in memory
	return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(char_stepcount)); // pass the value back up through the BLE stack
}
// Callback that is activated when the characteristic is written by central
static ssize_t write_stepcount(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			 const void *buf, uint16_t len, uint16_t offset,
			 uint8_t flags)
{
	uint8_t *value = attr->user_data;
	printf("Got a write for stepcount\n"); 	//when writing value this prints
	memcpy(value, buf, len); 		// copy the incoming value in the memory occupied by our characateristic variable
	return len;				//returns the len value
}

static ssize_t read_y_accel(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
	printf("Got a y_accel read %d\n",y_accel);	//when reading y accel value this prints
	const char *value = (const char *)&y_accel;	// point at the value in memory
	return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(y_accel)); // pass the value back up through the BLE stack
	return 0;
}
static ssize_t read_x_accel(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
	printf("Got a x_accel read %d\n",x_accel);	//when reading x accel value this prints
	const char *value = (const char *)&x_accel;	// point at the value in memory
	return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(x_accel));  // pass the value back up through the BLE stack
	return 0;
}
static ssize_t read_z_accel(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
	printf("Got a z_accel read %d\n",z_accel);	//when reading z accel value this prints
	const char *value = (const char *)&z_accel;	// point at the value in memory
	return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(z_accel)); // pass the value back up through the BLE stack
	return 0;
}


//**************************************************************************************************************************

static ssize_t read_value(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
	printf("Got a read for mag %p\n",attr);		//when reading mag value this prints
	// Could use 'const char *value =  attr->user_data' also here if there is the char value is being maintained with the BLE STACK
	const char *value = (const char *)&char_value; // point at the value in memory
	return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(char_value)); // pass the value back up through the BLE stack
}


static ssize_t read_y_mag(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
	printf("Got a y_mag read %d\n",y_mag);		//when reading y mag value this prints
	const char *value = (const char *)&y_mag;	//points the value into address of z_mag
	return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(y_mag)); // pass the value back up through the BLE stack
	return 0;
}
static ssize_t read_x_mag(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
	printf("Got a x_mag read %d\n",x_mag);		//when reading x mag value this prints
	const char *value = (const char *)&x_mag;	//points the value into address of z_mag
	return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(x_mag));  // pass the value back up through the BLE stack
	return 0;
}
static ssize_t read_z_mag(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
	printf("Got a z_mag read %d\n",z_mag);		//when reading z mag value this prints
	const char *value = (const char *)&z_mag;	//points the value into address of z_mag
	return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(z_mag)); // pass the value back up through the BLE stack
	return 0;
}

// Callback that is activated when a connection with a central device is established
static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		printf("Connection failed (err 0x%02x)\n", err); //prints if the connection fails
	} else {
		printf("Connected\n");
		active_conn = conn;				//puts conn into active_conn when connected
	}
}
// Callback that is activated when a connection with a central device is taken down
static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	printk("Disconnected (reason 0x%02x)\n", reason);
	active_conn = NULL;					//puts NULL into active_conn when disconnected
}
// structure used to pass connection callback handlers to the BLE stack
static struct bt_conn_cb conn_callbacks = {
	.connected = connected,
	.disconnected = disconnected,
};
// This is called when the BLE stack has finished initializing
static void bt_ready(void)
{
	int err; 				//local veriable called err
	printf("Bluetooth initialized\n"); 	//prints when br_ready function is called

// start advertising see https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/zephyr/reference/bluetooth/gap.html
/*
 * Excerpt from zephyr/include/bluetooth/bluetooth.h

 * #define BT_LE_ADV_CONN_NAME BT_LE_ADV_PARAM(BT_LE_ADV_OPT_CONNECTABLE | \
                                            BT_LE_ADV_OPT_USE_NAME, \
                                            BT_GAP_ADV_FAST_INT_MIN_2, \
                                            BT_GAP_ADV_FAST_INT_MAX_2, NULL)

 Also see : zephyr/include/bluetooth/gap.h for BT_GAP_ADV.... These set the advertising interval to between 100 and 150ms
 
 */
// Start BLE advertising using the ad array defined above
	err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
		printf("Advertising failed to start (err %d)\n", err);
		return;
	} //prints if there a fault for advertising
	printf("Advertising successfully started\n");
}

void main(void)				//main
{
	
	int ret_button;			//new variable ret_button
	int ret_matrix;			//new variable ret_button
	int err;			//new variable err
	uint8_t rows = 1;		//initilsing rows for led matrix 
	uint8_t cols = 1;		//initilsing cols for led matrix
	err = lsm303_ll_begin();	//calling lsm303_ll_begin function in lsm303_ll.c
	ret_button = buttons_begin();	//calling buttons begin function in buttons.c
	if (ret_button < 0)
	{
		printf("\nError initializing buttons.  Error code = %d\n",ret_button);	
	 while(1);
	} 	//error checking for buttons
	if (ret_matrix < 0)
	{
		printf("\nError initializing buttons.  Error code = %d\n",ret_matrix);	
	 while(1);
	}	//error checking for led matrix
	if (err < 0)
	{
		 printf("\nError initializing lsm303.  Error code = %d\n",err);  
         while(1);
	}	//error checking for lsm303
	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}	//error checking for bluetooth
	bt_ready(); // This function starts advertising
	bt_conn_cb_register(&conn_callbacks);
	printf("Zephyr Microbit V2 minimal BLE example! %s\n", CONFIG_BOARD);
	if (lsm303_countSteps(&stepcount_value) < 0)
	{
		printf("Error starting step counter\n");
		while(1);
	}	//error checking for stepcounter			
	while (1) {
	
		k_sleep(K_SECONDS(1));				//sleeps for 1 second
		char_stepcount++;				//increments the stepcounter
		y_accel = lsm303_ll_readAccelY(); 		//lets y_accel equal to lsm303_ll_readAccelY()
		x_accel = lsm303_ll_readAccelX();		//lets x_accel equal to lsm303_ll_readAccelX()
		z_accel = lsm303_ll_readAccelZ();		//lets z_accel equal to lsm303_ll_readAccelZ()
		y_mag = lsm303_ll_readMagY();			//lets y_mag equal to lsm303_ll_readMagY()
		x_mag = lsm303_ll_readMagX();			//lets x_mag equal to lsm303_ll_readMagX()
		z_mag = lsm303_ll_readMagZ();			//lets z_mag equal to lsm303_ll_readMagZ()
		
		
			if(get_buttonA() == 0){			
				printf("\n Button A pressed");	//prints when button A has been pressed
			}
			if(get_buttonB() == 0)			
			{
				printf("\n Button B pressed");	//prints when button B has been pressed
			}
		if (active_conn)				//if there is an active connection, notify the stepcount, button A and Button B
		{
		
		bt_gatt_notify(active_conn,&my_service_svc.attrs[2],&stepcount_value,sizeof(stepcount_value));	
		bt_gatt_notify(active_conn,&my_button_svc.attrs[2], &button_a_state,sizeof(button_a_state));
		bt_gatt_notify(active_conn,&my_button_svc.attrs[3], &button_b_state,sizeof(button_b_state));		
		}
			
	}
}
