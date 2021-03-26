
/*
!!!Linker default script generated automatically !!!
Please note that this file and all script files included from this one can not be modified directly
If you need to use your own script please do it from Ride7 LD linker options taking this one as example
*/

SEARCH_DIR(".")
SEARCH_DIR("C:\tien\PROJEKTY\procesory\STM\T104_V100a_GASTOP")
SEARCH_DIR("C:\tien\PROJEKTY\procesory\STM\T104_V100a_GASTOP\library\src")
SEARCH_DIR("C:\Program Files (x86)\Raisonance\Ride\Lib\ARM")
STARTUP("C:\Program Files (x86)\Raisonance\Ride\lib\ARM\crt0_STM32x_HD.o")
INPUT("C:\tien\PROJEKTY\procesory\STM\T104_V100a_GASTOP\stm32f10x_flash.o")
INPUT("C:\tien\PROJEKTY\procesory\STM\T104_V100a_GASTOP\control.o")
INPUT("C:\tien\PROJEKTY\procesory\STM\T104_V100a_GASTOP\sys.o")
INPUT("C:\tien\PROJEKTY\procesory\STM\T104_V100a_GASTOP\stm32f10x_it.o")
INPUT("C:\tien\PROJEKTY\procesory\STM\T104_V100a_GASTOP\RTC.o")
INPUT("C:\tien\PROJEKTY\procesory\STM\T104_V100a_GASTOP\main.o")
INPUT("C:\tien\PROJEKTY\procesory\STM\T104_V100a_GASTOP\lcd.o")
INPUT("C:\tien\PROJEKTY\procesory\STM\T104_V100a_GASTOP\hardware.o")
INPUT("C:\tien\PROJEKTY\procesory\STM\T104_V100a_GASTOP\gui.o")
INPUT("C:\tien\PROJEKTY\procesory\STM\T104_V100a_GASTOP\usb_core.o")
INPUT("C:\tien\PROJEKTY\procesory\STM\T104_V100a_GASTOP\usb_init.o")
INPUT("C:\tien\PROJEKTY\procesory\STM\T104_V100a_GASTOP\usb_int.o")
INPUT("C:\tien\PROJEKTY\procesory\STM\T104_V100a_GASTOP\usb_mem.o")
INPUT("C:\tien\PROJEKTY\procesory\STM\T104_V100a_GASTOP\usb_regs.o")
INPUT("C:\tien\PROJEKTY\procesory\STM\T104_V100a_GASTOP\usb_desc.o")
INPUT("C:\tien\PROJEKTY\procesory\STM\T104_V100a_GASTOP\usb_istr.o")
INPUT("C:\tien\PROJEKTY\procesory\STM\T104_V100a_GASTOP\usb_prop.o")
INPUT("C:\tien\PROJEKTY\procesory\STM\T104_V100a_GASTOP\usb_pwr.o")
INPUT("C:\tien\PROJEKTY\procesory\STM\T104_V100a_GASTOP\usb_endp.o")
INCLUDE "C:\tien\PROJEKTY\procesory\STM\T104_V100a_GASTOP\STM32F103_256K_48K_FLASH.ld"
EXTERN( __io_putchar )
EXTERN( _write )
INPUT("STM32x_io_putchar_thumb.a")
INPUT("e_stdio_thumb.a")
INPUT("STM32F10x_thumb.lib")
OUTPUT("C:\tien\PROJEKTY\procesory\STM\T104_V100a_GASTOP\T104_GA_V100a.elf")
