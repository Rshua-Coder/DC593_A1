#include "GSensor.h"
#include "Gsensor_DA380.h"
#include "SwTimer.h"
#include "Debug.h"
#include <stdlib.h>

#define abs(x) (((x) < 0) ? -(x) : (x))

static BOOL Gsensor_Check_ID(void);
static BOOL GSensor_open(void);
static BOOL GSensor_close(void);
static BOOL GSensor_GetData(Gsensor_Data *GS_Data);
static BOOL Gsensor_GetMotionFlag(void);
static void GSensor_SetSensitivity(UINT32 Thres );
static void GSensor_ParkingMode(BOOL OnOff);
static BOOL GSensor_getparkwakeupIntStatus(void);

static INT32 GsXData = 0,OldGsXData = 0;
static INT32 GsYData = 0,OldGsYData = 0;
static INT32 GsZData = 0,OldGsZData = 0;
unsigned int DA380Threshold=0x45;/*1~0x7f*/

static BOOL b_open = FALSE;
GSENSOR_OBJ GsensorObj_DA380 = {
    "drhDA380",
    Gsensor_Check_ID,
    GSensor_open,
    GSensor_close,
    GSensor_GetData,
    Gsensor_GetMotionFlag,
    GSensor_SetSensitivity,
    GSensor_ParkingMode,
    GSensor_getparkwakeupIntStatus
};


/*return value: 0: is ok    other: is failed*/
int     i2c_read_byte_data( unsigned char addr, unsigned char *data)
{
    UINT32 uiData = 0;
    uiData = GSensor_I2C_ReadReg(addr);
    *data =  (unsigned char)uiData;
    return 0;
}

/*return value: 0: is ok    other: is failed*/
int     i2c_write_byte_data( unsigned char addr, unsigned char data)
{
    GSensor_I2C_WriteReg(addr,data);
    return 0;
}

/*return value: 0: is count    other: is failed*/
int     i2c_read_block_data( unsigned char base_addr, unsigned char count, unsigned char *data)
{
    int i = 0;
    UINT32 uiData = 0;

    for(i = 0; i < count;i++)
    {
        uiData = GSensor_I2C_ReadReg(base_addr+i);
        *data =  (unsigned char)uiData;
        data ++;
    }	

    return count;
}

int mir3da_register_read( unsigned char addr, unsigned char *data)
{
    int     res = 0;

    res = i2c_read_byte_data(addr, data);
    if(res != 0) {
          return res;
    }	

    return res;
}

int mir3da_register_write( unsigned char addr, unsigned char data)
{
    int     res = 0;

    res = i2c_write_byte_data(addr, data);
    if(res != 0) {
         return res;
    }

    return res;
}

int mir3da_register_read_continuously( unsigned char addr, unsigned char count, unsigned char *data)
{
    int     res = 0;

    res = (count==i2c_read_block_data(addr, count, data)) ? 0 : 1;
    if(res != 0) {
         return res;
    }

    return res;
}

int mir3da_register_mask_write(unsigned char addr, unsigned char mask, unsigned char data)
{
    int     res = 0;
    unsigned char      tmp_data;
    res = mir3da_register_read(addr, &tmp_data);
    if(res) {
        return res;    
    }    
    tmp_data &= ~mask;
    tmp_data |= data & mask;
    res = mir3da_register_write(addr, tmp_data);
    return res;
}

/*return value: 0: is ok    other: is failed*/
int mir3da_init(void)
{
	mir3da_register_mask_write(NSA_REG_SPI_I2C, 0x24, 0x24);
	Delay_DelayMs(5);
	mir3da_register_mask_write(NSA_REG_G_RANGE, 0x03, 0x02);
	mir3da_register_mask_write(NSA_REG_POWERMODE_BW, 0xFF, 0x5E);
	mir3da_register_mask_write(NSA_REG_ODR_AXIS_DISABLE, 0xFF, 0x07);
	mir3da_register_mask_write(NSA_REG_INT_PIN_CONFIG, 0x0F, 0x05);//set int_pin level
	mir3da_register_mask_write(NSA_REG_INT_LATCH, 0x8F, 0x86);//clear latch and set latch mode
	mir3da_register_mask_write(NSA_REG_ENGINEERING_MODE, 0xFF, 0x83);
	mir3da_register_mask_write(NSA_REG_ENGINEERING_MODE, 0xFF, 0x69);
	mir3da_register_mask_write(NSA_REG_ENGINEERING_MODE, 0xFF, 0xBD);

	return TRUE;	    	
}

int mir3da_open_interrupt(int num)
{
    int   res = 0;

    res = mir3da_register_write(NSA_REG_INTERRUPT_SETTINGS1,0x03);
    res = mir3da_register_write(NSA_REG_ACTIVE_DURATION,0x03 );
    res = mir3da_register_write(NSA_REG_ACTIVE_THRESHOLD,0x0A );

    switch(num){
        case 0:
        res = mir3da_register_write(NSA_REG_INTERRUPT_MAPPING1,0x04 );
        break;

        case 1:
        res = mir3da_register_write(NSA_REG_INTERRUPT_MAPPING3,0x04 );
        break;
    }

    return res;
}

int mir3da_close_interrupt(int num)
{
    int   res = 0;

    res = mir3da_register_write(NSA_REG_INTERRUPT_SETTINGS1,0x00 );

    switch(num){
        case 0:
        res = mir3da_register_write(NSA_REG_INTERRUPT_MAPPING1,0x00 );
        break;

        case 1:
        res = mir3da_register_write(NSA_REG_INTERRUPT_MAPPING3,0x00 );
        break;
    }

    return res;
}

/*return value: 0: is ok    other: is failed*/
int mir3da_read_data(short *x, short *y, short *z)
{
    unsigned char    tmp_data[6] = {0};
	unsigned char data = 0;

    if (mir3da_register_read_continuously(NSA_REG_ACC_X_LSB, 6, tmp_data) != 0) {
        return -1;
    }
    
    *x = ((short)(tmp_data[1] << 8 | tmp_data[0]))>> 4;
    *y = ((short)(tmp_data[3] << 8 | tmp_data[2]))>> 4;
    *z = ((short)(tmp_data[5] << 8 | tmp_data[4]))>> 4;
    (*x)*=32;
    (*y)*=32;
    (*z)*=32;

	//debug_msg("oringal x y z is %d %d %d\r\n",*x,*y,*z);
    return TRUE;
}

char mir3da_read_motion_flag(void)
{
    char data = 0;

    mir3da_register_read(NSA_REG_MOTION_FLAG,&data);
    if(data&0x04)
        return 1;

    return 0;
}

static BOOL Gsensor_Check_ID(void)
{
    unsigned char data = 0;

    if (GSensor_I2C_ResetSlaveAddr(NSA_REG_I2C_WADDR, NSA_REG_I2C_RADDR))
    {
        mir3da_register_read(NSA_REG_WHO_AM_I,&data);
    }
    if (data == 0x13)
    {
        debug_msg("^G Gsensor Type:%s\r\n", GsensorObj_DA380.name);
        return TRUE;
    }
    else
    {
		debug_msg("^G %s Failse!\r\n", GsensorObj_DA380.name);
        return FALSE;
    }
}

static BOOL GSensor_open(void)
{
    if (!b_open)
    {
        b_open = TRUE;
        return mir3da_init();
    }
    return TRUE;
}

static BOOL GSensor_close(void)
{
	return TRUE;
}

static BOOL GSensor_GetData(Gsensor_Data *gsData)
{
    static BOOL b_firstread = TRUE;
    UINT32 G_INT_COUNT=0;
    unsigned int i;
    int abs_cal_x, abs_cal_y, abs_cal_z;
    UINT32 Threshold = 0xFFFFFFFF;
    short    gSensorXYZData[3]={0};
    if (b_firstread)
    {
        b_firstread = FALSE;
        if (!mir3da_read_data(&gSensorXYZData[0],&gSensorXYZData[1],&gSensorXYZData[2]))
        {
        return FALSE;
        }

        GsXData =gSensorXYZData[0];
        GsYData =gSensorXYZData[1];
        GsZData =gSensorXYZData[2];	
        OldGsXData = GsXData;
        OldGsYData = GsYData;
        OldGsZData = GsZData;
    }
    if (!mir3da_read_data(&gSensorXYZData[0],&gSensorXYZData[1],&gSensorXYZData[2]))
    {
    	return FALSE;
    }

    GsXData =gSensorXYZData[0];
    GsYData =gSensorXYZData[1];
    GsZData =gSensorXYZData[2];	

    abs_cal_x=abs(OldGsXData - GsXData);
    abs_cal_y=abs(OldGsYData-GsYData);
    abs_cal_z=abs(OldGsZData-GsZData);
    switch (DA380Threshold) {
        case GSENSOR_SENSITIVITY_LOW://80:
        //Threshold = 25000;
        Threshold = 35000;
        break;
        
        case GSENSOR_SENSITIVITY_MED://50:
        //Threshold = 20000;
        Threshold = 30000;
        break;

        case GSENSOR_SENSITIVITY_HIGH://20:
        //Threshold = 15000;
        Threshold = 25000;
        break;

        case GSENSOR_SENSITIVITY_OFF:
        defualt:
        Threshold = 80000;
        break;
    }
    OldGsXData = GsXData;
    OldGsYData = GsYData;
    OldGsZData = GsZData;

    //debug_msg("^G Gsensor GSensor_GetData X:%d, Y:%d, Z:%d  Thr:%d\r\n",abs_cal_x,abs_cal_y,abs_cal_z,Threshold);

    if(abs_cal_x >= Threshold || abs_cal_y >= Threshold || abs_cal_z >= Threshold)
    {
        //debug_msg("###abs_cal_x: %d### abs_cal_y:%d ### abs_cal_z: %d ###Threshold:%d\r\n",abs_cal_x,abs_cal_y,abs_cal_z, Threshold);
        debug_msg("----------MEMSDA380 EMG----------\n\r");
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOL Gsensor_GetMotionFlag(void)
{
    return mir3da_read_motion_flag();
}

static void GSensor_SetSensitivity(UINT32 Thres )
{
	DA380Threshold=Thres;
}

static void GSensor_ParkingMode(BOOL OnOff)
{
	if (OnOff) {
		mir3da_open_interrupt(0);
	}
}

static BOOL GSensor_getparkwakeupIntStatus(void)
{
    unsigned char RegIntSetValue = 0;
    unsigned char RegIntMapValue = 0;
    unsigned char RegActiveIntValue = 0;
    mir3da_register_read(NSA_REG_INTERRUPT_SETTINGS1, &RegIntSetValue);
    mir3da_register_read(NSA_REG_INTERRUPT_MAPPING1,& RegIntMapValue); 
    mir3da_register_read(NSA_REG_MOTION_FLAG,& RegActiveIntValue); 

    if (RegIntSetValue == 0x03 && RegIntMapValue == 0x04 && ((RegActiveIntValue&0x04) != 0x0))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
