#include "stm8s.h"
#include "my_board.h"
#include "uart2_user.h"
#include "data_processing.h"

extern void Flash_writedata(u8 *write_buf, u16 number, int start_address)
{
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    for (int i = 0; i < number; i++)
    {
        FLASH_EraseByte(start_address + i);
        Delayms_TIMER4(20);
        FLASH_ProgramByte(start_address + i, *(write_buf + i));
        Delayms_TIMER4(20);
    }
    FLASH_Lock(FLASH_MEMTYPE_DATA);
}
extern void Flash_readdata(u8 *read_buf, u16 number, int start_address)
{
    for (int i = 0; i < number; i++)
    {
        *(read_buf + i) = FLASH_ReadByte(start_address + i); 
    }
}
