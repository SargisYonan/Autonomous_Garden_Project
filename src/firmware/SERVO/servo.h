#define SERVO_PORT PORTH
#define SERVO_DDR DDRH
#define SERVO_PIN_NUMBER 4
#define SERVO_PIN PINH
#define SERVO_ON_SETTING 0x10

#define SERVO_OPEN 1
#define SERVO_CLOSED 0
#define SERVO_DUTY_ON 1
#define SERVO_DUTY_OFF 0

void InitializeServoPin (void);
void changeServoState(uint8_t state);

typedef struct SERVO_STATE
{
  uint8_t state;
  uint8_t duty_cycle_state;
  uint8_t duty_count;
} SERVO_STATE;

SERVO_STATE *servo_state;
