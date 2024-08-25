#define byte uint8_t

#define TIME_TO_SHOW_BALANCE              8000
#define TIME_TO_SHOW_STATE_UPDATES        2000
#define LCD_REFRESH_RATE                  1000
#define BOOT_REFRESH_RATE                 300
#define TIME_TO_SHOW_AUTHORIZATION_ERROR  5000


#define FLOW_SENSOR_MULTIPLICAION_FACTOR  0.0024

#define COST_PER_LITRE_OF_WATER 0.40


#define LCD_I2C_ADDRESS 0x27
#define LCD_ROWS        20
#define LCD_COLUMNS     4


/* Arduino Pin Configurations */
#define RFID_SS_PIN       20
#define RFID_RESET_PIN    21
#define FLOW_SENSOR_PIN   2
#define SOLENOID_PIN      8
#define Ethernet_PIN      53 //SO-->50, SI -->51, SCK-->52, CS--> 53

// Pin Map LCD_I2C → Second SCl,SDA pin after AREF