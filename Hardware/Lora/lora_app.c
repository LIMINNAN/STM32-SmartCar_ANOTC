#include "string.h"
#include "stdarg.h"
#include "stdio.h"
#include "usart.h"
#include "main.h"
#include "tim.h"

#include "lora_app.h"
#include "lora_cfg.h"

/*记录数据状态*/
uint16_t USART3_RX_STA=0;  
uint8_t USART3_TX_BUF[USART3_MAX_SEND_LEN]; 	      //发送缓冲,最大USART3_MAX_SEND_LEN字节
uint8_t  USART3_RX_BUF[USART3_MAX_RECV_LEN] ;
/************************************************
 ALIENTEK 阿波罗STM32F429开发板
 ATK-LORA-01模块功能驱动
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com  
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

//设备参数初始化(具体设备参数见lora_cfg.h定义)
_LoRa_CFG LoRa_CFG=
{
	.addr = LORA_ADDR,       //设备地址
	.power = LORA_POWER,     //发射功率
	.chn = LORA_CHN,         //信道
	.wlrate = LORA_RATE,     //空中速率
	.wltime = LORA_WLTIME,   //睡眠时间
	.mode = LORA_MODE,       //工作模式
	.mode_sta = LORA_STA,    //发送状态
	.bps = LORA_TTLBPS ,     //波特率设置
	.parity = LORA_TTLPAR    //校验位设置
};

//设备工作模式(用于记录设备状态)
uint8_t Lora_mode=0;// 0:配置模式 1:接收模式 2:发送模式

uint32_t obj_addr=0;//记录目标地址和信道（用于显示测试框右上角）（用户通过键盘数据输入时最大出现99999,所以需要定义uint32_t类型）
uint8_t obj_chn=0;

//将收到的AT指令应答数据返回给电脑串口
//mode:0,不清零USART3_RX_STA;
//     1,清零USART3_RX_STA;
void lora_at_response(uint8_t mode)
{
	if(USART3_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
		if(mode)USART3_RX_STA=0;
	} 
}

//lora发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//其他,期待应答结果的位置(str的位置)
uint8_t* lora_check_cmd(uint8_t *str)
{
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (uint8_t*)strx;
}

//lora发送命令
//cmd:发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串.
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
uint8_t lora_send_cmd(uint8_t *cmd,uint8_t *ack,uint16_t waittime)
{
	uint8_t res=0; 
	USART3_RX_STA=0;
 
	if((uint32_t)cmd<=0XFF)
	{			
		while((USART3->SR&0X40)==0);//等待上一次数据发送完成  
		USART3->DR=(uint32_t)cmd;
		
	}else u3_printf("%s\r\n",cmd);//发送命令
	
	if(ack&&waittime)		//需要等待应答
	{
	   while(--waittime)	//等待倒计时
	   { 
		  HAL_Delay(10);		
		  if(USART3_RX_STA&0X8000)//接收应答
		  {			
			  if(lora_check_cmd(ack))//接收到期待的应答结果
			  {	
				  USART3_RX_STA=0;
				  break;//得到有效数据
			  }  
		  } 
	   }
	   if(waittime==0)res=1; 
	}
	return res;
} 

//记录中断状态
static uint8_t Rec_mode=0;//0：关闭 1:上升沿 2:下降沿
//AUX中断设置
//mode:配置的模式 0:关闭 1:上升沿 2:下降沿
void Aux_Int(uint8_t mode)
{
	
	GPIO_InitTypeDef GPIO_Initure={0};
	if(!mode)
	{
		HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);//失能中断线11
	}else
	{
		if(mode==1)
		{
			
			 GPIO_Initure.Pin=lora_AUX_Pin; 
       GPIO_Initure.Mode=GPIO_MODE_IT_RISING;//上升沿触发
		}
			
	    else if(mode==2)
		{
			 GPIO_Initure.Pin=lora_AUX_Pin; 
       GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;//下降沿触发
		}
		
		 HAL_GPIO_Init(lora_AUX_GPIO_Port,&GPIO_Initure);//重新初始化
	     __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_11);//清除中断标志位
		 HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,0); //抢占优先级为2，子优先级为0
		 HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);//使能
	 
	}
	Rec_mode = mode;//记录中断模式
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
    __HAL_UART_DISABLE(&huart3);//关闭串口
	
	huart3.Init.BaudRate = bound;
	huart3.Init.StopBits = UART_STOPBITS_1; 
	
	if(parity==LORA_TTLPAR_8N1)//无校验
	{
		huart3.Init.WordLength= UART_WORDLENGTH_8B;    
		huart3.Init.Parity = UART_PARITY_NONE;
	}else if(parity==LORA_TTLPAR_8E1)//偶校验
	{
		huart3.Init.WordLength= UART_WORDLENGTH_9B;    
		huart3.Init.Parity = UART_PARITY_EVEN;
	}else if(parity==LORA_TTLPAR_8O1)//奇校验
	{
		huart3.Init.WordLength = UART_WORDLENGTH_9B;    
		huart3.Init.Parity = UART_PARITY_ODD;
	}
    HAL_UART_Init(&huart3);	
	
} 

//LoRa模块初始化
//返回值:0,检测成功
//       1,检测失败 
uint8_t LoRa_Init(void)
{
	 uint8_t retry=0;
	 uint8_t temp;

	LORA_MD0(0);
	 
	while(LORA_AUX)//确保LORA模块在空闲状态下(LORA_AUX=0)
	{
		 WS_Debug("BUSY!!"); 	 
	}
	LORA_MD0(1);//进入AT模式
	HAL_Delay(40); 
	retry=3;
	while(retry--)
	  {
		
		 if(!lora_send_cmd((uint8_t *)"AT",(uint8_t *)"OK",70))
		 {
			 temp=0;//检测成功
			 WS_Debug("LoRa_Init OK\r\n"); 	 
			 break;
		 }
		 	 
	  if(retry==0) {
			 WS_Debug("LoRa_Init ERROR\r\n"); 	 
			 temp=1;//检测失败
			}
		 
	  }
	  return temp;
}

//串口3,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
char formatBuf3[64];
void u3_printf(char *p, ...)
{
    /*  格式化数据  */
    va_list ap;
    va_start(ap, p);
    vsprintf(formatBuf3, p, ap);
    va_end(ap);
    HAL_UART_Transmit_DMA(&huart3, (uint8_t *)formatBuf3, strlen(formatBuf3));
}

//串口接收使能控制
//enable:0,关闭 1,打开
void usart3_rx(uint8_t enable)
{
	
	 __HAL_UART_DISABLE(&huart3); //失能串口 
	
	 if(enable)
	 {
		 huart3.Init.Mode=UART_MODE_TX_RX;//收发模式
	 }else
	 {
		  huart3.Init.Mode = UART_MODE_TX;//只发送  
	 }
	 
	 HAL_UART_Init(&huart3); //HAL_UART_Init()会使能UART3
}

//Lora模块参数配置
void LoRa_Set(void)
{
	uint8_t sendbuf[20];
	uint8_t lora_addrh,lora_addrl=0;
	
	usart3_set(LORA_TTLBPS_115200,LORA_TTLPAR_8N1);//进入配置模式前设置通信波特率和校验位(115200 8位数据 1位停止 无数据校验）
	usart3_rx(1);//开启串口3接收
	
	while(LORA_AUX);//等待模块空闲
	LORA_MD0(1); //进入配置模式
	HAL_Delay(40);
	Lora_mode=0;//标记"配置模式"
	
	lora_addrh =  (LoRa_CFG.addr>>8)&0xff;
	lora_addrl = LoRa_CFG.addr&0xff;
	sprintf((char*)sendbuf,"AT+ADDR=%02x,%02x",lora_addrh,lora_addrl);//设置设备地址
	lora_send_cmd(sendbuf,(uint8_t *)"OK",50);
	sprintf((char*)sendbuf,"AT+WLRATE=%d,%d",LoRa_CFG.chn,LoRa_CFG.wlrate);//设置信道和空中速率
	lora_send_cmd(sendbuf,(uint8_t *)"OK",50);
	sprintf((char*)sendbuf,"AT+TPOWER=%d",LoRa_CFG.power);//设置发射功率
	lora_send_cmd(sendbuf,(uint8_t *)"OK",50);
	sprintf((char*)sendbuf,"AT+CWMODE=%d",LoRa_CFG.mode);//设置工作模式
	lora_send_cmd(sendbuf,(uint8_t *)"OK",50);
	sprintf((char*)sendbuf,"AT+TMODE=%d",LoRa_CFG.mode_sta);//设置发送状态
	lora_send_cmd(sendbuf,(uint8_t *)"OK",50);
	sprintf((char*)sendbuf,"AT+WLTIME=%d",LoRa_CFG.wltime);//设置睡眠时间
	lora_send_cmd(sendbuf,(uint8_t *)(uint8_t *)"OK",50);
	sprintf((char*)sendbuf,"AT+UART=%d,%d",LoRa_CFG.bps,LoRa_CFG.parity);//设置串口波特率、数据校验位
	lora_send_cmd(sendbuf,(uint8_t *)"OK",50);

	LORA_MD0(0);//退出配置,进入通信
	HAL_Delay(40);
	while(LORA_AUX);//判断是否空闲(模块会重新配置参数)
	USART3_RX_STA=0;
	Lora_mode=1;//标记"接收模式"
	usart3_set(LoRa_CFG.bps,LoRa_CFG.parity);//返回通信,更新通信串口配置(波特率、数据校验位)
	Aux_Int(1);//设置LORA_AUX上升沿中断
	memset((char*)USART3_RX_BUF,0x00,sizeof(USART3_RX_BUF));//串口接收缓冲区清0
}


uint8_t Dire_Date[]={0x11,0x22,0x33,0x44,0x55};//定向传输数据
uint8_t date[30]={0};//定向数组
uint8_t Tran_Data[30]={0};//透传数组

#define Dire_DateLen sizeof(Dire_Date)/sizeof(Dire_Date[0])
	
//Lora模块发送数据
void LoRa_SendData(void)
{      
	static uint8_t num=0;
  uint16_t addr;
	uint8_t chn;
	uint16_t i=0; 
		
	if(LoRa_CFG.mode_sta == LORA_STA_Tran)//透明传输
	{
		sprintf((char*)Tran_Data,"ATK-LORA-01 TEST %d",num);
		u3_printf("%s\r\n",Tran_Data);	
		num++;
		if(num==255) num=0;
		
	}else if(LoRa_CFG.mode_sta == LORA_STA_Dire)//定向传输
	{
		
		addr = (uint16_t)obj_addr;//目标地址
		chn = obj_chn;//目标信道
		
		date[i++] =(addr>>8)&0xff;//高位地址
		date[i++] = addr&0xff;//低位地址
		date[i] = chn;  //无线信道
		for(i=0;i<Dire_DateLen;i++)//数据写到发送BUFF
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
   
	//有数据来了
	if(USART3_RX_STA&0x8000)
	{
		len = USART3_RX_STA&0X7FFF;
		USART3_RX_BUF[len]=0;//添加结束符
		USART3_RX_STA=0;
    WS_Debug("%s",USART3_RX_BUF);
		memset((char*)USART3_RX_BUF,0x00,len);//串口接收缓冲区清0
	}

}
//  LoRa任务
int WS_LoRa_Task_ReceData(void *date)
{	
	
    LoRa_ReceData() ;  
	
	  return 10;
}