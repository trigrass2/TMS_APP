1.����ʱ����ӵ�ͷ�ļ��ĺ궨��
USE_STDPERIPH_DRIVER,STM32F40_41xxx,HAVE_CONFIG_H,DEBUG,OPT_SPEED,
FPM_DEFAULT,STDC_HEADERS,USE_USB_OTG_FS,USE_STM324xG_EVAL,USE_EMBEDDED_PHY,
__FPU_PRESENT = 1,__FPU_USED=1,ARM_MATH_CM4

2. �����ļ��У��޸���PendSV_Handler ��SysTick_Handler  �ĺ������֣����жϷ�����������Ҳ�ı���
;DCD     PendSV_Handler             ; PendSV Handler

;DCD     SysTick_Handler            ; SysTick Handler

DCD     OS_CPU_PendSVHandler

DCD     OS_CPU_SysTickHandler

3.2016-05-12  ʹ��OS_CPU_SysTickHandler�����жϱȽ��鷳�����޸��������ļ�
DCD     PendSV_Handler             ; PendSV Handler

DCD     SysTick_Handler            ; SysTick Handler

;DCD     OS_CPU_PendSVHandler

;DCD     OS_CPU_SysTickHandler

3. �ڱ�׼�̼���1.5�С���Щ�ļ�stm32f407��û����Щ���ܵġ�����Ҫ�Ƴ�����
	stm32f4xx_cec.c		stm32f4xx_flash_ramfunc.c	stm32f4xx_fmc.c		stm32f4xx_fmpi2c.c
	stm32f4xx_spdifrx.c		stm32f4xx_qspi.c
