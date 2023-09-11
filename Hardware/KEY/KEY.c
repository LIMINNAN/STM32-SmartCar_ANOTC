#include "multi_button.h"
#include "main.h"

enum Button_IDs {
	btn1_id,
	btn2_id,
};

struct Button btn1;
struct Button btn2;

uint8_t read_button_GPIO(uint8_t button_id)
{
	// you can share the GPIO read function with multiple Buttons
	switch(button_id)
	{
		case btn1_id:
			return HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin);
		case btn2_id:
			return HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin);
		default:
			return 0;
	}
}

void BTN1_SINGLE_Click_Handler(void* btn)
{
	LEDRTOG;
	
}
void BTN1_DOUBLE_Click_Handler(void* btn)
{
	LEDBTOG;
}
int KEY_TASK_TEST(void *data)
{
	
	LEDRTOG;
	LEDBTOG;
  return 1000;
}

void BTN1_LONG_PRESS_START_Handler(void* btn)
{

	WS_OS_Start_Task(2,1);
}
extern int8_t testnum;
void BTN1_LONG_PRESS_HOLD_Handler(void* btn)
{
		
}

/*
void BTN1_PRESS_REPEAT_Handler(void* btn)
{
	static uint8_t count =0;
	count++;
	testnum=count;
	if(count == 3)//������
		WS_OS_Start_Task(2,1);

}
*/
void WS_KEY_Init(void)
{
	button_init(&btn1, read_button_GPIO, RESET, btn1_id);
	button_init(&btn2, read_button_GPIO, RESET, btn2_id);

//	button_attach(&btn1, PRESS_DOWN,       BTN1_PRESS_DOWN_Handler);	//�����κ�ʱ�䶼���ȴ�������������һ�¼����������Ӧ�ٶȿ죬��ͬʱ�ᴥ�������¼�������������multi_fuc����ʱʹ�á�
//	button_attach(&btn1, PRESS_UP,         BTN1_PRESS_UP_Handler);    //̧�ּ�������¼����ᴥ��
//	button_attach(&btn1, PRESS_REPEAT,     BTN1_PRESS_REPEAT_Handler); //ֻҪ������������¼�ʱ��������ص�����˻ᴥ��DOUBLE_Click
	button_attach(&btn1, SINGLE_CLICK,     BTN1_SINGLE_Click_Handler);		//����ʱ�����¼��ص�������ᴥ�������¼�����ִ��һ�Ρ�
	button_attach(&btn1, DOUBLE_CLICK,     BTN1_DOUBLE_Click_Handler);		//˫��ʱ�����¼��ص�����ִ��һ�Ρ�
	button_attach(&btn1, LONG_PRESS_START, BTN1_LONG_PRESS_START_Handler);//����ʱ�����¼��ص�����ִ��һ�Ρ�
	button_attach(&btn1, LONG_PRESS_HOLD,  BTN1_LONG_PRESS_HOLD_Handler); //����ʱ�����¼��ص�����������ֽ���һֱִ�лص�����

//	button_attach(&btn2, PRESS_DOWN,       BTN2_PRESS_DOWN_Handler);
//	button_attach(&btn2, PRESS_UP,         BTN2_PRESS_UP_Handler);
//	button_attach(&btn2, PRESS_REPEAT,     BTN2_PRESS_REPEAT_Handler);
//	button_attach(&btn2, SINGLE_CLICK,     BTN2_SINGLE_Click_Handler);
//	button_attach(&btn2, DOUBLE_CLICK,     BTN2_DOUBLE_Click_Handler);
//	button_attach(&btn2, LONG_PRESS_START, BTN2_LONG_PRESS_START_Handler);
//	button_attach(&btn2, LONG_PRESS_HOLD,  BTN2_LONG_PRESS_HOLD_Handler);

	button_start(&btn1);
	button_start(&btn2);
}
