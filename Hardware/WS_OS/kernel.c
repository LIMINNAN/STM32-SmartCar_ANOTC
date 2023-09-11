#include "kernel.h"

WspTsak   pOnhead,pRunhead,pHanghead;
Task   WsTsak[TaskLen];

// 任务函数指针
int ( *TsakFun)(void *);


void InitTask(void)
{  char i;
   pOnhead=0,pRunhead=0;pHanghead = 0;
	 for(i=0;i<TaskLen;i++)
	  {
	   WsTsak[i].ID = 0       ;     //  使用标志清零
		 WsTsak[i].next = 0     ;
		 WsTsak[i].parameter = 0;
		}
}

char StartTask(char id,int time)  //  启动挂起任务
{  
	  WspTsak pt,pt1;
    pt1 = pt = pHanghead;
	  while(pt != 0)
		{	
      if(pt->ID == id)
			{  
        DI_INTERRUPT                // 关闭总中断			
				if(pt == pHanghead )        // 把任务从挂起列表中删除
				    pHanghead = pt->next;
				else
					  pt1->next = pt->next; 
				
				 pt->Time = time   ;		
				 if(time == 0)     //  立刻执行任务,添加到执行列   
            AddRunTask(pt) ; 
				 else              //  添加到计时列表  
            AddOnTask(pt)  ;					 
         
         EN_INTERRUPT		   //  打开总中断
         return WS_Status_OK;       //  返回成功
				
      }
			pt1 = pt;
			pt  = pt->next;
    }
		return WS_Status_ERROR; //  失败，未找到相应任务    	
}

#if WS_MSG_EN > 0

char setTaskMsg(char taskid,Message msg)
{
    WspTsak pt;
    pt = pHanghead;
	  while(pt != 0)
		{	
      if(pt->ID == taskid)
			{  
         pt->msg  = msg;
         return 1;         //  返回成功			
      }
			pt = pt->next;
    }
		return 0; //  失败，位找到相应任务    	
}

#endif

char StopTask(char id)         //  将任务挂起
{
   WspTsak pt,pt1;
	 DI_INTERRUPT                // 关闭总中断	
	 if(pOnhead != 0)            // 计时列表
	 {  
      pt1 =  pt = pOnhead;
		  while(pt != 0)
			{   
				  if(pt->ID == id)
					{  DI_INTERRUPT                // 关闭总中断		
						 if(pt == pOnhead)          
						  pOnhead = pt->next    ; 
             else 
              pt1->next = pt->next  ; 
           
             AddHangTask(pt)        ; 
             EN_INTERRUPT		              //  打开总中断					 
             return WS_Status_OK;                    //  任务挂起成功，返回1
          }
					pt1 = pt;
          pt = pt->next;
      }
   }
	 
	 if(pRunhead != 0)  //  运行列表
	 {  
      pt1 =  pt = pRunhead;
		  while(pt != 0)
			{   
				  if(pt->ID == id)
					{  DI_INTERRUPT                // 关闭总中断		
						 if(pt == pRunhead)          
						  pRunhead = pt->next    ; 
             else 
              pt1->next = pt->next  ; 
           
             AddHangTask(pt)        ; 
             EN_INTERRUPT		              //  打开总中断					 
             return WS_Status_OK;                    //  任务挂起成功，返回1
          }
					pt1 = pt;
          pt = pt->next;
      }
   }
	 
	 
	 EN_INTERRUPT		              //  打开总中断	
	 return WS_Status_ERROR;                  
}

char DeleteTask(char id) //  删除任务
{
	 WspTsak pt,pt1;
   StopTask(id);
	  DI_INTERRUPT                // 关闭总中断	
	
	 pt1 = pt = pHanghead;
	 while(pt != 0)
		{	
      if(pt->ID == id)
			{  
       			
				if(pt == pHanghead )        // 把任务从挂起列表中删除
				    pHanghead = pt->next;
				 else
					  pt1->next = pt->next; 
				
				 pt->ID = 0   ;		         //  删除ID
         EN_INTERRUPT		              //  打开总中断		 
         return WS_Status_OK;                   //  返回成功
				
      }
			pt1 = pt;
			pt = pt->next;
    }
	
	  EN_INTERRUPT		              //  打开总中断	
	 return WS_Status_ERROR;  //  找不到任务
}

//  创建定时器
char CreateTimer(int (*fun)(void *),int time)
{
	 char i; 
   
   if(time <= 0)   return 0; 	     //  无效时间，返回错误
	
	 for(i=0;i<TaskLen;i++)          //  寻找当前函数是否已经创建定时器
	 {
		  if(WsTsak[i].fun == fun)  
			{
				WsTsak[i].Time = time;
				return WS_Status_OK;       //  已经创建，更新时间
			}
	 } 
	 for(i=0;i<TaskLen;i++)          //  寻找空任务栈
	 {  
      if(WsTsak[i].ID == 0)
			{  		 
				 WsTsak[i].ID   = -1;      //  定时器任务，统一用 -1 ID
         WsTsak[i].fun  = (int (*)(void * ))fun;
				 WsTsak[i].Time = time;
				 WsTsak[i].next = 0;

				 if(time == 0)     //  立刻执行任务,添加到执行列   
            AddRunTask(&WsTsak[i]) ; 
				 else              //  添加到计时列表  
            AddOnTask(&WsTsak[i])  ;
				 
				 //AddOnTask(&WsTsak[i]);	   //  添加到计时列表 
				 
				 return WS_Status_OK;  
      }
   } 
   return WS_Status_ERROR;
}

//  创建任务
char CreateTask(char id,int (*fun)(void *),int time)
{
	 char i;
	
	 for(i=0;i<TaskLen;i++)              //  查看ID是否为唯一
	  {
       if(WsTsak[i].ID == id)  
				  return WS_Status_BUSY;      //   返回0失败
    }
	 for(i=0;i<TaskLen;i++)              //  寻找空任务栈
	 {  
      if(WsTsak[i].ID == 0)
			{  
				 WsTsak[i].ID   = id;
         WsTsak[i].fun  = (int (*)(void *))fun;
				 WsTsak[i].Time = time;
				 WsTsak[i].next = 0;
				 
					 if(time > 0)     // 加入计时列表
					 {
						 AddOnTask(&WsTsak[i])  ;
					 }
					 else             // 创建 挂起任务
					 {
             AddHangTask(&WsTsak[i]);
           }
			  
			   return WS_Status_OK; 
      }
   }
	 
  return WS_Status_ERROR;   
}

void AddHangTask(WspTsak ptk)  //  添加任务到挂起列表
{  WspTsak pt; 
	 ptk->next = 0                ;
	 do
	 {
		 if(pHanghead == 0)
		 {
			 pHanghead = ptk;
       break;			 
		 }
		 
		 pt = pHanghead;           //  查找表尾
		 while(pt->next !=0)    
		 {
			 pt = pt->next;
		 }
		 pt->next = ptk;
	 }while(0);
}

void AddOnTask(WspTsak ptk)  //  添加任务到定时区
{   
	  WspTsak  pt;
	  ptk->next  = 0;
	  do
		{	
			if(pOnhead == 0)
			 {
				 pOnhead = ptk;
				 break;                //  推出
			 } 
			 pt = pOnhead;           //  查找表尾
			 while(pt->next !=0)    
			 {
				 pt = pt->next;
			 }
			 pt->next = ptk;
	 }while(0);
}
void OnTask(void)
{   
     WspTsak list,list1;      
	   list1 = list = pOnhead;     //  两个指针指向前后两个任务，便于删除就绪的任务
     while(list != 0)
		 {  
        if(list->Time > 0) list->Time --;  //  定时计数减一
			  if(list->Time == 0)                //  任务定时时间到
				{  
           if(list == pOnhead)             //  第一个任务时间到  
						  pOnhead = list->next      ;  //  改变表头指向
           else 
              list1->next = list->next  ;  //  删除当前就绪任务
           
           AddRunTask(list)             ;	 //  将任务添加到待执行列表				 
        }		
				list1 = list;       //  指向上一个任务
			  list = list->next;  //  指向下一个任务
     }	 
}

void AddRunTask(WspTsak ptk)
{
   WspTsak  list;
	 ptk->next  =  0;        //   末端任务标志
	 list = pRunhead;
	 if(list == 0)           //  等待执行列表为空
	 {
     pRunhead = ptk;  
   }
	 else
	 {
     while(list->next != 0)
		 {
       list = list->next;
     } 
		 list->next  = ptk  ;
   }
}
void RunTask(void)
{    int time;
	   WspTsak  pt;
	   while(pRunhead != 0)         //  任务列表不为空
		 { 
        TsakFun = pRunhead->fun;  //  执行任务
			  time = (*TsakFun)(pRunhead->parameter)    ; 
			  pRunhead->parameter = NULL;                  //  清除任务参数 
			  if(pRunhead->ID > 0)        //  任务栈
				{
					pt   = pRunhead->next;
					if(time >0)               //    根据任务返回值，决定任务去向
					{
						pRunhead->Time = time;  
						AddOnTask(pRunhead)  ;  //  添加任务到定时区 
					}
					else                      //   将任务添加到挂起列表
					{
						AddHangTask(pRunhead);
					}
          pRunhead = pt	;		//  执行一个任务，从链表里删除一个任务	
				}
				else                         //  定时器任务，直接去掉
				{
					pRunhead->ID = 0;          //  删除任务
					pRunhead->fun = 0;
          pRunhead = pRunhead->next;
        }
     }    
}


//  写入任务形参
char WS_OS_Write_Task_Parameter(short int id , void *parameter, int delayTime )
{
     
    WspTsak pt,pt1;
	
	  if(parameter == NULL || id <= 0)  return 0;
	
	  //   查询挂起列表
    pt1 = pt = pHanghead;
	  while(pt != 0)
		{	
      if(pt->ID == id)
			{  
        DI_INTERRUPT                // 关闭总中断			
				if(pt == pHanghead )        // 把任务从挂起列表中删除
				    pHanghead = pt->next;
				else
					  pt1->next = pt->next; 
				
				 pt->Time = delayTime      ;	
				 pt->parameter = parameter ;
         				
				 if(delayTime == 0)     //  立刻执行任务,添加到执行列   
            AddRunTask(pt) ; 
				 else              //  添加到计时列表  
            AddOnTask(pt)  ;					 
         
         EN_INTERRUPT		   //  打开总中断
         return WS_Status_OK;         //  返回成功
				
      }
			pt1 = pt;
			pt  = pt->next;
    }
		
		//  查询计时列表
	  pt =	pOnhead ;	
		while(pt != 0)
		{	
      if(pt->ID == id)
			{  
        DI_INTERRUPT                 // 关闭总中断			
			  pt->parameter = parameter ;
        EN_INTERRUPT		             //  打开总中断
        return WS_Status_OK;         //  返回成功		
      }
			pt  = pt->next;
    }
		return WS_Status_ERROR           ; //  失败，未找到相应任务    
}

