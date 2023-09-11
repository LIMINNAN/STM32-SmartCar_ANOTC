
/******************************************************************************
 * @file     				 ws_task.c
 * @brief    				 ws程序框架下的任务管理
 * @version  				 1.0.0
 * @creationTime     2020_02_29
 * @creator
 * @readme
 *        1.STM32F103系列
 *******************************************************************************/
#include "ws_task.h"
#include "main.h"
#include "pid.h"
#include "kernel.h"
#include "usart.h"
#include "string.h"
int test;
extern uint8_t Load_flag;
extern short EncoderLCount;
extern short EncoderRCount;
extern float MotorLSpeed;
extern float MotorRSpeed;
extern float MotorLSpeedFilter; // 整形后
extern float MotorRSpeedFilter; // 整形后
extern float MotorLSetpossum;
extern float MotorRSetpossum;

static uint8_t recive_and_load_ok = 0;
static uint8_t recive_ok = 0;

extern uint8_t rx_buffer1[128];
extern uint8_t rx_buffer1[128];


uint8_t Round = 0;

//   升级状态标志
char appUpdataIfg = 0;
extern float Weight;

char TargetRoom = 'N', Nextmove = 'N';
u8 Do_count = 0, Do2_count = 0;
;
int WS_OS_Task_Openmv_Dynamic_Recognition_Target(void *data)
{
		static uint8_t value;
    WS_OLED_Printf(0, 2, 0, "                   ");
		WS_OLED_Printf(0, 2, 0, "OPENMV:%s  ", rx_buffer1);
    WS_OLED_Printf(0, 1, 0, "TargetRoom *%c*", TargetRoom);	  
    value = atoi((const char *)rx_buffer1);
	
    switch (value)
    {
    case 1:
        LEDBON;
        TargetRoom = 'A';
        WS_OLED_Printf(0, 3, 0, "wait load");
        WS_OS_Start_Task(WS_OS_Task_LoadOrNot_define, 0);
        break;
    case 2:
        LEDBON;
        TargetRoom = 'B';
        WS_OLED_Printf(0, 3, 0, "wait load");
        WS_OS_Start_Task(WS_OS_Task_LoadOrNot_define, 0);
        break;
    case 3:
        LEDBON;
        TargetRoom = 'C';
        WS_OLED_Printf(0, 3, 0, "wait load");
        WS_OS_Start_Task(WS_OS_Task_LoadOrNot_define, 0);
        break;
    case 4:
        LEDBON;
        TargetRoom = 'D';
        WS_OLED_Printf(0, 3, 0, "wait load");
        WS_OS_Start_Task(WS_OS_Task_LoadOrNot_define, 0);
        break;
    case 5:
        LEDBON;
        TargetRoom = 'E';
        WS_OLED_Printf(0, 3, 0, "wait load");
        WS_OS_Start_Task(WS_OS_Task_LoadOrNot_define, 0);
        break;
    case 6:
        LEDBON;
        TargetRoom = 'F';
        WS_OLED_Printf(0, 3, 0, "wait load");
        WS_OS_Start_Task(WS_OS_Task_LoadOrNot_define, 0);
        break;
    case 7:
        LEDBON;
        TargetRoom = 'G';
        WS_OLED_Printf(0, 3, 0, "wait load");
        WS_OS_Start_Task(WS_OS_Task_LoadOrNot_define, 0);
        break;
    case 8:
        LEDBON;
        TargetRoom = 'H';
        WS_OLED_Printf(0, 3, 0, "wait load");
        WS_OS_Start_Task(WS_OS_Task_LoadOrNot_define, 0);
        break;
    case 40:
        if (TargetRoom == 'D')
        {
            Nextmove = 'L';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    case 41:
        if (TargetRoom == 'D')
        {
            Nextmove = 'R';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    case 42:
        if (TargetRoom == 'D')
        {
            Nextmove = 'L';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    case 43:
        if (TargetRoom == 'D')
        {
            Nextmove = 'R';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    case 30:
        if (TargetRoom == 'C')
        {
            Nextmove = 'L';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    case 31:
        if (TargetRoom == 'C')
        {
            Nextmove = 'R';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    case 32:
        if (TargetRoom == 'C')
        {
            Nextmove = 'L';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    case 33:
        if (TargetRoom == 'C')
        {
            Nextmove = 'R';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    case 50:
        if (TargetRoom == 'E')
        {
            Nextmove = 'L';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    case 51:
        if (TargetRoom == 'E')
        {
            Nextmove = 'R';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    case 52:
        if (TargetRoom == 'E')
        {
            Nextmove = 'L';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    case 53:
        if (TargetRoom == 'E')
        {
            Nextmove = 'R';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    case 60:
        if (TargetRoom == 'F')
        {
            Nextmove = 'L';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    case 61:
        if (TargetRoom == 'F')
        {
            Nextmove = 'R';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    case 62:
        if (TargetRoom == 'F')
        {
            Nextmove = 'L';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    case 63:
        if (TargetRoom == 'F')
        {
            Nextmove = 'R';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    case 70:
        if (TargetRoom == 'G')
        {
            Nextmove = 'L';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        else if (TargetRoom == 'E')
        {
            Nextmove = 'R';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    case 71:
        if (TargetRoom == 'G')
        {
            Nextmove = 'R';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        else if (TargetRoom == 'E')
        {
            Nextmove = 'L';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    case 72:
        if (TargetRoom == 'G')
        {
            Nextmove = 'L';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        else if (TargetRoom == 'E')
        {
            Nextmove = 'R';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    case 73:
        if (TargetRoom == 'G')
        {
            Nextmove = 'R';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        else if (TargetRoom == 'E')
        {
            Nextmove = 'L';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    case 80:
        if (TargetRoom == 'H')
        {
            Nextmove = 'L';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        else if (TargetRoom == 'F')
        {
            Nextmove = 'R';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    case 81:
        if (TargetRoom == 'H')
        {
            Nextmove = 'R';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        else if (TargetRoom == 'F')
        {
            Nextmove = 'L';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    case 82:
        if (TargetRoom == 'H')
        {
            Nextmove = 'L';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        else if (TargetRoom == 'F')
        {
            Nextmove = 'R';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    case 83:
        if (TargetRoom == 'H')
        {
            Nextmove = 'R';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        else if (TargetRoom == 'F')
        {
            Nextmove = 'L';
            WS_OLED_Printf(0, 4, 0, "NextMove*	%c", Nextmove);
        }
        break;
    }
    memset(rx_buffer1, 0, sizeof(rx_buffer1)); // Clear rx_buffer1 after each read
    HAL_UART_Receive_DMA(&huart1, rx_buffer1, sizeof(rx_buffer1));
    return 17;
}

char Nextmovetmp = 'N', Nextmovetmp2 = 'N';
extern uint8_t KEYstatus;

int WS_OS_Task_Goto12(void *data)
{
    if (Load_flag == 1)
    {
        if (TargetRoom == 'A')
        {
            switch (Do_count)
            {
            case 0: {
                WS_Debug("Task2!");
                Do_count++;
                test=Car_go(76);
            }
            break;

            case 1:
                if (Stop_Flag == 1)
                {
                    Do_count++;
                    spin_Turn(left_90);
                }
                break;

            case 2:
                if (Spin_succeed_flag == 1)
                {
                    Do_count++;
                    test=Car_go(50);
                }
                break;

            case 3:
                if (Stop_Flag == 1)
                {
                    LEDRON;
                }
                break;
            }
        }
        else if (TargetRoom == 'B')
        {
            switch (Do_count)
            {
            case 0: {
                Do_count++;
                WS_Debug("Task2!");
                test=Car_go(76);
										}
            break;

            case 1:
                if (Stop_Flag == 1)
                {
                    Do_count++;
                    spin_Turn(right_90);
                }
                break;

            case 2:
                if (Spin_succeed_flag == 1)
                {
                    Do_count++;
                    test=Car_go(50);
                }
                break;

            case 3:
                if (Stop_Flag == 1)
                {
                    LEDRON;
                }
                break;
            }
        }
    }
    else if (Load_flag == 2)
    {
        if (TargetRoom == 'A')
        {
            switch (Do2_count)
            {
            case 0:

                Do2_count++;
                spin_Turn(back_190);
                LEDROFF;

                break;

            case 1:
                if (Spin_succeed_flag == 1)
                {
                    Do2_count++;

                    test=Car_go(51);
                }
                break;

            case 2:
                if (Stop_Flag == 1)
                {
                    Do2_count++;
                    spin_Turn(right_90);
                }
                break;

            case 3:
                if (Spin_succeed_flag == 1)
                {
                    Do2_count++;
										test=Car_go(79);
                    if (KEYstatus == 0)
                    {
                        test=Car_go(79);
                    }
                    else if (KEYstatus == 1)
                    {
                        test=Car_go(79);
                    }
                }
                break;

            case 4:
                if (Stop_Flag == 1)
                {
                    LEDGON;
                    if (KEYstatus == 1)
                    {
                        Do2_count++;
                        WS_Delay_US(200000);
                        spin_Turn(back_180);
                        WS_Delay_US(200000);
                    }
                }
                break;
            case 5:
                WS_Debug("Task1!");
                NVIC_SystemReset();
                break;
            }
        }
        else if (TargetRoom == 'B')
        {
            switch (Do2_count)
            {
            case 0: {
                Do2_count++;

                spin_Turn(back_190);
                LEDROFF;
            }
            break;

            case 1:
                if (Spin_succeed_flag == 1)
                {
                    Do2_count++;
                    test=Car_go(51);
                }
                break;

            case 2:
                if (Stop_Flag == 1)
                {
                    Do2_count++;
                    spin_Turn(left_90);
                }
                break;

            case 3:
                if (Spin_succeed_flag == 1)
                {
                    Do2_count++;
										test=Car_go(79);
                    if (KEYstatus == 0)
                    {
                        test=Car_go(79);
                    }

                    else if (KEYstatus == 1)
                    {
                        test=Car_go(79);
                    }
                }
                break;

            case 4:
                if (Stop_Flag == 1)
                {
                    LEDGON;
                    if (KEYstatus == 1)
                    {
                        Do2_count++;
                        WS_Delay_US(200000);
                        spin_Turn(back_180);
                        WS_Delay_US(200000);
                    }
                }
                break;
            case 5:
                WS_Debug("Task1!");
                NVIC_SystemReset();
                break;
            }
        }
    }
    return 3;
}

int WS_OS_Task_Dynamic_Recognition(void *data)
{
    if (Load_flag == 1)
    {
        if (TargetRoom == 'A' || TargetRoom == 'B')
        {
            WS_OS_Start_Task(WS_OS_Task_Goto12_define, 0);
        }
        else if (TargetRoom == 'C' || TargetRoom == 'D')
        {
            switch (Do_count)
            {
            case 0: {
                WS_Debug("Task2!");
                Do_count++;
                test=Car_go(165 + 10 - viewdis); //*主要修改viewdis*
                                            ////此时 Stop_Flag ==0  完成后自动置1
                Nextmove = 'N';
            }
            break;

            case 1:

            if (Nextmove == 'L' || Nextmove == 'R')
                {
                    if (Stop_Flag == 1)
                    {
                        Nextmovetmp = Nextmove;
                        Do_count++;
                        test=Car_go(viewdis - 5);
                    }
                }
                break;
            case 2:
                if (Nextmovetmp == 'L')
                {
                    if (Stop_Flag == 1)
                    {
                        Do_count++;
                        spin_Turn(left_90);
                    }
                    break;
                }
                if (Nextmovetmp == 'R')
                {
                    if (Stop_Flag == 1)
                    {
                        Do_count++;
                        spin_Turn(right_90);
                    }
                    break;
                }

            case 3:
                if (Spin_succeed_flag == 1)
                {
                    Do_count++;
                    test=Car_go(49);
                }
                break;

            case 4:
                if (Stop_Flag == 1)
                {
                    LEDRON;
                    LEDBOFF;
                }
                break;
            }
        }
        else if (TargetRoom == 'E' || TargetRoom == 'F' || TargetRoom == 'G' || TargetRoom == 'H')
        {
            switch (Do_count)
            {
            case 0: {
                Do_count++;
                test=Car_go(258 - viewdis + 4); // 主要修改viewdis
                                           // //此时 Stop_Flag ==0  完成后自动置1
                Nextmove = 'N';
            }
            break;

            case 1:
                WS_Debug("Task2!");
                if (Nextmove == 'L' || Nextmove == 'R')
                {
                    if (Stop_Flag == 1)
                    {
                        if (TargetRoom == 'E')
                        {
                            Nextmovetmp = (Nextmove == 'L') ? 'R' : 'L';
                        }
                        else if (TargetRoom == 'F')
                        {
                            Nextmovetmp = (Nextmove == 'L') ? 'R' : 'L';
                        }
                        else
                        {
                            Nextmovetmp = Nextmove;
                        }
                        // 临时变量1存储第一次转向
                        Nextmove = 'N';
                        Do_count++;
                        test=Car_go(viewdis - 2); // 主要修改viewdis
                    }
                }
                break;
            case 2:
                if (Nextmovetmp == 'L')
                {
                    if (Stop_Flag == 1)
                    {
                        Do_count++;
                        Nextmove = 'N';
                        spin_Turn(left_90);
                    }
                    break;
                }
                if (Nextmovetmp == 'R')
                {
                    if (Stop_Flag == 1)
                    {
                        Do_count++;
                        Nextmove = 'N';
                        spin_Turn(right_90);
                    }
                    break;
                }

            case 3:
                if (Spin_succeed_flag == 1)
                {
                    Nextmove = 'N';
                    Do_count++;
                    test=Car_go(96 - viewdis + 3); // 主要修改viewdis
                }
                break;
            case 4:
                if (Nextmove == 'L' || Nextmove == 'R')
                {
                    if (Stop_Flag == 1)
                    {
                        if (TargetRoom == 'E')
                        {
                            Nextmovetmp2 = (Nextmove == 'L') ? 'L' : 'R';
                        }
                        else if (TargetRoom == 'F')
                        {
                            Nextmovetmp2 = (Nextmove == 'L') ? 'L' : 'R';
                        }
                        else
                        {
                            Nextmovetmp2 = Nextmove;
                        }
                        Nextmove = 'N';
                        Do_count++;
                        test=Car_go(viewdis - 3); // 主要修改viewdis
                    }
                }
                break;
            case 5:
                if (Nextmovetmp2 == 'L')
                {
                    if (Stop_Flag == 1)
                    {
                        Do_count++;
                        spin_Turn(left_90);
                    }
                    break;
                }
                if (Nextmovetmp2 == 'R')
                {
                    if (Stop_Flag == 1)
                    {
                        if (TargetRoom == 'F')
                        {
                            Do_count++;
                            spin_Turn(right_90);
                        }
                        else
                        {
                            Do_count++;
                            spin_Turn(right_90);
                        }
                    }
                    break;
                }
            case 6:
                if (Spin_succeed_flag == 1)
                {
                    Do_count++;
                    test=Car_go(50);
                }
                break;
            case 7:
                if (Stop_Flag == 1)
                {
                    LEDRON;
                    LEDBOFF;
                }
                break;
            }
        }
    }

    else if (Load_flag == 2)
    {
        if (TargetRoom == 'A' || TargetRoom == 'B')
        {
            WS_OS_Start_Task(WS_OS_Task_Goto12_define, 0);
        }
        else if (TargetRoom == 'C' || TargetRoom == 'D')
        {
            switch (Do2_count)
            {
            case 0: {
                Do2_count++;
                spin_Turn(back_180);
                LEDROFF;
            }
            break;
            case 1:
                if (Spin_succeed_flag == 1)
                {
                    Do2_count++;
                    test=Car_go(50);
                }
                break;
            case 2:
                if (Nextmovetmp == 'L')
                {
                    if (Stop_Flag == 1)
                    {
                        Do2_count++;
                        spin_Turn(right_90);
                    }
                }
                else if (Nextmovetmp == 'R')
                {
                    if (Stop_Flag == 1)
                    {
                        Do2_count++;
                        spin_Turn(left_90);
                    }
                }
                break;
            case 3:
                if (Spin_succeed_flag == 1)
                {
                    Do2_count++;
                    if (KEYstatus == 0)
                    {
                        test=Car_go(164);
                    }
                    else if (KEYstatus == 1)
                    {
                        test=Car_go(164);
                    }
                }
                break;

            case 4:
                if (Stop_Flag == 1)
                {
                    LEDGON;
                    if (KEYstatus == 1)
                    {
                        Do2_count++;
                        WS_Delay_US(200000);
                        spin_Turn(back_190);
                        WS_Delay_US(400000);
                    }
                }
                break;
            case 5:
                WS_Debug("Task1!");
                NVIC_SystemReset();
                break;
            }
        }
        else if (TargetRoom == 'E' || TargetRoom == 'F' || TargetRoom == 'G' || TargetRoom == 'H')
        {
            switch (Do2_count)
            {
            case 0: {
                Do2_count++;
                spin_Turn(back_190);
                LEDROFF;
            }
            break;
            case 1:
                if (Spin_succeed_flag == 1)
                {
                    Do2_count++;
                    test=Car_go(51);
                }
                break;
            case 2:
                if (Nextmovetmp2 == 'L')
                {
                    if (Stop_Flag == 1)
                    {
                        Do2_count++;
                        spin_Turn(right_90);
                    }
                }
                else if (Nextmovetmp2 == 'R')
                {
                    if (Stop_Flag == 1)
                    {
                        Do2_count++;
                        spin_Turn(left_90);
                    }
                }
                break;
            case 3:
                if (Spin_succeed_flag == 1)
                {
                    Do2_count++;
                    test=Car_go(94);
                }
                break;
            case 4:
                if (Nextmovetmp == 'L')
                {
                    if (Stop_Flag == 1)
                    {
                        Do2_count++;
                        spin_Turn(right_90);
                    }
                }
                else if (Nextmovetmp == 'R')
                {
                    if (Stop_Flag == 1)
                    {
                        Do2_count++;
                        spin_Turn(left_90);
                    }
                }
                break;
            case 5:
                if (Spin_succeed_flag == 1)
                {
                    Do2_count++;
                    if (KEYstatus == 0)
                    { // 单次模式 = 0
                        test=Car_go(252);
                    }
                    else if (KEYstatus == 1)
                    { // 连续模式 = 1
                        test=Car_go(252);
                    }
                }
                break;
            case 6:
                if (Stop_Flag == 1)
                {
                    LEDGON;
                    if (KEYstatus == 1)
                    {
                        Do2_count++;
                        WS_Delay_US(100000);
                        spin_Turn(back_190);
                        WS_Delay_US(400000); // 此延时目的是为了确保掉头完成
                    }
                }
                break;
            case 7:
                WS_Debug("Task1!"); // 自动reset之前给openmv发送装载指令
                NVIC_SystemReset(); // 自动reset
                break;
            }
        }
    }
    return 7;
}

uint8_t farornear = 0;
uint8_t normal = 0;
uint16_t count = 0;

int WS_OS_Task_Dynamic_Recognitiondd(void *data)
{
    if (Load_flag == 1)
    {
        if (TargetRoom == 'A' || TargetRoom == 'B')
        {
            WS_OS_Start_Task(WS_OS_Task_Goto12_define, 0);
        }
        else if (TargetRoom == 'C' || TargetRoom == 'D' || TargetRoom == 'E' || TargetRoom == 'F' ||
                 TargetRoom == 'G' || TargetRoom == 'H')
        {
            switch (normal)
            {
            case 0: {
                WS_Debug("Task2!");
                normal++;
                test=Car_go(165 + 10 - viewdis); //*主要修改viewdis*
                                            ////此时 Stop_Flag ==0  完成后自动置1
                Nextmove = 'N';
            }
            break;
            case 1: {
                if (Stop_Flag == 1)
                {
                    WS_Delay_US(1000);
                    count++;
                    WS_OLED_Printf(0, 4, 0, "Count = %d", count);
                    if (count >= 6)
                    {
                        normal++;
                        farornear = 2;
                        Do_count =  2;
                        Nextmovetmp = 'N';
                    }
                    else if (Nextmove == 'L' || Nextmove == 'R')
                    {
                        Nextmovetmp = Nextmove;
                        normal++;
                    }
                }
            }
            break;
            case 2:
                if (Nextmovetmp == 'L' || Nextmovetmp == 'R')
                {
                    normal++;
                    if (Stop_Flag == 1)
                    {
                        Nextmovetmp = Nextmove;
                        farornear = 1;
                        test=Car_go(viewdis - 5);
                        Do_count = 2;
                    }
                }
                else
                {
                    normal++;
                }
                break;
            case 3: {
                if (farornear == 1)
                {
                    switch (Do_count)
                    {
                    case 2:
                        if (Nextmovetmp == 'L')
                        {
                            if (Stop_Flag == 1)
                            {
                                Do_count++;
                                spin_Turn(left_90);
                            }
                            break;
                        }
                        if (Nextmovetmp == 'R')
                        {
                            if (Stop_Flag == 1)
                            {
                                Do_count++;
                                spin_Turn(right_90);
                            }
                            break;
                        }

                    case 3:
                        if (Spin_succeed_flag == 1)
                        {
                            Do_count++;
                            test=Car_go(49);
                        }
                        break;

                    case 4:
                        if (Stop_Flag == 1)
                        {
                            LEDRON;
                            LEDBOFF;
                        }
                        break;
                    }
                }
                else if (farornear == 2)
                {
                    switch (Do_count)
                    {
                    case 2: {
                        Do_count++;
                        test=Car_go(258 - viewdis + 4 - (165 + 10 - viewdis)); // 主要修改viewdis
                                                                          // //此时 Stop_Flag ==0  完成后自动置1
                        Nextmove = 'N';
                    }
                    break;

                    case 3:
                        if (Nextmove == 'L' || Nextmove == 'R')
                        {
                            if (Stop_Flag == 1)
                            {
                                if (TargetRoom == 'E')
                                {
                                    Nextmovetmp = (Nextmove == 'L') ? 'R' : 'L';
                                }
                                else if (TargetRoom == 'F')
                                {
                                    Nextmovetmp = (Nextmove == 'L') ? 'R' : 'L';
                                }
                                else
                                {
                                    Nextmovetmp = Nextmove;
                                }
                                // 临时变量1存储第一次转向
                                Nextmove = 'N';
                                Do_count++;
                                test=Car_go(viewdis - 3); // 主要修改viewdis
                            }
                        }
                        break;
                    case 4:
                        if (Nextmovetmp == 'L')
                        {
                            if (Stop_Flag == 1)
                            {
                                Do_count++;
                                Nextmove = 'N';
                                spin_Turn(left_90);
                            }
                            break;
                        }
                        if (Nextmovetmp == 'R')
                        {
                            if (Stop_Flag == 1)
                            {
                                Do_count++;
                                Nextmove = 'N';
                                spin_Turn(right_90);
                            }
                            break;
                        }

                    case 5:
                        if (Spin_succeed_flag == 1)
                        {
                            Nextmove = 'N';
                            Do_count++;
                            test=Car_go(96 - viewdis + 3); // 主要修改viewdis
                        }
                        break;
                    case 6:
                        if (Nextmove == 'L' || Nextmove == 'R')
                        {
                            if (Stop_Flag == 1)
                            {
                                if (TargetRoom == 'E')
                                {
                                    Nextmovetmp2 = (Nextmove == 'L') ? 'L' : 'R';
                                }
                                else if (TargetRoom == 'F')
                                {
                                    Nextmovetmp2 = (Nextmove == 'L') ? 'L' : 'R';
                                }
                                else
                                {
                                    Nextmovetmp2 = Nextmove;
                                }
                                Nextmove = 'N';
                                Do_count++;
                                test=Car_go(viewdis - 3); // 主要修改viewdis
                            }
                        }
                        break;
                    case 7:
                        if (Nextmovetmp2 == 'L')
                        {
                            if (Stop_Flag == 1)
                            {
                                Do_count++;
                                spin_Turn(left_90);
                            }
                            break;
                        }
                        if (Nextmovetmp2 == 'R')
                        {
                            if (Stop_Flag == 1)
                            {
                                if (TargetRoom == 'F')
                                {
                                    Do_count++;
                                    spin_Turn(right_90);
                                }
                                else
                                {
                                    Do_count++;
                                    spin_Turn(right_90);
                                }
                            }
                            break;
                        }
                    case 8:
                        if (Spin_succeed_flag == 1)
                        {
                            Do_count++;
                            test=Car_go(50);
                        }
                        break;
                    case 9:
                        if (Stop_Flag == 1)
                        {
                            LEDRON;
                            LEDBOFF;
                        }
                        break;
                    }
                }
            }
            break;
            }
        }
    }
    else if (Load_flag == 2)
    {
        if (farornear == 1)
        {
            switch (Do2_count)
            {
            case 0: {
                Do2_count++;
                spin_Turn(back_190);
                LEDROFF;
            }
            break;
            case 1:
                if (Spin_succeed_flag == 1)
                {
                    Do2_count++;
                    test=Car_go(49);
                }
                break;
            case 2:
                if (Nextmovetmp == 'L')
                {
                    if (Stop_Flag == 1)
                    {
                        Do2_count++;
                        spin_Turn(right_90);
                    }
                }
                else if (Nextmovetmp == 'R')
                {
                    if (Stop_Flag == 1)
                    {
                        Do2_count++;
                        spin_Turn(left_90);
                    }
                }
                break;
            case 3:
                if (Spin_succeed_flag == 1)
                {
                    Do2_count++;
                    if (KEYstatus == 0)
                    {
                        test=Car_go(164);
                    }
                    else if (KEYstatus == 1)
                    {
                        test=Car_go(164);
                    }
                    else if (KEYstatus == 2)
                    {
                        test=Car_go(164);
                    }
                }
                break;

            case 4:
                if (Stop_Flag == 1)
                {
                    LEDGON;
                    if (KEYstatus == 1 || KEYstatus == 2)
                    {
                        Do2_count++;
                        WS_Delay_US(200000);
                        spin_Turn(back_190);
                        WS_Delay_US(400000);
                    }
                }
                break;
            case 5:
                WS_Debug("Task1!");
                NVIC_SystemReset();
                break;
            }
        }
        else if (farornear == 2)
        {
            switch (Do2_count)
            {
            case 0: {
                Do2_count++;
                spin_Turn(back_190);
                LEDROFF;
            }
            break;
            case 1:
                if (Spin_succeed_flag == 1)
                {
                    Do2_count++;
                    test=Car_go(51);
                }
                break;
            case 2:
                if (Nextmovetmp2 == 'L')
                {
                    if (Stop_Flag == 1)
                    {
                        Do2_count++;
                        spin_Turn(right_90);
                    }
                }
                else if (Nextmovetmp2 == 'R')
                {
                    if (Stop_Flag == 1)
                    {
                        Do2_count++;
                        spin_Turn(left_90);
                    }
                }
                break;
            case 3:
                if (Spin_succeed_flag == 1)
                {
                    Do2_count++;
                    test=Car_go(94);
                }
                break;
            case 4:
                if (Nextmovetmp == 'L')
                {
                    if (Stop_Flag == 1)
                    {
                        Do2_count++;
                        spin_Turn(right_90);
                    }
                }
                else if (Nextmovetmp == 'R')
                {
                    if (Stop_Flag == 1)
                    {
                        Do2_count++;
                        spin_Turn(left_90);
                    }
                }
                break;
            case 5:
                if (Spin_succeed_flag == 1)
                {
                    Do2_count++;
                    if (KEYstatus == 0)
                    { // 单次模式 = 0
                        test=Car_go(250);
                    }
                    else if (KEYstatus == 1)
                    { // 连续模式 = 1
                        test=Car_go(250);
                    }
                    else if (KEYstatus == 2)
                    { // 连续模式 = 1
                        test=Car_go(250);
                    }
                }
                break;
            case 6:
                if (Stop_Flag == 1)
                {
                    LEDGON;
                    if (KEYstatus == 1 || KEYstatus == 2)
                    {
                        Do2_count++;
                        WS_Delay_US(100000);
                        spin_Turn(back_190);
                         WS_Delay_US(400000); // 此延时目的是为了确保掉头完成
                    }
                }
                break;
            case 7:
                WS_Debug("Task1!"); // 自动reset之前给openmv发送装载指令
                NVIC_SystemReset(); // 自动reset
                break;
            }
        }
    }
    return 7;
}

int WS_OS_Task_LoadOrNot(void *data)
{
    if (Load_flag == 1)
    {
        if (KEYstatus == 2)
        {
            WS_OS_Start_Task(WS_OS_Task_Dynamic_Recognitiondd_define, 0);
            WS_OLED_Printf(0, 5, 0, "Dynamic_Recog*2*");
        }
        else if (KEYstatus == 1)
        {
            WS_OS_Start_Task(WS_OS_Task_Dynamic_Recognition_define, 0);
            WS_OLED_Printf(0, 5, 0, "Dynamic_Recognition");
        }
    }
    return 2;
}
int WS_OS_Task_Load(void *data)
{
		LoadOrNot();
    return 100;
}
/******************************************************************************
 *方法名称： WS_OS_TASK_INIT(void)
 *功能：     WS_OS  任务创建  , 必须放在板载资源初始化之前
 *参数：     无
 *返回:			无
 *******************************************************************************/
void WS_OS_Task_Create(void)
{
    WS_OS_Init_Task();
	
	  WS_OS_Create_Task(WS_OS_Task_Load_define,WS_OS_Task_Load,1);
		WS_OS_Create_Task(WS_OLED_Task_Buffer_Update_define,WS_OLED_Task_Buffer_Update,1); 
    WS_OS_Create_Task(WS_OS_Task_Openmv_Dynamic_Recognition_Target_define, WS_OS_Task_Openmv_Dynamic_Recognition_Target,10);
    WS_OS_Create_Task(WS_OS_Task_LoadOrNot_define, WS_OS_Task_LoadOrNot, 1);
    WS_OS_Create_Task(WS_OS_Task_Dynamic_Recognition_define, WS_OS_Task_Dynamic_Recognition, 0);
    WS_OS_Create_Task(WS_OS_Task_Dynamic_Recognitiondd_define, WS_OS_Task_Dynamic_Recognitiondd, 0);
    WS_OS_Create_Task(WS_OS_Task_Goto12_define, WS_OS_Task_Goto12, 0); // 1,2死库
	  WS_OS_Create_Task(WS_LoRa_Task_ReceData_define,WS_LoRa_Task_ReceData,10);
}
