#define byte uint8_t

#define TIME_TO_SHOW_BALANCE              8000
#define TIME_TO_SHOW_STATE_UPDATES        2000
#define LCD_REFRESH_RATE                  1000
#define BOOT_REFRESH_RATE                 300
#define TIME_TO_SHOW_AUTHORIZATION_ERROR  5000
#define GSM_WAKEUP_TIME                   15000
#define GSM_BAUD_CHECK_TIME               3000
#define BROKER_RESPONSE_TIME              20000
#define GSM_CMD_INTERVAL                  50

#define CARD_UID_LENGTH       4
#define MAX_UART_DATA_LENGTH  100
#define MAX_BALANCE_LENGTH    12
#define MAX_DEVICE_ID_LENGTH  5
#define MAX_MESSAGE_LENGTH    254
#define MAX_BROKER_LENGTH     254

#define LCD_I2C_ADDRESS 0x27
#define LCD_ROWS        20
#define LCD_COLUMNS     4

#define FLOW_SENSOR_MULTIPLICAION_FACTOR  0.0024

#define COST_PER_LITRE_OF_WATER 0.40

/* Arduino Pin Configurations */
#define DEBUG_LED         13
#define RFID_SS_PIN       49
#define RFID_RESET_PIN    47
#define FLOW_SENSOR_PIN   19
#define SOLENOID_PIN      28
#define UV_PIN            27
#define GSM_RX            10
#define GSM_TX            11
#define GSM_RESET         48
#define CAN_CONTROLLER_CS 12
