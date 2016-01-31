
// Min/Max values that the servos can handle
#define PWM_MAX         8750
#define PWM_MIN         1250
#define PWM_DEFAULT     3000
#define PWM_PERIOD      125000
#define TIMER_1_8_PRESCALE  0x02
#define TIMER_1_IR_ENABLE   0x02


struct servo {
    unsigned int duty;
    unsigned char state;
    
};

int init_servos(void);
int set_servo(int, int);
int get_servo(int);