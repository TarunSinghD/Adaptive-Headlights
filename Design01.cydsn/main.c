
/* ========================================
    HRLV- EZ4 Ultrasonic sensors and stepper motor interfacing
    26 Jul 2017
    Tarun and Rohith
    
    Adaptive Headlight:  This code controls two stepper motors which aim a 
    bicycle or motorcycle headlight in the desired direction regardless of
    the tilt of the vehicle, which is detected by an ultrasonic sensor.
    
    In this version, speed is not taken into account but should be in later 
    versions.
    
    Summer Project
 * ========================================
*/
#include "project.h"
#include <stdio.h>
#include <stdlib.h>

#define HALF_STEP

// Diagnostic (test) variables.
uint32 bad_dx_count ;
float percent_good_counts ;
uint32 bad_event_array[10] ;  // Keep track whenevr something goes wrong.

// Table of how to point the beam.  Values determined by experiment.
int32 yaw_table[11] = {-4, -3, -2, -1, -1, 0, 1, 1, 2, 3, 4}, yaw_index = 0;

int32 pitch_table[11] = {4, 3, 2, 1, 0, 0, 0, 1, 2, 3, 4}, pitch_index = 0;

/* Yaw index will point to home position at the beginning, which is 5 */
int32 prev_yaw_index = 5, prev_pitch_index = 5;

/* Steps to move */
int32 move_steps = 0, pitch_move_steps = 0, pitch_no_of_steps = 0;

/* Count variables to keep track of the pulse width of the sensor response */
int count = 0, count2, sensor_delay_count = 0;

/* Variables to store distances measured by the two sensors */
float distance = 0.0, distance2;

/* Delay between the steps of the stepper motor, 
    delay is sufficient to generate enough track for our application */
uint16 delay = 10000;

/* Keep track of the sum and differences between the distances tracked by the two sensors */
int sum, difference;
int hall_count = 0;

/* Flags to keep track of motor movement */
uint16 ccw_flag = 0, cw_flag = 0;

/* Variable to keep track of no of steps that the stepper motor should rotate */
int16 no_of_steps = 10;

/* Loop variable */
int i;

/* Variables to store previous valid value of distances */
float prev_distance = 0.0, prev_distance2;

/* A flag to indicate when the magnet is detected by the Hall Sensor */
uint8 hall_sensor_flag = 0;
uint8 pitch_limit = 10;

#ifdef HALF_STEP
void yaw_step1()
{
    Pin_A1_Write(1);
    Pin_A2_Write(0);
    Pin_B1_Write(0);
    Pin_B2_Write(0);   
}
void yaw_step2()
{
    Pin_A1_Write(1);
    Pin_A2_Write(0);
    Pin_B1_Write(1);
    Pin_B2_Write(0);   
}
void yaw_step3() 
{
    Pin_A1_Write(0);
    Pin_A2_Write(0);
    Pin_B1_Write(1);
    Pin_B2_Write(0);   
}
void yaw_step4() 
{
    Pin_A1_Write(0);
    Pin_A2_Write(1);
    Pin_B1_Write(1);
    Pin_B2_Write(0);   
}
void yaw_step5()
{
    Pin_A1_Write(0);
    Pin_A2_Write(1);
    Pin_B1_Write(0);
    Pin_B2_Write(0);   
}
void yaw_step6() 
{
    Pin_A1_Write(0);
    Pin_A2_Write(1);
    Pin_B1_Write(0);
    Pin_B2_Write(1);   
}
void yaw_step7()
{
    Pin_A1_Write(0);
    Pin_A2_Write(0);
    Pin_B1_Write(0);
    Pin_B2_Write(1);   
}
void yaw_step8() 
{
    Pin_A1_Write(1);
    Pin_A2_Write(0);
    Pin_B1_Write(0);
    Pin_B2_Write(1);   
}

void yaw_clockwise()
{
    yaw_step1(); 
    CyDelayUs(delay);
    yaw_step2();
    CyDelayUs(delay);
    yaw_step3();
    CyDelayUs(delay);
    yaw_step4();
    CyDelayUs(delay);
    yaw_step5(); 
    CyDelayUs(delay);
    yaw_step6();
    CyDelayUs(delay);
    yaw_step7();
    CyDelayUs(delay);
    yaw_step8();
    CyDelayUs(delay);
}

void yaw_counter_clockwise()
{
    yaw_step7(); 
    CyDelayUs(delay);
    yaw_step6();
    CyDelayUs(delay);
    yaw_step5();
    CyDelayUs(delay);
    yaw_step4();
    CyDelayUs(delay);
    yaw_step3(); 
    CyDelayUs(delay);
    yaw_step2();
    CyDelayUs(delay);
    yaw_step1();
    CyDelayUs(delay);
    yaw_step8();
    CyDelayUs(delay);
}

#endif

/* Yaw motor movement */
void step1() 
{
    Pin_A1_Write(0);
    Pin_A2_Write(1);
    Pin_B1_Write(1);
    Pin_B2_Write(0);
}

void step2() 
{
    Pin_A1_Write(0);
    Pin_A2_Write(1);
    Pin_B1_Write(0);
    Pin_B2_Write(1);
}

void step3() 
{
    Pin_A1_Write(1);
    Pin_A2_Write(0);
    Pin_B1_Write(0);
    Pin_B2_Write(1);
}
void step4() 
{
    Pin_A1_Write(1);
    Pin_A2_Write(0);
    Pin_B1_Write(1);
    Pin_B2_Write(0);
}

/* Pitch motor movement */
void step1_1() 
{
    Pin_A1_1_Write(0);
    Pin_A2_1_Write(1);
    Pin_B1_1_Write(1);
    Pin_B2_1_Write(0);
}

void step2_1() 
{
    Pin_A1_1_Write(0);
    Pin_A2_1_Write(1);
    Pin_B1_1_Write(0);
    Pin_B2_1_Write(1);
}

void step3_1() 
{
    Pin_A1_1_Write(1);
    Pin_A2_1_Write(0);
    Pin_B1_1_Write(0);
    Pin_B2_1_Write(1);
}
void step4_1() 
{
    Pin_A1_1_Write(1);
    Pin_A2_1_Write(0);
    Pin_B1_1_Write(1);
    Pin_B2_1_Write(0);
}


/* Pitch */
void stopMotor_1() 
{
    Pin_A1_1_Write(0);
    Pin_A2_1_Write(0);
    Pin_B1_1_Write(0);
    Pin_B2_1_Write(0);
    CyDelay(25);
}

/* Yaw */
void stopMotor() 
{
    Pin_A1_Write(0);
    Pin_A2_Write(0);
    Pin_B1_Write(0);
    Pin_B2_Write(0);
    CyDelay(25);
}

// Motor "1" is the pitch (up or down) motor.
// down
void clockwise_1()
{
    step1_1(); 
    CyDelayUs(delay);
    step2_1();
    CyDelayUs(delay);
    step3_1();
    CyDelayUs(delay);
    step4_1();
    CyDelayUs(delay);
}

// Yaw motor motion.
void clockwise()
{
    step1(); 
    CyDelayUs(delay);
    step2();
    CyDelayUs(delay);
    step3();
    CyDelayUs(delay);
    step4();
    CyDelayUs(delay);
}

void counter_clockwise_1()
{
    step3_1();
    CyDelayUs(delay);
    step2_1();
    CyDelayUs(delay);
    step1_1();
    CyDelayUs(delay);
    step4_1();
    CyDelayUs(delay);
}

void counter_clockwise()
{
    step3();
    CyDelayUs(delay);
    step2();
    CyDelayUs(delay);
    step1();
    CyDelayUs(delay);
    step4();
    CyDelayUs(delay);
}


/* ISR to detect home position of flash light */
CY_ISR(HALL_SENSOR)
{
    hall_sensor_flag = 1;
    hall_count++;
}

//Interrupt to Start counter1 for ultrasonic sensor1
//This interrupt is triggered at the rising edge of the pulse response received from the ultrasonic sensor 1
CY_ISR(RISING_EDGE_1)
{
    /* The echo pin of the ISR sends back a pulse proportional to the distnce */
    /* Use a counter to measure the pulse width, Distance can be derived from this pulse width */
    Counter_1_Start();
}

//Interrupt to Stop counter1 for ultrasonic 1
CY_ISR(FALLING_EDGE_1)
{
    /* Stop the counter, read its content, which indirectly translates to the pulse width */
    /* Distance information can be obtained from the pulse width */
    Counter_1_Stop();
    count = Counter_1_ReadCounter();
    Counter_1_WriteCounter(0);
    
    /* Convert the pulse width to distance in cm */
    distance = (float)(count / 10.0);
    
    /* Simple filtering to ignore rouge values */
    if ((distance < 150) &&(distance > 30))
    {
        prev_distance = distance;
        //good_dx_count++ ;
    }
    
    else
    {
        distance =  prev_distance; 
        bad_dx_count++;
    }
    
    // Keep track of often wehave bad counts.
    //percent_good_counts = 100. * (float)good_dx_count / (float)bad_dx_count  ;
    
    /* Print the distances on the LCD */
    LCD_Position(0, 1);
    LCD_PrintString("    ");
    LCD_Position(0, 1);
    LCD_PrintU32Number(distance);
}

//Interrupt to Stop counter2 for ultrasonic sensor2
//This interrupt is triggered at the rising edge of the pulse response received from the ultrasonic sensor 2
CY_ISR(RISING_EDGE_2)
{
    /* The echo pin of the ISR sends back a pulse proportional to the distnce */
    /* Use a counter to measure the pulse width, Distance can be derived from this pulse width */
    Counter_2_Start();
}

//Interrupt to Stop counter1 for ultrasonic 1
CY_ISR(FALLING_EDGE_2)
{
    /* Stop the counter, read its content, which indirectly translates to the pulse width */
    /* Distance information can be obtained from the pulse width */
    Counter_2_Stop();
    count2 = Counter_2_ReadCounter();
    Counter_2_WriteCounter(0);
    
    /* Convert the pulse width to distance in cm */
    distance2 = (float)(count2 / 10.0);
    
    /* Restricting range of valid values */
    if ((distance2 < 150) &&(distance2 > 55))
    {
        prev_distance2 = distance2;   
    }
    
    /*  Digital Filtering, We have a narrow window of valid readings
        Beyond which we throw the signal away, and use the previous valid reading   
    */
    else
    {
        distance2 =  prev_distance2;  
    }
    
    /* Print the distances on the LCD */
    LCD_Position(1, 1);
    LCD_PrintString("    ");
    LCD_Position(1, 1);
    LCD_PrintU32Number(distance2);
    
    #if 1
    LCD_Position(0, 8);
    LCD_PrintString( "D:    " );
    LCD_Position(0, 10);
    difference = distance - distance2;
    
    if (difference < 0)
    {
       LCD_PutChar('-');
       LCD_PrintNumber(-1 * difference);  
    }
    
    else
    {
        LCD_PrintNumber(difference);
    }
    #endif
    
    sensor_delay_count++;
    
    #if 0
    /* Need an equation to relate the difference with the number of steps */
    /* Detect the tilt to the left, and move the motor */
    if ((difference > 4) && (difference < 11) && (ccw_flag == 0))
    {
        //difference = -1 * difference;
        no_of_steps = 10;
        ccw_flag = 1;
        
        for(i = 0; i < no_of_steps; i++)
        {
            counter_clockwise();
        }
        
        for (i = 0; i < pitch_limit; i++)
        counter_clockwise_1();
    }
    
    else if ((difference <= 4) && (difference > -4)  && (ccw_flag == 1))
    {
        #if 0
        while (Digital_Out_P3_0_Read())
        {
            clockwise();   
        }
        
        for (i = 0; i < 2; i++)
            clockwise();
            
            stopMotor();
        
        
        #else
        
        for (i = 0; i < pitch_limit; i++)
        clockwise_1();
        
        for(i = 0; i < no_of_steps; i++)
        {
            clockwise();
        }
        ccw_flag = 0;
        #endif   
    }

        #if 1
        if ((difference < -4) && (difference > -11)&& (cw_flag == 0))
        {
            //difference = -1 * difference;
            no_of_steps = 10;
            cw_flag = 1;
            
            for(i = 0; i < no_of_steps; i++)
            {
                clockwise();
            }
            
            for (i = 0; i < pitch_limit; i++)
            counter_clockwise_1();
        }
        
        else if ((difference > -4) && (difference < 4) && (cw_flag == 1))
        {
            #if 0
            while (Digital_Out_P3_0_Read())
            {
                counter_clockwise();   
            }
            
            //for (i = 0; i < 2; i++)
                counter_clockwise(); 
                
                stopMotor();
           
            
            #else
            
            for (i = 0; i < pitch_limit; i++)
                clockwise_1();
            
            for(i = 0; i < no_of_steps; i++)
            {
                counter_clockwise();
            }
            cw_flag = 0;
            #endif
        }
        #endif
    //Indexing
    
    #else
        
    if ((sensor_delay_count % 4) == 0)
    {
        if (((difference >= -5) && (difference <= 5)))
        {
            if (!((difference == -1) || (difference == 1)))
            {
                // Pointing to the right index in the lookup table based on the difference readings
                // Yaw lookup table
                yaw_index = difference + 5;
                no_of_steps = yaw_table[yaw_index];
                move_steps = no_of_steps - yaw_table[prev_yaw_index];
                
                // Pitch lookup table
                pitch_index = difference + 5;
                pitch_no_of_steps = pitch_table[pitch_index];
                pitch_move_steps = pitch_no_of_steps - pitch_table[prev_pitch_index];
                
                // clockwise
                if (move_steps < 0)
                {
                    move_steps = -1 * move_steps;
                    
                    // no of steps to move is obtained from the lookup table based on index obtained 
                    // in the previous step
                    for (i = 0; i < move_steps; i++)
                    {
                        #ifdef HALF_STEP
                            yaw_clockwise();
                        
                        #else
                            clockwise();
                            
                        #endif
                    }
                }
                
                //counter_clockwise
                else
                {
                    //no of steps to move is obtained from the lookup table based on index obtained 
                    //in the previous step
                    for (i = 0; i < move_steps; i++)
                    {
                        #ifdef HALF_STEP
                            yaw_counter_clockwise();
                        
                        #else
                            counter_clockwise();
                            
                        #endif 
                    }  
                }
                
                // Pitch motor control
                // Moved away from home position
                if (pitch_move_steps > 0)
                {
                    //pitch motor has to move up
                    for (i = 0; i < pitch_move_steps; i++)
                    {
                       counter_clockwise_1(); 
                    
                    }
                    
                }
                
                // Motor moving towards home position
                else
                {
                    // Need 
                    pitch_move_steps = -1 * pitch_move_steps;
                    
                    // pitch motor has to move down
                    for (i = 0; i < pitch_move_steps; i++)
                    {
                       clockwise_1(); 
                    } 
                }
                
                prev_pitch_index = pitch_index;
                prev_yaw_index = yaw_index;
            }
        }
    }
    #endif
}






/////////////////////////////////  Start of main.c  ////////////////////////////////////
//
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    #if 1
     for (i = 0; i < 50; i++)
        clockwise();
        
    for (i = 0; i < 50; i++) 
        counter_clockwise_1();
        
    for (i = 0; i < 16; i++)  
        clockwise_1(); 
        
    for (i = 0; i < 8; i++)
        counter_clockwise();
        #endif
         
    ccw_flag = 0, cw_flag = 0;
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    //right sensor
    isr_1_StartEx(RISING_EDGE_1);
    isr_3_StartEx(FALLING_EDGE_1);

    //left sensor
    isr_2_StartEx(RISING_EDGE_2);
    isr_4_StartEx(FALLING_EDGE_2);

    //hall sensor
    isr_5_StartEx(HALL_SENSOR);
    hall_sensor_flag = 0;
    
    #if 0
    /* Run stepper motor orientation test once */
    for (i = 0; i < 20; i++)
    {
        if (hall_sensor_flag == 0)
        {
            clockwise();
        }
        
        else
        {
            clockwise();        
            break;
        }
    }

    /* Initial sweep of clockwise doesn't detect the hall sensor, so back */
    if (i == 20)
    {
        for (i = 0; i < 50; i++)
        {
            if (!hall_sensor_flag)
            {
                counter_clockwise();
            }
            
            else
            {
                counter_clockwise();             
                break;
            }
        }
        
        hall_sensor_flag = 0;
    }
    #endif
    
    
    for(;;)
    {
        /* Place your application code here. */
    }
    
    return 0;
}

/* [] END OF FILE */








////////////////////////////////// Scrap after this point /////////////////////////////

/* Scrap code */
#if 0
        /* Send a trigger pulse of duration 10 us to signal the sensor to start measuring distance */
        Ultrasonic2_Trigger_P_12_6_Write(1);
        Ultrasonic1_Trigger_P_12_7_Write(1);
        CyDelayUs(25);
        Ultrasonic1_Trigger_P_12_7_Write(0);
        Ultrasonic2_Trigger_P_12_6_Write(0);
        
        /* Wait for 500 ms, whic is the max time required for a valid response */
        CyDelay(500);
        
        #endif
        
         #if 0
        /* Need an equation to relate the difference with the number of steps */
        if ((difference > 5) && (ccw_flag == 0))
        {
            //difference = -1 * difference;
            no_of_steps = 10;
            ccw_flag = 1;
            
            for(i = 0; i < no_of_steps; i++)
            {
                counter_clockwise();
            }
        }
        
        else if (difference < 4  && ccw_flag == 1)
        {
            for(i = 0; i < no_of_steps; i++)
            {
                clockwise();
            }
            ccw_flag = 0;
        }
        
        if ((difference < -5) && (cw_flag == 0))
        {
            //difference = -1 * difference;
            no_of_steps = 10;
            cw_flag = 1;
            
            for(i = 0; i < no_of_steps; i++)
            {
                clockwise();
            }
        }
        
        else if (difference > -4  && cw_flag == 1)
        {
            for(i = 0; i < no_of_steps; i++)
            {
                counter_clockwise();
            }
            cw_flag = 0;
        }
    #endif
        //clockwise();
