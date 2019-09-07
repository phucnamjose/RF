#ifndef __DATA_PROCESSING_H
#define __DATA_PROCESSING_H


extern void Flash_writedata(u8 *write_buf, u16 number, int start_address);
extern void Flash_readdata(u8 *read_buf, u16 number, int start_address);


#endif /* __DATA_PROCESSING_H */