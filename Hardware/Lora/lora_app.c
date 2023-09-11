#include "string.h"
#include "stdarg.h"
#include "stdio.h"
#include "usart.h"
#include "main.h"
#include "tim.h"

#include "lora_app.h"
#include "lora_cfg.h"

/*��¼����״̬*/
uint16_t USART3_RX_STA=0;  
uint8_t USART3_TX_BUF[USART3_MAX_SEND_LEN]; 	      //���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
uint8_t  USART3_RX_BUF[USART3_MAX_RECV_LEN] ;
/************************************************
 ALIENTEK ������STM32F429������
 ATK-LORA-01ģ�鹦������
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com  
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

//�豸������ʼ��(�����豸������lora_cfg.h����)
_LoRa_CFG LoRa_CFG=
{
	.addr = LORA_ADDR,       //�豸��ַ
	.power = LORA_POWER,     //���书��
	.chn = LORA_CHN,         //�ŵ�
	.wlrate = LORA_RATE,     //��������
	.wltime = LORA_WLTIME,   //˯��ʱ��
	.mode = LORA_MODE,       //����ģʽ
	.mode_sta = LORA_STA,    //����״̬
	.bps = LORA_TTLBPS ,     //����������
	.parity = LORA_TTLPAR    //У��λ����
};

//�豸����ģʽ(���ڼ�¼�豸״̬)
uint8_t Lora_mode=0;// 0:����ģʽ 1:����ģʽ 2:����ģʽ

uint32_t obj_addr=0;//��¼Ŀ���ַ���ŵ���������ʾ���Կ����Ͻǣ����û�ͨ��������������ʱ������99999,������Ҫ����uint32_t���ͣ�
uint8_t obj_chn=0;

//���յ���ATָ��Ӧ�����ݷ��ظ����Դ���
//mode:0,������USART3_RX_STA;
//     1,����USART3_RX_STA;
void lora_at_response(uint8_t mode)
{
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
		if(mode)USART3_RX_STA=0;
	} 
}

//lora���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//����,�ڴ�Ӧ������λ��(str��λ��)
uint8_t* lora_check_cmd(uint8_t *str)
{
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (uint8_t*)strx;
}

//lora��������
//cmd:���͵������ַ���(����Ҫ��ӻس���),��cmd<0XFF��ʱ��,��������(���緢��0X1A),���ڵ�ʱ�����ַ���.
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
uint8_t lora_send_cmd(uint8_t *cmd,uint8_t *ack,uint16_t waittime)
{
	uint8_t res=0; 
	USART3_RX_STA=0;
 
	if((uint32_t)cmd<=0XFF)
	{			
		while((USART3->SR&0X40)==0);//�ȴ���һ�����ݷ������  
		USART3->DR=(uint32_t)cmd;
		
	}else u3_printf("%s\r\n",cmd);//��������
	
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
	   while(--waittime)	//�ȴ�����ʱ
	   { 
		  HAL_Delay(10);		
		  if(USART3_RX_STA&0X8000)//����Ӧ��
		  {			
			  if(lora_check_cmd(ack))//���յ��ڴ���Ӧ����
			  {	
				  USART3_RX_STA=0;
				  break;//�õ���Ч����
			  }  
		  } 
	   }
	   if(waittime==0)res=1; 
	}
	return res;
} 

//��¼�ж�״̬
static uint8_t Rec_mode=0;//0���ر� 1:������ 2:�½���
//AUX�ж�����
//mode:���õ�ģʽ 0:�ر� 1:������ 2:�½���
void Aux_Int(uint8_t mode)
{
	
	GPIO_InitTypeDef GPIO_Initure={0};
	if(!mode)
	{
		HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);//ʧ���ж���11
	}else
	{
		if(mode==1)
		{
			
			 GPIO_Initure.Pin=lora_AUX_Pin; 
       GPIO_Initure.Mode=GPIO_MODE_IT_RISING;//�����ش���
		}
			
	    else if(mode==2)
		{
			 GPIO_Initure.Pin=lora_AUX_Pin; 
       GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;//�½��ش���
		}
		
		 HAL_GPIO_Init(lora_AUX_GPIO_Port,&GPIO_Initure);//���³�ʼ��
	     __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_11);//����жϱ�־λ
		 HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,0); //��ռ���ȼ�Ϊ2�������ȼ�Ϊ0
		 HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);//ʹ��
	 
	}
	Rec_mode = mode;//��¼�ж�ģʽ
}

void usart3_set(uint8_t bps,uint8_t parity)
{

  static uint32_t bound=0;
	
	switch(bps)
	{
		case LORA_TTLBPS_1200:   bound=1200;     break;
		case LORA_TTLBPS_2400:   bound=2400;     break;
		case LORA_TTLBPS_4800:   bound=4800;     break;
		case LORA_TTLBPS_9600:   bound=9600;     break;
		case LORA_TTLBPS_19200:  bound=19200;    break;
		case LORA_TTLBPS_38400:  bound=38400;    break;
		case LORA_TTLBPS_57600:  bound=57600;    break;
		case LORA_TTLBPS_115200: bound=115200;   break;
	} 
    __HAL_UART_DISABLE(&huart3);//�رմ���
	
	huart3.Init.BaudRate = bound;
	huart3.Init.StopBits = UART_STOPBITS_1; 
	
	if(parity==LORA_TTLPAR_8N1)//��У��
	{
		huart3.Init.WordLength= UART_WORDLENGTH_8B;    
		huart3.Init.Parity = UART_PARITY_NONE;
	}else if(parity==LORA_TTLPAR_8E1)//żУ��
	{
		huart3.Init.WordLength= UART_WORDLENGTH_9B;    
		huart3.Init.Parity = UART_PARITY_EVEN;
	}else if(parity==LORA_TTLPAR_8O1)//��У��
	{
		huart3.Init.WordLength = UART_WORDLENGTH_9B;    
		huart3.Init.Parity = UART_PARITY_ODD;
	}
    HAL_UART_Init(&huart3);	
	
} 

//LoRaģ���ʼ��
//����ֵ:0,���ɹ�
//       1,���ʧ�� 
uint8_t LoRa_Init(void)
{
	 uint8_t retry=0;
	 uint8_t temp;

	LORA_MD0(0);
	 
	while(LORA_AUX)//ȷ��LORAģ���ڿ���״̬��(LORA_AUX=0)
	{
		 WS_Debug("BUSY!!"); 	 
	}
	LORA_MD0(1);//����ATģʽ
	HAL_Delay(40); 
	retry=3;
	while(retry--)
	  {
		
		 if(!lora_send_cmd((uint8_t *)"AT",(uint8_t *)"OK",70))
		 {
			 temp=0;//���ɹ�
			 WS_Debug("LoRa_Init OK\r\n"); 	 
			 break;
		 }
		 	 
	  if(retry==0) {
			 WS_Debug("LoRa_Init ERROR\r\n"); 	 
			 temp=1;//���ʧ��
			}
		 
	  }
	  return temp;
}

//����3,printf ����
//ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
char formatBuf3[64];
void u3_printf(char *p, ...)
{
    /*  ��ʽ������  */
    va_list ap;
    va_start(ap, p);
    vsprintf(formatBuf3, p, ap);
    va_end(ap);
    HAL_UART_Transmit_DMA(&huart3, (uint8_t *)formatBuf3, strlen(formatBuf3));
}

//���ڽ���ʹ�ܿ���
//enable:0,�ر� 1,��
void usart3_rx(uint8_t enable)
{
	
	 __HAL_UART_DISABLE(&huart3); //ʧ�ܴ��� 
	
	 if(enable)
	 {
		 huart3.Init.Mode=UART_MODE_TX_RX;//�շ�ģʽ
	 }else
	 {
		  huart3.Init.Mode = UART_MODE_TX;//ֻ����  
	 }
	 
	 HAL_UART_Init(&huart3); //HAL_UART_Init()��ʹ��UART3
}

//Loraģ���������
void LoRa_Set(void)
{
	uint8_t sendbuf[20];
	uint8_t lora_addrh,lora_addrl=0;
	
	usart3_set(LORA_TTLBPS_115200,LORA_TTLPAR_8N1);//��������ģʽǰ����ͨ�Ų����ʺ�У��λ(115200 8λ���� 1λֹͣ ������У�飩
	usart3_rx(1);//��������3����
	
	while(LORA_AUX);//�ȴ�ģ�����
	LORA_MD0(1); //��������ģʽ
	HAL_Delay(40);
	Lora_mode=0;//���"����ģʽ"
	
	lora_addrh =  (LoRa_CFG.addr>>8)&0xff;
	lora_addrl = LoRa_CFG.addr&0xff;
	sprintf((char*)sendbuf,"AT+ADDR=%02x,%02x",lora_addrh,lora_addrl);//�����豸��ַ
	lora_send_cmd(sendbuf,(uint8_t *)"OK",50);
	sprintf((char*)sendbuf,"AT+WLRATE=%d,%d",LoRa_CFG.chn,LoRa_CFG.wlrate);//�����ŵ��Ϳ�������
	lora_send_cmd(sendbuf,(uint8_t *)"OK",50);
	sprintf((char*)sendbuf,"AT+TPOWER=%d",LoRa_CFG.power);//���÷��书��
	lora_send_cmd(sendbuf,(uint8_t *)"OK",50);
	sprintf((char*)sendbuf,"AT+CWMODE=%d",LoRa_CFG.mode);//���ù���ģʽ
	lora_send_cmd(sendbuf,(uint8_t *)"OK",50);
	sprintf((char*)sendbuf,"AT+TMODE=%d",LoRa_CFG.mode_sta);//���÷���״̬
	lora_send_cmd(sendbuf,(uint8_t *)"OK",50);
	sprintf((char*)sendbuf,"AT+WLTIME=%d",LoRa_CFG.wltime);//����˯��ʱ��
	lora_send_cmd(sendbuf,(uint8_t *)(uint8_t *)"OK",50);
	sprintf((char*)sendbuf,"AT+UART=%d,%d",LoRa_CFG.bps,LoRa_CFG.parity);//���ô��ڲ����ʡ�����У��λ
	lora_send_cmd(sendbuf,(uint8_t *)"OK",50);

	LORA_MD0(0);//�˳�����,����ͨ��
	HAL_Delay(40);
	while(LORA_AUX);//�ж��Ƿ����(ģ����������ò���)
	USART3_RX_STA=0;
	Lora_mode=1;//���"����ģʽ"
	usart3_set(LoRa_CFG.bps,LoRa_CFG.parity);//����ͨ��,����ͨ�Ŵ�������(�����ʡ�����У��λ)
	Aux_Int(1);//����LORA_AUX�������ж�
	memset((char*)USART3_RX_BUF,0x00,sizeof(USART3_RX_BUF));//���ڽ��ջ�������0
}


uint8_t Dire_Date[]={0x11,0x22,0x33,0x44,0x55};//����������
uint8_t date[30]={0};//��������
uint8_t Tran_Data[30]={0};//͸������

#define Dire_DateLen sizeof(Dire_Date)/sizeof(Dire_Date[0])
	
//Loraģ�鷢������
void LoRa_SendData(void)
{      
	static uint8_t num=0;
  uint16_t addr;
	uint8_t chn;
	uint16_t i=0; 
		
	if(LoRa_CFG.mode_sta == LORA_STA_Tran)//͸������
	{
		sprintf((char*)Tran_Data,"ATK-LORA-01 TEST %d",num);
		u3_printf("%s\r\n",Tran_Data);	
		num++;
		if(num==255) num=0;
		
	}else if(LoRa_CFG.mode_sta == LORA_STA_Dire)//������
	{
		
		addr = (uint16_t)obj_addr;//Ŀ���ַ
		chn = obj_chn;//Ŀ���ŵ�
		
		date[i++] =(addr>>8)&0xff;//��λ��ַ
		date[i++] = addr&0xff;//��λ��ַ
		date[i] = chn;  //�����ŵ�
		for(i=0;i<Dire_DateLen;i++)//����д������BUFF
		{
			date[3+i] = Dire_Date[i];
		}
		HAL_UART_Transmit_DMA(&huart3,date,30);	
	}		
}
void LoRa_ReceData(void)
{
    uint16_t i=0;
    uint16_t len=0;
   
	//����������
	if(USART3_RX_STA&0x8000)
	{
		len = USART3_RX_STA&0X7FFF;
		USART3_RX_BUF[len]=0;//��ӽ�����
		USART3_RX_STA=0;
    WS_Debug("%s",USART3_RX_BUF);
		memset((char*)USART3_RX_BUF,0x00,len);//���ڽ��ջ�������0
	}

}
//  LoRa����
int WS_LoRa_Task_ReceData(void *date)
{	
	
    LoRa_ReceData() ;  
	
	  return 10;
}